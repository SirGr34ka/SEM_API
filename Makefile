# Define variables for project name and directories
PROJECT_NAME := sem_api
SOURCE_DIR   := src
INCLUDE_DIR  := include
BUILD_DIR    ?= build

# Define variables for source and object files
SOURCE_FILES := $(wildcard $(SOURCE_DIR)/*.c)
OBJECT_FILES := $(patsubst $(SOURCE_DIR)/%.c, $(BUILD_DIR)/%.o, $(SOURCE_FILES))

# Define variables for compiler and flags
ifeq ($(origin CC), default)
	CC = ./../arm-gnu-toolchain-12.2.rel1-x86_64-aarch64-none-linux-gnu/bin/aarch64-none-linux-gnu-gcc
endif

CFLAGS ?= -Wall -Wextra -pedantic -std=c11 -I./$(INCLUDE_DIR)

.PHONY : all
all : $(PROJECT_NAME)

# Include all dependencies
NODEPS = clean

ifeq (0, $(words $(findstring $(MAKECMDGOALS), $(NODEPS))))
include $(OBJECT_FILES:.o=.d)
endif

# Compile the project
$(PROJECT_NAME) : $(OBJECT_FILES)
	$(CC) $(CFLAGS) $^ -o $@

$(BUILD_DIR)/%.o : $(SOURCE_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.d : $(SOURCE_DIR)/%.c
	$(CC) $(CFLAGS) -E -MM -MT $(@:.d=.o) $< > $@

# Clean up object files and executable
.PHONY : clean
clean :
	rm -f $(addprefix $(BUILD_DIR)/, *.o *.d) $(PROJECT_NAME)