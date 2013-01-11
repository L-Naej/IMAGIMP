# $(BIN) est la nom du binaire généré
BIN_GLIM = libglimagimp.so
BIN_LIST = liblist.so
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
LIBS_GLIM = -lGL -lGLU -lm
# Compilateur
CC = gcc
# OBJET
DIR_OBJ = obj
## OBJ_MATH : liste des objets de la bibliothèque
OBJ_GLIM = $(DIR_OBJ)/interface.o $(DIR_OBJ)/outils.o
OBJ_LIST = $(DIR_OBJ)/list.o

#finalement list n'étant pas une bibliothèque indépendante, 
#est compilée avec le projet
all: glimagimp
	@echo "-----Fin de la compilation des librairies.-----\n"
	
glimagimp: $(OBJ_GLIM)
	@echo "Phase de lien glimagimp..."
	@$(CC) -L$(LIBDIR) $(LIBS_GLIM) $(OBJ_GLIM) -fPIC -shared -o $(LIBFIN)/$(BIN_GLIM)
	@echo "glimagimp compilée"
	
list: $(OBJ_LIST)
	@echo "Phase de lien liblist..."
	@$(CC) $(OBJ_LIST) -fPIC -shared -o $(LIBFIN)/$(BIN_LIST)
	@echo "liblist compilée"


$(DIR_OBJ)/%.o: $(SRC)/%.c $(INC)/%.h
	@echo "Génération de $@\c"
	@$(CC) -I$(INC) $(FLAGS) -c $< -o $@
	@echo "=>OK"
	
clean:
	rm -f $(DIR_OBJ)/*.o
	find . -name "*~" -exec rm {} \;

bigclean:
	rm -f $(DIR_OBJ)/*.o $(SRC)/*~ $(INC)/*~ $(LIBFIN)/$(BIN_GLIM) $(LIBFIN)/$(BIN_LIST)
	find . -name "*~" -exec rm {} \;

