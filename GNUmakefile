# -*- mode: makefile-gmake; -*-

TARGET=scripts-rofi
SRCS   = main.c
OBJS   = $(SRCS:.c=.o)


# ----------------------------------------------


LDFLAGS += -fsanitize=address,undefined

CFLAGS  += -g -O0 -Wall -Werror -Wpedantic \
		-D_GLIBCXX_ASSERTIONS \
		-fstack-protector \
		-fsanitize=address,undefined


all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)




