#ifndef SECURE_MEMORY_UNIT_SIMULATION_ENCRYPTOR_HPP
#define SECURE_MEMORY_UNIT_SIMULATION_ENCRYPTOR_HPP
#include <systemc.h>
SC_MODULE(Encryptor) {
        sc_in<sc_uint<32>> data_in;
        sc_in<sc_uint<8>> key;
        sc_out<sc_uint<32>> data_out;

        void encrypt_process() {
            sc_uint<32> input = data_in.read();//取输入数据
            sc_uint<8> k = key.read();//取八位密钥
            //按照字节进行异或操作
            sc_uint<8> byte0 = input.range(7, 0)   ^ k;
            sc_uint<8> byte1 = input.range(15, 8)  ^ k;
            sc_uint<8> byte2 = input.range(23, 16) ^ k;
            sc_uint<8> byte3 = input.range(31, 24) ^ k;
            //数据拼接
            sc_uint<32> result;
            result.range(7, 0)    = byte0;
            result.range(15, 8)   = byte1;
            result.range(23, 16)  = byte2;
            result.range(31, 24)  = byte3;
            data_out.write(result);//输出结果
        }

        SC_CTOR(Encryptor) {
            SC_METHOD(encrypt_process);
            sensitive << data_in << key;
        }
};

#endif //SECURE_MEMORY_UNIT_SIMULATION_ENCRYPTOR_HPP
