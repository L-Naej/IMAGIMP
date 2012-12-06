# $(BIN) est la nom du binaire genere
BIN = bin/imagimp
BINTEST = bin/test
# FLAG
FLAGS = -g -Wall
#Répertoire d'include des librairies
INCLUDES = ./lib/include
# Répertoire des librairies
LIBDIR = ./lib
# Librairies
LIBS =  -lglimagimp -lglut -lGL -lGLU -lm
# Compilateur
CC = gcc
SRC = src/image.c src/utils.c src/lut.c src/lutfunction.c 
#src/layer.c src/list.c
SRCTEST = src/testListes.c $(SRC)

all:
	@$(CC) -o $(BIN) $(FLAGS) $(SRC) -I$(INCLUDES) -L$(LIBDIR) $(LIBS) 
	
test:
	@$(CC) -o$(BINTEST) $(FLAGS) $(SRCTEST) -I$(INCLUDES) -L$(LIBDIR) $(LIBS)
	@$(BINTEST) images/Clown.256.ppm
clean:
	rm -f $(BIN)
