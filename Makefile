SRC_DIR 	:= src
BUILD_DIR := build
BIN_DIR 	:= bin

EXE := $(BIN_DIR)/main
SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(SRC:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

CFLAGS   := -Wall -Wpedantic -O0 -g -std=gnu11 -Iinclude
LDFLAGS  := # -Llib
LDLIBS   := -lm
CC		 := clang

.PHONY: all clean run

all: $(EXE)

$(EXE): $(OBJ) | $(BIN_DIR)
	@$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR) $(BUILD_DIR):
	@mkdir -p $@

clean:
	@$(RM) -r $(BIN_DIR) $(BUILD_DIR)

run: all
	@./$(EXE)

-include $(OBJ:.o=.d)
