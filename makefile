# --------------------------------------------------------------------
# Makefile Genérico :: Módulo 2. Curso Experto Desarrollo Videojuegos
# Carlos González Morcillo     Escuela Superior de Informática (UCLM)
# --------------------------------------------------------------------
EXEC := PacMan

DIRSRC := src/
DIROBJ := obj/
DIRHEA := include/

CXX := g++

# Flags de compilación -----------------------------------------------
CXXFLAGS := -I $(DIRHEA) -Wall -I/usr/local/include/cegui-0/CEGUI -I/usr/local/include/cegui-0 `pkg-config --cflags OGRE OIS OGRE-Overlay`
# Flags del linker ---------------------------------------------------
LDFLAGS :=  `pkg-config --libs-only-L OGRE` -lOIS -lGL -lstdc++ -lboost_system -lCEGUIBase-0 -lCEGUIOgreRenderer-0
LDLIBS := `pkg-config --libs-only-l gl OIS OGRE OGRE-Overlay SDL2_mixer` -lstdc++ -lboost_system -lOIS -lGL

# Modo de compilación (-mode=release -mode=debug) --------------------
ifeq ($(mode), release) 
	CXXFLAGS += -O2 -D_RELEASE
else 
	CXXFLAGS += -g -D_DEBUG
	mode := debug
endif

# Obtención automática de la lista de objetos a compilar -------------
OBJS := $(subst $(DIRSRC), $(DIROBJ), \
	$(patsubst %.cpp, %.o, $(wildcard $(DIRSRC)*.cpp)))

.PHONY: all clean

all: info dirs $(EXEC)

info:
	@echo '------------------------------------------------------'
	@echo '>>> Using mode $(mode)'
	@echo '    (Please, call "make" with [mode=debug|release])  '
	@echo '------------------------------------------------------'

dirs:
	mkdir -p $(DIROBJ)

# Enlazado -----------------------------------------------------------
$(EXEC): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

# Compilación --------------------------------------------------------
$(DIROBJ)%.o: $(DIRSRC)%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpieza de temporales ---------------------------------------------
clean:
	rm -f *.log $(EXEC) *~ $(DIRSRC)*~ $(DIRHEA)*~ 
	rm -rf $(DIROBJ)
edit:
	emacs $(wildcard $(DIRSRC)*.cpp) $(wildcard $(DIRHEA)*.h) &
