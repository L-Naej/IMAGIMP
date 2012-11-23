# $(BIN) est la nom du binaire genere
BIN = bin/imagimp
# FLAG
FLAGS = -g -Wall
#Répertoire d'include des librairies
INCLUDES = ./lib/include
# Répertoire des librairies
LIBDIR = ./lib
# Librairies
LIBS =  -lglimagimp -lglut -lGL -lGLU
# Compilateur
CC = gcc
# Compilateur
SRC = src/image.c src/utils.c

all:
	@$(CC) -o $(BIN) $(FLAGS) $(SRC) -I$(INCLUDES) -L$(LIBDIR) $(LIBS) 
	@echo 'Done'

clean:
	rm -f $(BIN)
