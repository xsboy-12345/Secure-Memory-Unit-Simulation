1. LCG-based PRNG
how to choose A,B,M in the formular RandSeed = (A * RandSeed + B) % M
The period of PRNG is less than or equal to M 
requirements to make the period maximum:
 1.1 B and M are relatively prime
 1.2 All prime factors of M are divisible by A-1;
 1.3 If M is a multiple of 4, so is A-1;
 1.4 A, B, N[0] ARE SMALLER THAN M
 



1、加密模块的设计思路采用假设一直八位密钥和待加密数据，分字节按位进行xor操作，最后拼接
（按位进行xor是由systemC的“^”操作是按照按位XOR设计的 ）
2、地址加扰模块的设计思路是基于一个公式（scrambled_addr = logic_addr ^ scramble_key）
3、奇偶校验模块的实际思路是将32位地址按位xor最后得到一个奇偶校验位输出
xor_reduce()方法来自GPT，xor_reduce() 是 SystemC 提供的内建方法，用于 sc_bv<>、sc_lv<>、sc_uint<>、sc_int<> 等类型的按位归约操作。