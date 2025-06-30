#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <sys/stat.h>
#include <stdint.h>


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

extern struct Result run_simulation(uint32_t cycles,
                                    const char* tracefile,
                                    uint8_t endianness,
                                    uint32_t latencyScrambling,
                                    uint32_t latencyEncrypt,
                                    uint32_t latencyMemoryAccess,
                                    uint32_t seed,
                                    uint32_t numRequests,
                                    struct Request* requests);

const char* usage_msg =
        "Usage: %s [options] <file>\n   "
        "Options:\n"
        "   -c,--cycles N: uint32_t        Number of cycles (default: 1)\n"
        "   -t,--tf : String               Path to the TraceFile\n"
        "   -h,--help: flag                Show the help message\n";

const char* help_msg =
        "Positional arguments:\n"
        "  <file>  The path to input file: list of requests\n"
        "\n"
        "Optional arguments:\n"
        "  --cycles: uint32_t   The number of cycles (default: 1)\n"
        "  --tf: String          Path to the TraceFile\n"
        "  --help: flag          Show the help message\n";

void print_usage(const char* progname) {
    fprintf(stderr, usage_msg, progname);
}

void print_help(const char* progname) {
    printf(usage_msg, progname);
    printf("\n%s", help_msg);
}

int parse_cli(int argc, char *argv[], uint32_t *cycles, char **tracefile, char **input_file) {
    static struct option long_options[] = {
            {"cycles", required_argument, 0, 'c'},
            {"tf", required_argument, 0, 't'},
            {"help", no_argument, 0, 'h'},
            {0, 0, 0, 0}
    };
    int opt;
    while ((opt = getopt_long(argc,argv,"c:t:h", long_options, NULL)) != -1) {
        switch (opt) {
            case 'c': *cycles = (uint32_t)strtoul(optarg, NULL, 10); break;
            case 't': *tracefile = optarg; break;
            case 'h': print_help(argv[0]); return EXIT_SUCCESS;
            default: print_usage(argv[0]); return EXIT_FAILURE;
        }
    }
    if (optind >= argc) {
        fprintf(stderr, "No input file specified.\n");
        return 1;
    }
    while (optind < argc) {
        if (*input_file != NULL) {
            fprintf(stderr, "Too many input files specified.\n");
            return 1;
        }
        *input_file = argv[optind++];
    }
    return EXIT_SUCCESS;
}

FILE* validate_and_open_read(const char *path) {
    FILE* file;
    if (!(file = fopen(path, "r"))) return NULL;

    struct stat statbuf;
    if (fstat(fileno(file), &statbuf)) { fclose(file); return NULL; }
    if (!S_ISREG(statbuf.st_mode) || statbuf.st_size <= 0) {
        fclose(file);
        return NULL;
    }
    return file;
}

#define MAX_REQUESTS 1024

int parse_csv(const char* input, struct Request* requests, uint32_t* num_reqs) {
    char* line = strtok((char*)input, "\n");
    *num_reqs = 0;
    if (!line) return -1; // skip header
    line = strtok(NULL, "\n");
    while (line && *num_reqs < MAX_REQUESTS) {
        struct Request r = {0};
        char type;
        char addr[64], data[64], fault[64], bit[64];
        int n = sscanf(line, "%c,%[^,],%[^,],%[^,],%[^\n]", &type, addr, data, fault, bit);
        if (type == 'W') {
            r.w = 1;
            r.addr = strtoul(addr, NULL, 0);
            r.data = strtoul(data, NULL, 0);
        } else if (type == 'R') {
            r.r = 1;
            r.addr = strtoul(addr, NULL, 0);
        } else if (type == 'F') {
            r.fault = strtoul(fault, NULL, 0);
            r.faultBit = (uint8_t)strtoul(bit, NULL, 0);
        }
        requests[(*num_reqs)++] = r;
        line = strtok(NULL, "\n");
    }
    return 0;
}

int main(int argc, char** argv){
    static uint32_t cycles = 1;
    static char *tracefile = NULL;
    static char *input_file = NULL;

    if (parse_cli(argc, argv, &cycles , &tracefile, &input_file) != 0 || input_file == NULL) return 1;

    FILE* file = validate_and_open_read(input_file);
    if (!file) {
        fprintf(stderr, "Failed to open input file '%s'.\n", input_file);
        return 1;
    }
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* input = (char*)malloc(file_size + 1);
    if (!input) {
        fprintf(stderr, "Failed to allocate memory for input file.\n");
        fclose(file);
        return 1;
    }
    fread(input, 1, file_size, file);
    fclose(file);
    input[file_size] = '\0';

    struct Request requests[MAX_REQUESTS];
    uint32_t num_requests = 0;
    if (parse_csv(input, requests, &num_requests) != 0) {
        fprintf(stderr, "Failed to parse CSV requests.\n");
        free(input);
        return 1;
    }

    free(input);

    struct Result res = run_simulation(cycles, tracefile, 0, 10, 10, 20, 1234, num_requests, requests);
    printf("Simulation finished. Cycles: %u, Errors: %u\n", res.cycles, res.errors);
    return 0;
}