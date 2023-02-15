.PHONY: all run_test clean

.ONESHELL:

CURR_DIR=$(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
OBJ_DIR:=$(R_TARGET_DIR)/tests
$(shell mkdir -p $(OBJ_DIR))

sources_c = $(wildcard $(CURR_DIR)/*.cc)
objs_cc = $(patsubst %.cc,$(OBJ_DIR)/%.cc.o,$(sources_c))

link_libs = -lgtest \
	-lgmock  \
	-lgtest_main \
	-lgmock_main \
	-llog \
	-lstatus \
	-lspdlog \
	-loption \
	-lboost_program_options \
	-lasan

O:=$(OBJ_DIR)

all: $(OBJ_DIR)/nydus_unit_test

run_test:
	for r in $$(ls $(R_TARGET_DIR)/tests/); do
		fullr=$(R_TARGET_DIR)/tests/$$r
		if [ -x $$fullr ]; then
			$$fullr
		fi
	done

$(OBJ_DIR)/nydus_unit_test: $(sources_c)
	$(CXX) $(MY_CXXFLAGS) $(MY_TEST_FLAGS) $(EXTRA_INCLUDE_FLAG) $(EXTRA_LIB_FLAG) $^ -o $@ $(link_libs)

clean:
	rm -rf $(OBJ_DIR)

