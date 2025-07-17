# ---------------------------------------
# CONFIGURATION BEGIN
# ---------------------------------------

CPP_SRCS = read_write_tests.cpp simulation_tests.cpp
CPP_OBJS = $(CPP_SRCS:.cpp=.o)

TARGET := read_write_tests

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

all: debug

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

debug: CXXFLAGS += -g
debug: $(TARGET)

release: CXXFLAGS += -O2
release: $(TARGET)

$(TARGET): $(CPP_OBJS)
	$(CXX) $(CXXFLAGS) $(CPP_OBJS) $(LDFLAGS) -o $@

clean:
	rm -f $(TARGET) $(CPP_OBJS)

.PHONY: all debug release clean