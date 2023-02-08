.PHONY: deps clean src build_test build_benchmark \
	test build run_test benchmark run_benchmark

# Use one shell in targets. Multiple lines of commands in a target run
# in one shell, so we can use the result of previous commands.
.ONESHELL:

# gcc only, maybe support clang later.
# CC=gcc
# CXX=g++

# The path of project.
PROJECT_ROOT:=$(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
# the build target, all compile result be put into this folder.
TARGET_DIR ?= $(PROJECT_ROOT)/target

# If in debug mode, build all target to ./target/debug/...,
# else build to ./target/release/...
ifneq ($(RELEASE),)
	R_TARGET_DIR = $(TARGET_DIR)/release
	C_COMMON_FLAGS = -O3
else
	DEBUG_FLAGS = -g
	R_TARGET_DIR = $(TARGET_DIR)/debug
endif

# install dependencies using prefix $(DEPS_INSTALL_DIR)
DEPS_INSTALL_DIR = $(R_TARGET_DIR)/deps

# add deps install location, and ./src/ into include path.
EXTRA_INCLUDE_FLAG=-I$(DEPS_INSTALL_DIR)/include -I$(PROJECT_ROOT)/src
# add deps install location into link path.
EXTRA_LIB_FLAG=-L$(DEPS_INSTALL_DIR)/lib -L$(DEPS_INSTALL_DIR)/lib64 -L$(R_TARGET_DIR)

# create location, avoid "no such file" error.
$(shell mkdir -p $(R_TARGET_DIR))
$(shell mkdir -p $(R_TARGET_DIR)/deps/{include,lib})
$(shell mkdir -p $(R_TARGET_DIR)/obj)
$(shell mkdir -p $(R_TARGET_DIR)/pcm)

# add sanitize flags if defined environment variable SANITIZE!=no
ifneq ($(SANITIZE),no)
	ifeq ($(DEBUG_FLAGS),)
		DEBUG_FLAGS += -g
	endif
	DEBUG_FLAGS += -fsanitize=address
	ifneq ($(CC),clang)
		DEBUG_FLAGS += -lasan
	endif
endif

# use C++23
USE_CXX_VERSION=2b

# strict mode, for C and C++
MY_C_COMMON_FLAGS += -Werror -Wall -Wextra -pedantic -ffile-prefix-map=$(PROJECT_ROOT)/src/=
MY_TEST_FLAGS = -ffile-prefix-map=$(PROJECT_ROOT)/=
# use c17, just for C
MY_C_STANDARD := -std=c17
# use C++23, just for C++
MY_CXX_STANDARD := -std=c++${USE_CXX_VERSION}

ifeq ($(CC),clang)
	MY_C_COMMON_FLAGS += $(shell llvm-config --cxxflags)
endif

# flags pass to CC only
MY_CFLAGS:= $(MY_C_COMMON_FLAGS) $(DEBUG_FLAGS) $(MY_C_STANDARD)
# flags pass to CXX only
# locate pcm files into target/{debug/release}/pcm
MY_CXXFLAGS:= $(MY_C_COMMON_FLAGS) $(DEBUG_FLAGS) $(MY_CXX_STANDARD) \
	 -fmodules-ts '-fmodule-mapper=|@g++-mapper-server -r'$(R_TARGET_DIR)/pcm

# export all variables that defined
export

# alias for deps.sh script.
DEPS_GET:= $(PROJECT_ROOT)/deps/deps.sh

all: src benchmarks test
src: deps
	$(MAKE) -C src
test: run_test

build_test: src
	$(MAKE) -C tests
run_test: build_test
	$(MAKE) run_test -C tests
build_benchmark: src
	$(MAKE) -C benchmarks
benchmark: run_banchmark
run_benchmark: build_benchmark
	$(MAKE) run_benchmarks -C benchmarks

# download and install dependencies defined in ./deps/deps-xxxxx.sh
deps:
	$(DEPS_GET) googletest-1.12.1
	$(DEPS_GET) benchmark-1.7.1
	$(DEPS_GET) boost-1.18.0.rc1
	$(DEPS_GET) openssl-3.0.7
	# $(DEPS_GET) fmt-9.1.0
	$(DEPS_GET) spdlog-1.11.0
	$(DEPS_GET) double-conversion-3.2.1
	$(DEPS_GET) libevent-2.1.12
	$(DEPS_GET) leveldb-1.23

clean:
	$(MAKE) clean -C tests
	$(MAKE) clean -C benchmarks
	$(MAKE) clean -C src
	rm -rf $(R_TARGET_DIR)/pcm

