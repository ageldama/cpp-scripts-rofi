# -*- mode: makefile-gmake; -*-

TARGET=scripts-rofi
SRCS   = src/main.cpp src/argp.cpp src/tildeexpand.cpp src/db.cpp src/file_find.cpp src/rofi.cpp src/rofi_ask_yn.cpp src/rofi_sel_list.cpp src/str.cpp src/safe_regex.cpp src/exec.cpp src/main_aux.cpp src/str_tokenize_cmdline.cpp
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


all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)

fmt:
	-${CLANG_FORMAT} -i src/*.c src/*.cpp include/*.h include/*.hpp



