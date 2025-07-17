# ---------------------------------------
# CONFIGURATION BEGIN
# ---------------------------------------

# Entry point source files
C_SRCS = src/main.c
CPP_SRCS = src/simulation.cpp

# Object files
C_OBJS = $(C_SRCS:.c=.o)
CPP_OBJS = $(CPP_SRCS:.cpp=.o)

# Output binary name
TARGET := projekt

# Path to SystemC installation
SCPATH := $(HOME)/systemc-2.3.3-install

# Compiler
CXX := clang++
CC := clang

# Compiler flags
CXXFLAGS := -std=c++11 -I$(SCPATH)/include

# Linker flags
LDFLAGS := -L$(SCPATH)/lib -lsystemc -lm

# macOS 下自动添加 rpath
ifeq ($(shell uname), Darwin)
    LDFLAGS += -Wl,-rpath,$(SCPATH)/lib
endif

# ---------------------------------------
# CONFIGURATION END
# ---------------------------------------

# 默认目标
all: debug

# C 代码编译
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# C++ 代码编译
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Debug 版本
debug: CXXFLAGS += -g
debug: $(TARGET)

# Release 版本
release: CXXFLAGS += -O2
release: $(TARGET)

# 链接
$(TARGET): $(C_OBJS) $(CPP_OBJS)
	$(CXX) $(CXXFLAGS) $(C_OBJS) $(CPP_OBJS) $(LDFLAGS) -o $(TARGET)

# 清理
clean:
	rm -f $(TARGET)
	rm -f $(C_OBJS) $(CPP_OBJS)

.PHONY: all debug release clean

