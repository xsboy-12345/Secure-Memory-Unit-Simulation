#include <systemc>

#include "include/SecureMemoryUnit_debug.hpp"


struct Result run_simulation(
    uint32_t max_cycles,
    const char *tracefile,
    uint8_t endianness,
    uint32_t latency_scrambling,
    uint32_t latency_encryption,
    uint32_t latency_memory_access,
    uint32_t seed,
    uint32_t numRequests,
    struct Request *requests) {
    sc_clock clk("clk", 1, SC_NS);
    sc_signal<uint32_t> addr, wdata, fault;
    sc_signal<bool>     r, w;
    sc_signal<sc_dt::sc_bv<4>> faultbit;
    sc_signal<uint32_t> rdata;
    sc_signal<bool>     ready, error;
    SecureMemoryUnit smu("smu", endianness, latency_scrambling, latency_encryption, latency_memory_access, seed);

    if(tracefile != NULL) {
        sc_trace_file *tf = sc_create_vcd_trace_file(tracefile);
        sc_trace(tf, clk, "clk");
        sc_trace(tf, addr, "addr");
        sc_trace(tf, wdata, "wdata");
        sc_trace(tf, r, "r");
        sc_trace(tf, w, "w");
        sc_trace(tf, fault, "fault");
        sc_trace(tf, faultbit, "faultbit");
        sc_trace(tf, ready, "ready");
        sc_trace(tf, error, "error");
        sc_trace(tf, rdata, "rdata");
        }

    smu.clk(clk); 
    smu.addr(addr); 
    smu.wdata(wdata);
    smu.r(r); 
    smu.w(w);
    smu.fault(fault);
    smu.faultbit(faultbit);
    smu.rdata(rdata); 
    smu.ready(ready); 
    smu.error(error);

    fault.write(UINT32_MAX);
    faultbit.write("0000");
    addr.write(0);
    wdata.write(0);
    r.write(false);
    w.write(false);
    sc_start(1, SC_NS); 
    uint32_t err_count = 0;
    uint32_t cyc_count = 0;
    //for debugging
    // std::cout << "seed: " << smu.seed << std::endl;
    // std::cout << "encrypt key: " << smu.encryptor_key << std::endl;
    // std::cout << "scramble key: " << smu.scrambler_key << std::endl;
    for (uint32_t i = 0; i < numRequests && cyc_count < max_cycles; ++i){
        cout << "Processing request " << i + 1 << " of " << numRequests << std::endl;
        cout << "Request details: "<< requests[i].r<< " "<< requests[i].w << " " << requests[i].addr << " " << requests[i].data << std::endl;

        while (!ready.read() && cyc_count < max_cycles) {
            sc_start(1, SC_NS);
            ++cyc_count;
        }
    
        if (requests[i].fault != UINT32_MAX) {
            fault.write(requests[i].fault);
            faultbit.write(requests[i].faultBit & 0xF);
        } else {
            fault.write(UINT32_MAX);
        } 
       if (requests[i].r) {
            r.write(true);  
            w.write(false);
            addr.write(requests[i].addr);
        } else {                              
            w.write(true);  
            r.write(false);
            wdata.write(requests[i].data);
            addr.write(requests[i].addr);
        }
        sc_start(1, SC_NS); 
        cyc_count++;
        while (!ready.read() && cyc_count < max_cycles){
            sc_start(1, SC_NS);
            cyc_count++;
        }
        

        if (error.read()){
            err_count++;
            cout << "!! Error detected in request " << i + 1 << std::endl;
        }
        cout << "Cumulative errors after request " << i + 1 << ": " << err_count << std::endl;
        //for debugging
        if(w.read()){
            cout << "Write to address: " <<  addr.read() 
                 << " Data: " << wdata.read() << std::endl;
        }
        if(r.read()){
            cout << "Read from address: " << addr.read() 
                 << " Data: "<< rdata.read() << std::endl;
        }

        r.write(false); 
        w.write(false);
        addr.write(0);
        fault.write(UINT32_MAX);
        faultbit.write("0000");
        wdata.write(0);
        // sc_start(1, SC_NS);
        // cyc_count++;
    }


    struct Result res = {
         cyc_count,
         err_count
    };
    return res;

}

int sc_main(int argc, char* argv[])
{
    std::cout << "ERROR" << std::endl;
    return 1;
}