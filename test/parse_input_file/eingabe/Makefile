# ---------------------------------------
# CONFIGURATION BEGIN
# ---------------------------------------

# Entry point and target name
MAIN := module.cpp
TARGET := module

# Use system clang++ on macOS (or change to g++ if preferred)
CXX := clang++

# SystemC installation path
SYSTEMC := /opt/systemc233

# Include and link flags
CXXFLAGS := -std=c++11 -I$(SYSTEMC)/include
LDFLAGS := -L$(SYSTEMC)/lib-macosx64 -lsystemc

# ---------------------------------------
# CONFIGURATION END
# ---------------------------------------

# Default target
all: debug

debug: CXXFLAGS += -g
debug: $(TARGET)

release: CXXFLAGS += -O2
release: $(TARGET)

$(TARGET): $(MAIN)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f $(TARGET)

.PHONY: all debug release clean
