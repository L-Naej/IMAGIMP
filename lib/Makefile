# $(BIN) est la nom du binaire généré
BIN = libglimagimp.a
BINDYN = libglimagimp.so
# FLAG
FLAGS = -Wall -fPIC -g
# INCLUDES
INC = ./include
# LIB DIR FINALE
LIBFIN = .
# INCLUDES
LIBDIR = .
# SOURCES
SRC = ./src
# Librairies
LIBS = -lGL -lGLU -lm
# Compilateur
CC = gcc
# Pour la librairie statique
AR = ar rcs
# OBJET
DIR_OBJ = obj
## OBJ_MATH : liste des objets de la bibliothèque
OBJECTS = $(DIR_OBJ)/interface.o $(DIR_OBJ)/outils.o

# all est la première règle à être exécutée car elle est la première
# dans le fichier Makefile. Notons que les dépendances peuvent être
# remplacées par une variable, ainsi que n'importe quel chaine de
# caractères des commandes
all: $(OBJECTS)
	@echo "**** PHASE DE LIEN ****"
	$(CC) -L$(LIBDIR) $(LIBS) $(OBJECTS) -fPIC -shared -o $(LIBFIN)/$(BINDYN)

static: $(OBJECTS)
	@echo "**** PHASE DE LIEN ****"
	$(AR) $(LIBFIN)/$(BIN) $(OBJECTS)

$(DIR_OBJ)/%.o: $(SRC)/%.c $(INC)/%.h
	@echo "**** $@ ****"
	$(CC) -I$(INC) $(FLAGS) -c $< -o $@

clean:
	rm -f $(DIR_OBJ)/*.o
	find . -name "*~" -exec rm {} \;

bigclean:
	rm -f $(DIR_OBJ)/*.o $(SRC)/*~ $(INC)/*~ $(LIBFIN)/$(BIN) $(LIBFIN)/$(BINDYN)
	find . -name "*~" -exec rm {} \;

tar:
	make clean
	tar -czvf glimagimp.tgz INSTALL install.sh lib/ sample/ include/

