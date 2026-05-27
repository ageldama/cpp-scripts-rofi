# -*- mode: makefile-gmake; -*-

TARGET=scripts-rofi
SRCS   = src/main.cpp src/argp.cpp src/tildeexpand.cpp src/db.cpp src/file_find.cpp src/rofi.cpp src/rofi_ask_yn.cpp src/rofi_sel_list.cpp src/str.cpp src/safe_regex.cpp src/exec.cpp src/main_aux.cpp src/str_tokenize_cmdline.cpp src/main2.cpp
OBJS   = $(SRCS:.cpp=.o)


# ----------------------------------------------

DEBUG?=1

CXXFLAGS  += -std=c++20 -Wall -Werror -Wpedantic

ifeq ($(DEBUG), 1)
	CXXFLAGS  += -g -O0
	CXXFLAGS  += -fstack-protector
	CXXFLAGS  += -fsanitize=address,undefined

	LDFLAGS += -fsanitize=address,undefined
else
	CXXFLAGS  += -O3
endif

CXXFLAGS  += -Iinclude

CLANG_FORMAT ?= clang-format
CLANG_TIDY ?= clang-tidy
BEAR ?= bear
PARALLEL ?= parallel

all: $(TARGET) ## Build

$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean: ## Clean
	-rm -f $(TARGET) $(OBJS) compile_commands.json

.PHONY: help
help: ## Display this help
	@awk 'BEGIN {FS = ":.*##"; printf "\nUsage:\n  make \033[36m<target>\033[0m\n\nTargets:\n"} /^[a-zA-Z_-]+:.*?##/ { printf "  \033[36m%-10s\033[0m %s\n", $$1, $$2 }' $(MAKEFILE_LIST)


fmt: ## Reformatting
	-${CLANG_FORMAT} -i src/*.c src/*.cpp include/*.h include/*.hpp

compile_commands.json: ## Build compile_commands.json
	-$(RM) compile_commands.json
	$(BEAR) -- $(MAKE) clean all

.PHONY: lint
lint: compile_commands.json
	#--halt now,fail=1
	$(PARALLEL) --bar --jobs 1 $(CLANG_TIDY) -p $(PWD) -config-file=$(PWD)/.clang-tidy --use-color {} ::: $(SRCS)
