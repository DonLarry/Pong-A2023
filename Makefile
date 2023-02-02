INC_DIR = include
SRC_DIR = src
BIN_DIR = bin
OBJ_DIR = obj

EXE = $(BIN_DIR)/main
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

CC       = clang
CPPFLAGS = -I$(INC_DIR) -I.
CFLAGS   = -Wall
LIBS     = -lallegro -lallegro_font -lallegro_primitives -lallegro_font -lallegro_ttf -lallegro_audio -lallegro_acodec

all: $(EXE)

.PHONY: all

$(EXE): $(OBJ) | $(BIN_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) main.c $^ $(LIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

clean:
	$(RM) -r $(BIN_DIR) $(OBJ_DIR)
