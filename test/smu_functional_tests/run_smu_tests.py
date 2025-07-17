import ctypes
import csv
import sys
import os

# 定义结构体，与C端保持一致
class Request(ctypes.Structure):
    _fields_ = [
        ('addr', ctypes.c_uint32),
        ('data', ctypes.c_uint32),
        ('r', ctypes.c_uint8),
        ('w', ctypes.c_uint8),
        ('fault', ctypes.c_uint32),
        ('faultBit', ctypes.c_uint8),
    ]

class Result(ctypes.Structure):
    _fields_ = [
        ('cycles', ctypes.c_uint32),
        ('errors', ctypes.c_uint32),
    ]

def load_requests_from_csv(filename):
    requests = []
    with open(filename, newline='') as csvfile:
        reader = csv.DictReader(csvfile)
        for row in reader:
            # 支持十六进制和十进制输入
            def parse_int(val):
                val = val.strip()
                if val.startswith('0x') or val.startswith('0X'):
                    return int(val, 16)
                return int(val)
            req = Request(
                parse_int(row['addr']),
                parse_int(row['data']),
                int(row['r']),
                int(row['w']),
                parse_int(row['fault']),
                int(row['faultBit']),
            )
            requests.append(req)
    return requests

def main():
    # 加载 so 库
    lib_path = os.path.join(os.path.dirname(__file__), "libsmu.so")
    lib = ctypes.CDLL(lib_path)

    # 设置C函数参数和返回类型
    lib.run_simulation.argtypes = [
        ctypes.c_uint32,        # max_cycles
        ctypes.c_char_p,        # tracefile
        ctypes.c_uint8,         # endianness
        ctypes.c_uint32,        # latency_scrambling
        ctypes.c_uint32,        # latency_encryption
        ctypes.c_uint32,        # latency_memory_access
        ctypes.c_uint32,        # seed
        ctypes.c_uint32,        # numRequests
        ctypes.POINTER(Request) # requests
    ]
    lib.run_simulation.restype = Result

    # 读取csv
    if len(sys.argv) < 2:
        print("Usage: python3 run_smu_tests.py universal_test.csv")
        return
    csv_filename = sys.argv[1]
    requests = load_requests_from_csv(csv_filename)
    num = len(requests)
    RequestArray = Request * num
    arr = RequestArray(*requests)

    # 运行测试（参数可根据你自己的仿真配置灵活设定）
    result = lib.run_simulation(
        10000,                         # max_cycles
        b"tracefile.txt",              # tracefile
        0,                             # endianness (0小端, 1大端)
        1, 1, 1,                       # latency_scrambling, latency_encryption, latency_memory_access
        12345,                         # seed
        num,
        arr
    )

    print("\n--- Python测得仿真结果 ---")
    print(f"Cycles: {result.cycles}")
    print(f"Errors: {result.errors}")

if __name__ == '__main__':
    main()