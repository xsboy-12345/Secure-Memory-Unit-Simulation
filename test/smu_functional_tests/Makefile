# ---------------------------------------
# CONFIGURATION BEGIN
# ---------------------------------------

TEST_SRCS = read_write_tests.cpp fault_injection_tests.cpp simulation_tests.cpp
TEST_BINS = $(TEST_SRCS:.cpp=)

SCPATH := $(HOME)/systemc-2.3.3-install
CXX := clang++
CXXFLAGS := -std=c++11 -Iinclude -I$(SCPATH)/include
LDFLAGS := -L$(SCPATH)/lib -lsystemc -lm

ifeq ($(shell uname), Darwin)
    LDFLAGS += -Wl,-rpath,$(SCPATH)/lib
endif

# ---------------------------------------
# CONFIGURATION END
# ---------------------------------------

all: $(TEST_BINS)

# pattern rule: %.cpp -> %
%: %.cpp simulation_tests.cpp
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $@

clean:
	rm -f $(TEST_BINS) *.o

.PHONY: all clean