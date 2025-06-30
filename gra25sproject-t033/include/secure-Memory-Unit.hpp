//TODO
#ifndef SECURE_MEMORY_UNIT_HPP
#define SECURE_MEMORY_UNIT_HPP

#include <systemc>
#include <cstdint>
#include <array>
#include <map>
#include <random>

using namespace sc_core;
using namespace sc_dt;

SC_MODULE(SecureMemoryUnit) {
        sc_in<bool> clk;

        sc_in<uint32_t> addr;
        sc_in<uint32_t> wdata;
        sc_in<bool> r;
        sc_in<bool> w;
        sc_in<uint32_t> fault;
        sc_in<sc_bv<4>> faultBit;

        sc_out<uint32_t> rdata;
        sc_out<bool> ready;
        sc_out<bool> error;

        uint8_t endianness;              // 0 = Little Endian, 1 = Big Endian
        uint32_t latency_scrambling;
        uint32_t latency_encrypt;
        uint32_t latency_memory;
        uint32_t seed;


        std::map<uint32_t, uint8_t> memory;
        std::map<uint32_t, bool> parity_bits;

        uint8_t encryption_key[4];       // 每个 byte 一个 key
        uint32_t scrambling_key;

        std::mt19937 rng;

        SC_HAS_PROCESS(SecureMemoryUnit);
        SecureMemoryUnit(sc_module_name name,
        uint8_t endianness_,
        uint32_t latency_scrambling_,
        uint32_t latency_encrypt_,
        uint32_t latency_memory_,
        uint32_t seed_)
        : sc_module(name),
        endianness(endianness_),
        latency_scrambling(latency_scrambling_),
        latency_encrypt(latency_encrypt_),
        latency_memory(latency_memory_),
        seed(seed_)
        {
            SC_THREAD(process);
            sensitive << clk.pos();

            rng.seed(seed_);
            init_keys();
        }

        void process() {
            while (true) {
                wait();  // 等待时钟上升沿

                ready.write(false);
                if (r.read() || w.read()) {
                    // 注入 Fault
                    handle_fault_injection();

                    // 加密/地址转换延迟
                    wait(std::max(latency_scrambling, latency_encrypt), SC_NS);

                    // 执行读或写
                    if (w.read()) {
                        write_access();
                    } else if (r.read()) {
                        read_access();
                    }

                    wait(latency_memory, SC_NS);

                    ready.write(true);
                } else {
                    // 即使不访问，也需支持 fault injection
                    handle_fault_injection();
                }
            }
        }

        void write_access() {
            uint32_t value = wdata.read();
            uint8_t bytes[4];
            split_bytes(value, bytes);

            for (int i = 0; i < 4; ++i) {
                bytes[i] ^= encryption_key[i];  // 加密
                uint32_t sub_addr = scramble_address(addr.read() + i);
                memory[sub_addr] = bytes[i];
                parity_bits[sub_addr] = compute_parity(bytes[i]);
            }

            error.write(false);
        }

        // ==== 读访问 ====
        void read_access() {
            uint8_t bytes[4];
            bool parity_error = false;

            for (int i = 0; i < 4; ++i) {
                uint32_t sub_addr = scramble_address(addr.read() + i);
                bytes[i] = memory[sub_addr];
                if (compute_parity(bytes[i]) != parity_bits[sub_addr]) {
                    parity_error = true;
                }
                bytes[i] ^= encryption_key[i];  // 解密
            }

            if (parity_error) {
                error.write(true);
                rdata.write(0);
            } else {
                error.write(false);
                rdata.write(combine_bytes(bytes));
            }
        }

        uint32_t scramble_address(uint32_t a) {
            return a ^ scrambling_key;
        }

        bool compute_parity(uint8_t b) {
            int ones = __builtin_popcount(b);
            return ones % 2 != 0;
        }

        void handle_fault_injection() {
            uint32_t fault_addr = fault.read();
            if (fault_addr == UINT32_MAX) return;

            uint32_t phys_addr = scramble_address(fault_addr);
            uint8_t bit = faultBit.read().to_uint();

            if (bit < 8) {
                memory[phys_addr] ^= (1 << bit);
            } else if (bit == 8) {
                parity_bits[phys_addr] = !parity_bits[phys_addr];
            }
        }

        void init_keys() {
            for (int i = 0; i < 4; ++i)
                encryption_key[i] = rng() % 256;
            scrambling_key = rng();
        }

        void split_bytes(uint32_t val, uint8_t* bytes) {
            if (endianness == 0) { // Little Endian
                for (int i = 0; i < 4; ++i)
                    bytes[i] = (val >> (i * 8)) & 0xFF;
            } else {
                for (int i = 0; i < 4; ++i)
                    bytes[i] = (val >> ((3 - i) * 8)) & 0xFF;
            }
        }

        uint32_t combine_bytes(uint8_t* bytes) {
            uint32_t result = 0;
            if (endianness == 0) {
                for (int i = 0; i < 4; ++i)
                    result |= (bytes[i] << (i * 8));
            } else {
                for (int i = 0; i < 4; ++i)
                    result |= (bytes[i] << ((3 - i) * 8));
            }
            return result;
        }

        // 额外方法（如果需要手动设置密钥）
        void setScramblingKey(uint32_t key) { scrambling_key = key; }
        void setEncryptionKey(uint32_t key) {
            for (int i = 0; i < 4; ++i)
                encryption_key[i] = (key >> (i * 8)) & 0xFF;
        }

        uint8_t getByteAt(uint32_t phys_addr) {
            return memory[phys_addr];
        }
};

#endif // SECURE_MEMORY_UNIT_HPP