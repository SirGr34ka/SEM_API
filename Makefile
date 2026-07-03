# Define variables for project name and required libraries
PROJECT_NAME := sem_api
SOURCE_DIR   := src
INCLUDE_DIR  := include
BUILD_DIR    := build

# Define compiler flags
CC           := ./../arm-gnu-toolchain-12.2.rel1-x86_64-aarch64-none-linux-gnu/bin/aarch64-none-linux-gnu-gcc
CFLAGS       := -Wall -Wextra -pedantic -std=c11

all: $(PROJECT_NAME)

# Compile the project
$(PROJECT_NAME): $(notdir $(patsubst %.c, %.o, $(wildcard $(SOURCE_DIR)/*.c)))
	$(CC) $(CFLAGS) $^ -o $@

VPATH := $(BUILD_DIR)

%.o: $(SOURCE_DIR)/%.c
	$(CC) $(CFLAGS) -c -MD $^ -o $(BUILD_DIR)/$@
	include $(patsubst $(SOURCE_DIR)/%.c, $(BUILD_DIR)/%.d, $^)

# Clean up object files and executable
clean:
	rm -f $(addprefix $(BUILD_DIR)/, *.o *.d) $(PROJECT_NAME)