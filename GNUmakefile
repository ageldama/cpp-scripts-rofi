# -*- mode: makefile-gmake; -*-

TARGET=scripts-rofi
SRCS   = src/main.cpp src/argp.cpp src/tildeexpand.cpp src/db.cpp
OBJS   = $(SRCS:.cpp=.o)


# ----------------------------------------------

DEBUG?=1

CFLAGS  += -Wall -Werror -Wpedantic

#ifeq ($(DEBUG), 1)
	CFLAGS  += -g -O0
	CFLAGS  += -fstack-protector
	CFLAGS  += -fsanitize=address,undefined

	LDFLAGS += -fsanitize=address,undefined
#else
	CFLAGS  += -O3
#endif

CFLAGS  += -Imlib -Iinclude

CXXFLAGS += -std=c++20 $(CFLAGS)


CLANG_FORMAT ?= clang-format


all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)

fmt:
	${CLANG_FORMAT} -i src/*.c src/*.cpp include/*.h include/*.hpp



