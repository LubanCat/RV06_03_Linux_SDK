# Makefile for building gb28181_sip
ifeq ($(APP_PARAM), )
    APP_PARAM:=../../Makefile.param
    include $(APP_PARAM)
endif

# Project name
TARGET = gb28181-sip

# Directories
BUILD_DIR = build
SRC_DIR = src
INC_DIR = . include include/libmxml4
LIB_DIR = ./lib

# Compiler and linker
CC = $(RK_APP_CROSS)-gcc

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.c)

# Object files
OBJS = $(patsubst %.c, $(BUILD_DIR)/%.o, $(notdir $(SRCS)))

# Header files
DEPS = $(wildcard $(INC_DIR)/*.h)

# Libraries
LD_LIBS = eXosip2 osip2 mxml4 osipparser2 pthread

# Compiler flags
CFLAGS = $(patsubst %, -I%, $(INC_DIR))
LDFLAGS = $(patsubst %, -L%, $(LIB_DIR)) $(patsubst %, -l%, $(LD_LIBS))

# Default target
.PHONY: all
all: $(BUILD_DIR)/$(TARGET)

# Build target
$(BUILD_DIR)/$(TARGET): $(OBJS)
	@mkdir -p $(dir $@)
	$(CC) -o $@ $^ $(LDFLAGS)

# Build object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(DEPS)
	@mkdir -p $(dir $@)
	$(CC) -c -o $@ $< $(CFLAGS)

# Clean target
.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

# Clean all target
.PHONY: cleanall
cleanall: clean

# Include dependency files if they exist
-include $(OBJS:.o=.d)
