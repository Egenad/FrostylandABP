#########################################################################################
#---------------------------------- FROSTYLAND -----------------------------------------#
#########################################################################################

MKDIR_P := mkdir -p
CC := g++
SOURCES := $(wildcard *.cpp)
_OBJECTS := $(subst .cpp,.o,$(SOURCES))
OBJ_DIR := ../obj/
SRC_DIR := ../src/
BIN_DIR := ../bin/
OBJECTS :=$(patsubst %,$(OBJ_DIR)%,$(_OBJECTS))
LIBS := -lAnimaEngine -L/usr/X11R6/lib/64 -lGLEW -lglfw3 -ldl -lpthread -lX11 -lXi -lXrandr -lXxf86vm -lXinerama -lXcursor -lrt -lm -lglut -lGL -lXxf86vm -lXext -lXcursor -lassimp -lBulletCollision -lBulletDynamics -lBulletFileLoader -lBulletWorldImporter -lLinearMath -lfmod -lfmodL -lfmodstudio -lfmodstudioL -ltinyxml -lfreetype
CPPFLAGS =-I/usr/X11R6/include -I../AnimaEngine/head -I/usr/include/freetype2 -I../head -I/usr/include/bullet -I/usr/include -I/usr/include/GL -I/usr/local/include/GLFW
CFLAGS := -O3 -Wall -g

#$(BIN_DIR)ejecutable: $(OBJECTS) para cuando queramos el ejecutable en el la carpeta BIN
all: make_engine dirs Frostyland
	$(warning Compiled Frostyland!)

make_engine:
	$(MAKE) -C ../AnimaEngine/src

Frostyland: $(OBJECTS)
	$(CC) -o $@ $^ $(LIBS)

$(OBJ_DIR)%.o: %.cpp #macro derecha $^
	$(CC) $(CFLAGS) -o $@ -c $^ $(CPPFLAGS)

#------------------------CLEAN-------------------------
.PHONY: clean

clean: clean_obj
	$(warning Cleaned all folders!)
	rm -r $(BIN_DIR)

clean_obj:
	rm -r $(OBJ_DIR)
#-----------------------------------------------------

#-----------------------DIRS------------------------
dirs: create_bin_dir
	$(MKDIR_P) $(OBJ_DIR)

create_bin_dir:
	$(MKDIR_P) $(BIN_DIR)
#---------------------------------------------------

