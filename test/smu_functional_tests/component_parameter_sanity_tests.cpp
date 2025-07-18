#include <iostream>
#include "include/SecureMemoryUnit_debug.hpp"

// Test for invalid latency parameter
void test_invalid_latency() {
    Request reqs[1];
    reqs[0].addr = 0x20;
    reqs[0].data = 0xDEADBEEF;
    reqs[0].r = 0;
    reqs[0].w = 1;
    reqs[0].fault = UINT32_MAX;
    reqs[0].faultBit = 0;

    uint32_t invalid_latency = 0xFFFFFFFF;

    std::cout << "\n--- Test with extremely large latency parameter ---" << std::endl;
    Result result = run_simulation(
            10000,            // max_cycles
            "invalid_latency_trace", // tracefile
            0,                // endianness
            invalid_latency,  // latency_scrambling (invalid)
            1,                // latency_encryption
            1,                // latency_memory_access
            1,                // seed
            1,                // numRequests
            reqs
    );
    std::cout << "Cycles: " << result.cycles << std::endl;
    std::cout << "Errors: " << result.errors << std::endl;
}



int main() {
    test_invalid_latency();
    return 0;
}