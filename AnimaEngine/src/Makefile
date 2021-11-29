###########################################################################################
#---------------------------------- ANIMA ENGINE -----------------------------------------#
###########################################################################################

TARGET := libAnimaEngine
MKDIR_P := mkdir -p
CC := g++
SOURCES := $(wildcard *.cpp)
_OBJECTS := $(subst .cpp,.o,$(SOURCES))
OBJ_DIR := ../obj/
SRC_DIR := ../src/
BIN_DIR := ../bin/
OBJECTS :=$(patsubst %,$(OBJ_DIR)%,$(_OBJECTS))
LIBS := -L/usr/X11R6/lib/64 -lGLEW -lglfw3 -ldl -lpthread -lX11 -lXi -lXrandr -lXxf86vm -lXinerama -lXcursor -lrt -lm -lglut -lGL -lXxf86vm -lXext -lXcursor -lassimp -lfreetype -lBulletCollision -lBulletDynamics -lBulletFileLoader -lBulletWorldImporter -lLinearMath
CPPFLAGS = -I/usr/X11R6/include -I../head -I/usr/include -I/usr/include/freetype2 -I/usr/include/GL -I/usr/local/include/GLFW -I/usr/include/bullet

#$(BIN_DIR)ejecutable: $(OBJECTS) para cuando queramos el ejecutable en el la carpeta BIN
all: dirs AnimaEngine library copy
	$(warning Compiled Anima Engine)

copy:
	sudo cp ../bin/libAnimaEngine.so /usr/lib/

library:
	$(CC) -O3 -fPIC -shared -o $(BIN_DIR)$(TARGET).so $(addprefix $(BIN_DIR)$(OBJ_DIR), $(shell ls $(BIN_DIR)$(OBJ_DIR))) $(LIBS)

AnimaEngine: $(OBJECTS)
	$(CC) -O3 -fPIC -o $@ $^ $(LIBS)

$(OBJ_DIR)%.o: %.cpp #macro derecha $^
	$(CC) -O3 -fPIC -o $@ -c $^ $(CPPFLAGS)

#------------------------CLEAN-------------------------
clean: clean_obj
	$(warning Cleaned Anima Engine)
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

