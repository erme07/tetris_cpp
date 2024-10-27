#Nombre del ejecutable
EXECUTABLE=tetris

# Variables
CXX = g++
CXXFLAGS = -I$(INCLUDE_DIR) -Wall -g
LDFLAGS = -lgdi32 -mwindows
#LDFLAGS = -lgdi32
OBJ_DIR=./obj
SRC_DIR=./src
INCLUDE_DIR=./include


# Objetivo principal
all: create_dirs $(EXECUTABLE)

# Compilación del archivo principal
tetris: $(OBJ_DIR)/main.o $(OBJ_DIR)/miniwin.o
	$(CXX) $(CXXFLAGS) -o tetris $(OBJ_DIR)/main.o $(OBJ_DIR)/miniwin.o $(LDFLAGS)

# Compilación del archivo objeto
$(OBJ_DIR)/main.o: $(SRC_DIR)/main.cpp $(INCLUDE_DIR)/miniwin.h 
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/main.cpp -o $(OBJ_DIR)/main.o

# Compilación del archivo objeto de miniwin
$(OBJ_DIR)/miniwin.o: $(INCLUDE_DIR)/miniwin.h $(SRC_DIR)/miniwin.cpp 
	$(CXX) $(CXXFLAGS) -c $(SRC_DIR)/miniwin.cpp -o $(OBJ_DIR)/miniwin.o

# Crear directorios necesarios
create_dirs:
	mkdir -p $(OBJ_DIR)

# Limpiar archivos generados
clean:
	rm -f $(OBJ_DIR)/*.o $(EXECUTABLE)