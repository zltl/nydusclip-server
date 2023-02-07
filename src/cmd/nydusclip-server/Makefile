.PHONY: all clean

CURR_DIR=$(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
# $(sub_name) is the folder's name that have a Makefile including this file.
# For examploe: .../path/to/mylibx --> sub_name=mylibx
sub_name = $(shell basename $(CURR_DIR))

OBJ_DIR:=$(R_TARGET_DIR)/cmd/$(sub_name)
$(shell mkdir -p $(OBJ_DIR))

sources_c = $(wildcard *.cc)
objs_cc = $(patsubst %.cc,$(OBJ_DIR)/%.cc.o,$(sources_c))

mylibs = $(wildcard $(R_TARGET_DIR)/*.a)

mlibs = -llog \
	-lstatus \
	-lspdlog \
	-loption \
	-lboost_program_options \
	-lco_event \
	-levent \
	-levent_core \
	-levent_extra \
	-levent_openssl \
	-levent_pthreads

all: $(R_TARGET_DIR)/$(sub_name)

$(R_TARGET_DIR)/$(sub_name): $(objs_cc) $(mylibs)
	$(CXX) $(MY_CXXFLAGS) $(EXTRA_LIB_FLAG) $^ -o $@ $(mlibs)

$(OBJ_DIR)/%.cc.o: $(CURR_DIR)/%.cc
	$(CXX) $(MY_CXXFLAGS) $(EXTRA_INCLUDE_FLAG) $(MODECFLAGS) -c $< -o $@

clean:
	rm -rf $(R_TARGET_DIR)/$(sub_name)
	rm -rf $(OBJ_DIR)

