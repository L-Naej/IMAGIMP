# $(BIN) est la nom du binaire genere
BIN = bin/imagimp
# FLAG
FLAGS = -g
# INCLUDES
LIBDIR = ./lib
# Librairies
LIBS =  -lglimagimp -lglut -lGL -lGLU
# Compilateur
CC = gcc
# Compilateur
SRC = src/image.c src/utils.c

all:
	@$(CC) -o $(BIN) $(FLAGS) $(SRC) -L$(LIBDIR) $(LIBS) 
	@echo 'Done'

clean:
	rm -f $(BIN)
