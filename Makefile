# Compilateur utilisé
CC=g++

#Chemin Eigen
path=./EigenLibrary/Eigen

# Options en mode optimisé - La variable NDEBUG est définie
OPTIM_FLAG = -O3 -DNDEBUG -I $(path) -std=c++11 -Wall
# Options en mode debug
DEBUG_FLAG = -g -I $(path) -std=c++11 -Wall

# On choisit comment on compile
CXX_FLAGS = $(DEBUG_FLAG)

# Le nom de l'exécutable
PROGCHALEUR = runChaleur
PROGINVERSE = runInverse

# Les fichiers source à compiler
SRCCHALEUR = mainChaleur.cc Chaleur2D.cpp Datafile.cpp
SRCINVERSE = mainInverse.cc ProblemeInverse.cpp Datafile.cpp

# La commande complète : compile seulement si un fichier a été modifié
$(PROGINVERSE) : $(SRCINVERSE)
	$(CC) $(SRCINVERSE) $(CXX_FLAGS) -o $(PROGINVERSE)

$(PROGCHALEUR) : $(SRCCHALEUR)
	$(CC) $(SRCCHALEUR) $(CXX_FLAGS) -o $(PROGCHALEUR)

# Supprime l'exécutable, les fichiers binaires (.o) et les fichiers
# temporaires de sauvegarde (~)
clean :
	rm -f *.o *~ $(PROG)
