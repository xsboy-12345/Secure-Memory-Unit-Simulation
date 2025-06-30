#include "secure-Memory-Unit.hpp"
#include <systemc>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;
using namespace sc_core;

struct Request {
    uint32_t addr;
    uint32_t data;
    uint8_t r;
    uint8_t w;
    uint32_t fault;
    uint8_t faultBit;
};

struct Result {
    uint32_t cycles;
    uint32_t errors;
};

SC_MODULE(SimulationWrapper) {
        // 信号
        sc_signal<uint32_t> addr, wdata, fault;
        sc_signal<bool> r, w;
        sc_signal<sc_bv<4>> faultBit;
        sc_signal<uint32_t> rdata;
        sc_signal<bool> ready, error;
        sc_clock clk;

        SecureMemoryUnit* smu;

        const vector<Request>& requests;
        uint32_t error_count = 0;

        SC_HAS_PROCESS(SimulationWrapper);
        SimulationWrapper(sc_module_name name,
        const vector<Request>& reqs,
        uint8_t endianness,
        uint32_t lat_scr, uint32_t lat_enc, uint32_t lat_mem, uint32_t seed)
        : clk("clk", 10, SC_NS), requests(reqs) {
            smu = new SecureMemoryUnit("SMU", endianness, lat_scr, lat_enc, lat_mem, seed);

            smu->clk(clk);
            smu->addr(addr);
            smu->wdata(wdata);
            smu->r(r);
            smu->w(w);
            smu->fault(fault);
            smu->faultBit(faultBit);
            smu->rdata(rdata);
            smu->ready(ready);
            smu->error(error);

            SC_THREAD(driver);
        }

        void driver() {
            fault.write(UINT32_MAX);
            r.write(false);
            w.write(false);

            for (auto& req : requests) {
                addr.write(req.addr);
                wdata.write(req.data);
                r.write(req.r);
                w.write(req.w);
                fault.write(req.fault);
                faultBit.write(req.faultBit);

                wait(clk.posedge_event());
                r.write(false);
                w.write(false);
                fault.write(UINT32_MAX);

                while (!ready.read()) wait(clk.posedge_event());

                if (req.r && error.read()) {
                    error_count++;
                }
            }

            sc_stop();
        }
};

Result run_simulation(uint32_t cycles,
                      const char* tracefile,
                      uint8_t endianness,
                      uint32_t latencyScrambling,
                      uint32_t latencyEncrypt,
                      uint32_t latencyMemoryAccess,
                      uint32_t seed,
                      uint32_t numRequests,
                      Request* requests) {
    vector<Request> reqs(requests, requests + numRequests);
    SimulationWrapper sim("sim", reqs, endianness, latencyScrambling,
                          latencyEncrypt, latencyMemoryAccess, seed);
    sc_start();
    Result result = {sc_time_stamp().value() / 10, sim.error_count}; // 每个周期10ns
    return result;
}