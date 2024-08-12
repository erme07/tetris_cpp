# Variables
CXX = g++
CXXFLAGS = -Wall -g
LDFLAGS = -lgdi32 -mwindows


# Objetivo principal
all: main

# Compilación del archivo principal
main: main.o miniwin.o
	$(CXX) $(CXXFLAGS) -o main main.o miniwin.o $(LDFLAGS)

# Compilación del archivo objeto
main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

# Compilación del archivo objeto de miniwin
miniwin.o: miniwin.cpp
	$(CXX) $(CXXFLAGS) -c miniwin.cpp

# Limpiar archivos generados
clean:
	rm -f main main.o miniwin.o