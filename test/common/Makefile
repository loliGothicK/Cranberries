# Makefile for test

<<<<<<< HEAD
TARGET 	= test.out
SRCS 	= ./test.cpp
=======
TARGET1 	= test.out
SRCS1 	= ./test.cpp
>>>>>>> common

# 基本コマンド
RM 		:= rm
CXX 	:= g++
CC 		:= gcc

# デバッグ時とリリース時の微調整
CXX_DEBUG_FLAGS		=	-g -O0 -Wall -Wextra
CXX_RELEASE_FLAGS	=	-O2 -Wall -Wextra

# 基本オプション
CPPFLAGS = -std=c++14

# make
# debug
.PHONY: Debug
Debug: CXXFLAGS+=$(CXX_DEBUG_FLAGS)
Debug: all
# release
.PHONY: Release
Release: CXXFLAGS+=$(CXX_RELEASE_FLAGS)
Release: all

all: $(TARGET)
$(TARGET): $(SRCS)
<<<<<<< HEAD
	touch $(SRCS) && $(CXX) $^ -o $@ $(CXXFLAGS) $(CPPFLAGS) && ./$(TARGET)
=======
	$(CXX) $^ -o $@ $(CXXFLAGS) $(CPPFLAGS) && ./$(TARGET)
>>>>>>> common

# make clean
.PHONY: clean
clean:
	$(RM) -f *.out
