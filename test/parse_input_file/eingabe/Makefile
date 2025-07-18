# ---------------------------------------
# CONFIGURATION BEGIN
# ---------------------------------------

MAIN_C := module.c
TARGET := module

# 编译器
CC := clang
CXX := clang++

SYSTEMC := /opt/systemc233

CFLAGS := -I$(SYSTEMC)/include
CXXFLAGS := -std=c++11 -I$(SYSTEMC)/include
LDFLAGS := -L$(SYSTEMC)/lib-macosx64 -lsystemc -Wl,-rpath,$(SYSTEMC)/lib-macosx64

# ---------------------------------------
# CONFIGURATION END
# ---------------------------------------

OBJ_C := $(MAIN_C:.c=.o)
OBJ_CPP := $(MAIN_CPP:.cpp=.o)

all: debug

debug: CFLAGS += -g
debug: CXXFLAGS += -g
debug: $(TARGET)

release: CFLAGS += -O2
release: CXXFLAGS += -O2
release: $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJ_C) $(OBJ_CPP)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f $(TARGET) $(OBJ_C) $(OBJ_CPP)

.PHONY: all debug release clean
