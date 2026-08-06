DEBUG ?= 1
TARGET_DIR = target

CFLAGS = -Wall -Wextra -Werror -std=c11
LDFLAGS =

ifeq ($(DEBUG),1)
	CFLAGS += -g -O0 -DDEBUG
	OBJ_DIR = $(TARGET_DIR)/debug
else
	CFLAGS += -O3 -DNDEBUG
	OBJ_DIR = $(TARGET_DIR)/release
endif

SRCS := $(filter-out %_ut.c, $(wildcard *.c))
OBJS := $(addprefix $(OBJ_DIR)/,$(SRCS:.c=.o))
TARGET := $(OBJ_DIR)/lox

UT_SRCS := utest.c $(wildcard *_ut.c)
UT_OBJS := $(addprefix $(OBJ_DIR)/,$(UT_SRCS:.c=.o))
UT := $(OBJ_DIR)/utest

build: $(TARGET)

test: $(UT)
	$(UT)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $< 

$(UT): $(UT_OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ 

$(OBJ_DIR)/%.o: %.c $(OBJ_DIR)
	$(CC) -c $(CFLAGS) -o $@ $<

$(OBJ_DIR):
	mkdir -p $@
	
clean:
	$(RM) -fr $(TARGET_DIR)

fmt:
	rg --files -g '*.c' -g '*.h' | xargs clang-format -i

.PHONY: build test clean fmt
