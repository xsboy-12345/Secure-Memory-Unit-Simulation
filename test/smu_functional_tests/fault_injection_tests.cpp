#include <iostream>
#include "include/SecureMemoryUnit_debug.hpp"

int main() {
    Request reqs[12];

    reqs[0].addr = 0x20;
    reqs[0].data = 0xDEADBEEF;
    reqs[0].r = 0;
    reqs[0].w = 1;
    reqs[0].fault = UINT32_MAX;
    reqs[0].faultBit = 0;

    reqs[1].addr = 0x20;
    reqs[1].data = 0;
    reqs[1].r = 1;
    reqs[1].w = 0;
    reqs[1].fault = UINT32_MAX;
    reqs[1].faultBit = 0;

    reqs[2].addr = 0;
    reqs[2].data = 0;
    reqs[2].r = 0;
    reqs[2].w = 0;
    reqs[2].fault = 0x5DE4A452;
    reqs[2].faultBit = 2;

    reqs[3].addr = 0x20;
    reqs[3].data = 0;
    reqs[3].r = 1;
    reqs[3].w = 0;
    reqs[3].fault = UINT32_MAX;
    reqs[3].faultBit = 0;

    reqs[4].addr = 0x30;
    reqs[4].data = 0x12345678;
    reqs[4].r = 0;
    reqs[4].w = 1;
    reqs[4].fault = 0x5DE4A441;
    reqs[4].faultBit = 3;

    reqs[5].addr = 0x30;
    reqs[5].data = 0;
    reqs[5].r = 1;
    reqs[5].w = 0;
    reqs[5].fault = UINT32_MAX;
    reqs[5].faultBit = 0;

    reqs[6].addr = 0x40;
    reqs[6].data = 0x23456789;
    reqs[6].r = 0;
    reqs[6].w = 1;
    reqs[6].fault = UINT32_MAX;
    reqs[6].faultBit = 0;

    reqs[7].addr = 0x40;
    reqs[7].data = 0;
    reqs[7].r = 1;
    reqs[7].w = 0;
    reqs[7].fault = 0x5DE4A432;
    reqs[7].faultBit = 2;

    reqs[8].addr = 0x50;
    reqs[8].data = 0x34567890;
    reqs[8].r = 0;
    reqs[8].w = 1;
    reqs[8].fault = UINT32_MAX;
    reqs[8].faultBit = 0;

    reqs[9].addr = 0x50;
    reqs[9].data = 0;
    reqs[9].r = 1;
    reqs[9].w = 0;
    reqs[9].fault = UINT32_MAX;
    reqs[9].faultBit = 0;

    reqs[10].addr = 0;
    reqs[10].data = 0;
    reqs[10].r = 0;
    reqs[10].w = 0;
    reqs[10].fault = 0x5DE4A423;
    reqs[10].faultBit = 8;

    reqs[11].addr = 0x50;
    reqs[11].data = 0;
    reqs[11].r = 1;
    reqs[11].w = 0;
    reqs[11].fault = UINT32_MAX;
    reqs[11].faultBit = 0;

    uint32_t test_key = 0xCAFEBABE;
    Result result = run_simulation(
            10000,           // max_cycles
            "fault_trace",  // tracefile
            0,               // endianness
            1,               // latency_scrambling
            1,               // latency_encryption
            1,               // latency_memory_access
            test_key,        // seed
            12,              // numRequests
            reqs
    );

    std::cout << "== Fault Injection Mixed Test Done ==" << std::endl;
    std::cout << "Cycles: " << result.cycles << std::endl;
    std::cout << "Errors: " << result.errors << std::endl;
    return 0;
}