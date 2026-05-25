# -*- mode: makefile-gmake; -*-

TARGET=scripts-rofi
SRCS   = src/main.c src/argp.c src/tildeexpand.c src/db.c
OBJS   = $(SRCS:.c=.o)


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


CLANG_FORMAT ?= clang-format


all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)

fmt:
	${CLANG_FORMAT} -i src/*.c include/*.h



