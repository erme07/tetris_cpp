#include "miniwin.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include <windows.h>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>


using namespace miniwin;
using namespace std;

const int cellsize = 20;

#define MARGIN 10
#define GAP 2
#define MAINROWS 20
#define MAINCOLS 10
#define NEXTROWS 4
#define NEXTCOLS 4

class Tablero{
  public:
  int ROWS;
  int COLS;
  int posX;
  int posY;

  std::vector<std::vector<int>> matriz;

  void inicializarTablero() {
      for (int i = 0; i < ROWS; ++i) {
          for (int j = 0; j < COLS; ++j) 
              matriz[i][j] = 0;
      }
  }
  Tablero(int filas, int columnas) : ROWS(filas), COLS(columnas), matriz(filas, std::vector<int>(columnas)) {
      inicializarTablero();
      if(columnas==4){
        posX = 220;
        posY = 100;
      }else{
        posX = 0;
        posY = 0;
      }
  }
  void imprimirTablero(){
    for(int y=0; y<ROWS; y++){
      for(int x=0; x<COLS; x++){
        if(matriz[y][x]==0)
          color_rgb(131,141,114);
        else
          color(NEGRO);
        rectangulo((x*cellsize)+MARGIN+posX+GAP,(y*cellsize)+MARGIN+posY+GAP,(x*cellsize)+cellsize+MARGIN+posX-1,(y*cellsize)+ cellsize + MARGIN+posY-1);
        rectangulo_lleno((x*cellsize)+MARGIN+4+posX+GAP,(y*cellsize)+MARGIN+4+posY+GAP,(x*cellsize)+cellsize+MARGIN-4+posX,(y*cellsize)+cellsize+MARGIN-4+posY);
      }
    }
  }
  void mostrarTablero(){
    for(int y=0; y<ROWS; y++){
      for(int x=0; x<COLS; x++){
        std::cout << matriz[y][x] << " ";
      }
      std::cout << endl;
    }
  }
  bool verificarLineas(int& nivel, int& puntaje){
    int lineas = 0;
    for(int i=0; i<ROWS; i++){
      int contador = 0;
      for(int j=0; j<COLS; j++){
        if(matriz[i][j]==1)
          contador++;
      }
      if(contador==COLS){
        lineas++;
        for(int j=0; j<COLS; j++)
          matriz[i][j]=0;
        for(int k=i; k>0; k--){
          for(int j=0; j<COLS; j++)
            matriz[k][j]=matriz[k-1][j];
        }
      }
    }
    if(lineas == 1 || lineas == 2)
      puntaje += 100 * lineas * nivel;
    else if(lineas == 3)
      puntaje += 400 * nivel;
    else if(lineas == 4)
      puntaje += 800 * nivel;
    
    return (lineas == 0) ? false : true;
  }
};

enum formas{
  O_MINO,
  I_MINO,
  T_MINO,
  S_MINO,
  Z_MINO,
  L_MINO,
  J_MINO
};

struct posicion_t{
  int x=4;
  int y=-2;
};

struct tetraminos{
  int bolsaTetraminos[7] = {O_MINO,I_MINO,T_MINO,S_MINO,Z_MINO,L_MINO,J_MINO};
  int figuraActual;
  int figuraSiguiente;
  std::random_device rd;
  std::mt19937 gen;
  
  std::vector<std::vector<int>> omino = {
    {1,1},
    {1,1}
  };
  std::vector<std::vector<int>> imino = {
    {0,0,0,0},
    {1,1,1,1},
    {0,0,0,0},
    {0,0,0,0}
  };
  std::vector<std::vector<int>> tmino = {
    {0,1,0},
    {1,1,1},
    {0,0,0}
  };
  std::vector<std::vector<int>> smino = {
    {0,1,1},
    {1,1,0},
    {0,0,0}
  };
  std::vector<std::vector<int>> zmino = {
    {1,1,0},
    {0,1,1},
    {0,0,0}
  };
  std::vector<std::vector<int>> jmino = {
    {1,0,0},
    {1,1,1},
    {0,0,0}
  };
  std::vector<std::vector<int>> lmino = {
    {0,0,1},
    {1,1,1},
    {0,0,0}
  };

  tetraminos(): gen(std::chrono::system_clock::now().time_since_epoch().count()){
    inicializarTetramino();
  }
  void inicializarTetramino(){
    figuraActual = 0;
    figuraSiguiente = 1;
    barajarBolsa();
  }
  void barajarBolsa() {
    std::shuffle(std::begin(bolsaTetraminos), std::end(bolsaTetraminos), gen);
  }
};



struct pieza_t{
  
  posicion_t posicion;
  std::vector<std::vector<int>> forma;
  tetraminos& tetramino;

  pieza_t (tetraminos& tetramino) : tetramino(tetramino){
    asignarForma(tetramino.bolsaTetraminos,tetramino.figuraActual);
  }
  void asignarForma(int bolsaTetraminos[],int posicionBolsa){
    std::vector<std::vector<int>>* formas[] = {
        &tetramino.omino,
        &tetramino.imino,
        &tetramino.tmino,
        &tetramino.smino,
        &tetramino.zmino,
        &tetramino.jmino,
        &tetramino.lmino
    };
    forma = *formas[bolsaTetraminos[posicionBolsa]];
  }
  void imprimirPieza(){
    color(NEGRO);
    for(size_t y=0; y<forma.size(); y++){
      for(size_t x=0; x<forma[y].size(); x++){
        if(forma[y][x]==1 && posicion.y+(int)y>=0){
          rectangulo((x*cellsize)+MARGIN+(posicion.x*cellsize)+GAP,(y*cellsize)+MARGIN+(posicion.y*cellsize)+GAP,(x*cellsize)+cellsize+MARGIN-1+(posicion.x*cellsize),(y*cellsize)+ cellsize + MARGIN-1+(posicion.y*cellsize));
          rectangulo_lleno((x*cellsize)+MARGIN+4+(posicion.x*cellsize)+GAP,(y*cellsize)+MARGIN+4+(posicion.y*cellsize)+GAP,(x*cellsize)+cellsize+MARGIN-4+(posicion.x*cellsize),(y*cellsize)+cellsize+MARGIN-4+(posicion.y*cellsize));
        }
      }
    }
  }
  void mostrarPieza(){
    for(size_t y=0; y<forma.size(); y++){
      for(size_t x=0; x<forma[y].size(); x++){
        std::cout << forma[y][x] << " ";
      }
      std::cout << std::endl;
    }
  }
  bool verificarColision(Tablero tablero,int tecla=ARRIBA){
    posicion_t direccion = {0, 0};
    if(tecla==DERECHA)
      direccion = {1,0};
    else if(tecla==IZQUIERDA)
      direccion = {-1, 0};
    else if(tecla==ABAJO)
      direccion = {0, 1};
    for(size_t y=0; y<forma.size(); y++){
      for(size_t x=0; x<forma[y].size(); x++){
        if (forma[y][x] == 1) {
          int posX = posicion.x + direccion.x + (int)x;
          int posY = posicion.y + direccion.y + (int)y;
          if (posX < 0 || posX >= tablero.COLS || posY >=tablero.ROWS)
            return true;
          if(posY>=0){
            if (posY < tablero.ROWS && posX >= 0 && posX < tablero.COLS && tablero.matriz[posY][posX] == 1)
              return true;
          }
        }
      }
    }
    return false;
  }
  bool verificarGameOver(Tablero tablero){
    for(size_t y=0; y<forma.size(); y++){
      for(size_t x=0; x<forma[y].size(); x++){
        if (forma[y][x] == 1) {
          int posX = posicion.x + x;
          int posY = posicion.y + y;
          if (posX < 0 || posX >= tablero.COLS || posY <0)
            return true;
        }
      }
    }
    return false;
  }
  void fijarPieza(Tablero& tablero){
    for(size_t y=0; y<forma.size(); y++){
      for(size_t x=0; x<forma[y].size(); x++){
        if (forma[y][x] == 1) {
          int posX = posicion.x + x;
          int posY = posicion.y + y;
          if (posY >= 0 && posY < tablero.ROWS && posX >= 0 && posX < tablero.COLS)
            tablero.matriz[posY][posX] = 1;
        }
      }
    }
    
  }
  void cambiarPieza(){
    posicion.y = -2;
    posicion.x = 4;
    asignarForma(tetramino.bolsaTetraminos,tetramino.figuraSiguiente);
    if(tetramino.figuraSiguiente==6){
      tetramino.barajarBolsa();
      tetramino.figuraActual=0;
      tetramino.figuraSiguiente=1;
    }else{
      tetramino.figuraActual++;
      tetramino.figuraSiguiente++;
    }
  }
  void rotarPieza(Tablero& tableroPrincipal) {
    if(posicion.x<0)
      posicion.x = 0;
    if(posicion.x + forma.size() > 9)
      posicion.x = 10 - forma.size();
    std::vector<std::vector<int>> formaOriginal = forma;
    std::vector<std::vector<int>> nuevaForma(forma[0].size(), std::vector<int>(forma.size()));
    for (size_t i = 0; i < forma.size(); i++) {
        for (size_t j = 0; j < forma[i].size(); j++) 
            nuevaForma[j][forma.size() - 1 - i] = forma[i][j];
    }
    forma = nuevaForma;
    if (verificarColision(tableroPrincipal))
        forma = formaOriginal;
  }
};


class Datos{
  public:
  int nivel;
  int score;
  int velocidad;
  Datos(){
    inicializarDatos();
  }
  void inicializarDatos(){
    nivel = 1;
    score = 0;
    velocidad = 80 * nivel;
    color(NEGRO);
    texto(230, 10, "SCORE");
    texto(230, 30, std::to_string(score));
    texto(230, 60, "LEVEL");
    texto(230, 80, std::to_string(nivel));
  }
  void actualizarScore(){
    int nuevonivel = 1;
    color_rgb(137,151,117);
    rectangulo_lleno(230, 30,320, 50);
    color(NEGRO);
    texto(230, 30, std::to_string(score));
    if(score>=12000)
      nuevonivel = 5;
    else if(score>=9000)
      nuevonivel = 4;
    else if(score>=6000)
      nuevonivel = 3;
    else if(score>=3000)
      nuevonivel = 2;
    actualizarNivel(nuevonivel);
  }
  void actualizarNivel(int nuevonivel){
    if(nuevonivel!=nivel){
      nivel = nuevonivel;
      color_rgb(137,151,117);
      rectangulo_lleno(230, 80,320, 100);
      color(NEGRO);
      texto(230, 80, std::to_string(nivel));
      velocidad = 80 * nivel;
    }
  }
};

void imprimirMarco();
void gameover(std::vector<std::vector<int>>);
void reset(Datos&,tetraminos&,Tablero&,Tablero&,pieza_t&,pieza_t&);

int main(){

  int tiempo = 400;
  bool gameOver = false;

  vredimensiona(320, 420);
  color_rgb(137,151,117);
  rectangulo_lleno(0, 0, 320, 420);
  imprimirMarco();

  Datos estadisticas;

  Tablero tableroPrincipal(20,10);
  Tablero tableroSiguiente(2,4);

  tetraminos tetramino;

  pieza_t pieza(tetramino);
  pieza_t piezaSiguiente(tetramino);

  piezaSiguiente.posicion = {0,0};
  

  tableroPrincipal.imprimirTablero();
  tableroSiguiente.imprimirTablero();
  
  refresca();
  mensaje("Comezar");
  piezaSiguiente.asignarForma(tetramino.bolsaTetraminos,tetramino.figuraSiguiente);
  piezaSiguiente.fijarPieza(tableroSiguiente);
  tableroSiguiente.imprimirTablero();

  refresca();
  
  while(!gameOver){
    int t = tecla();
    if (t == ABAJO) {
      tiempo = 0;
      if(!pieza.verificarColision(tableroPrincipal,ABAJO)){
        pieza.posicion.y+=1;
        tableroPrincipal.imprimirTablero();
        pieza.imprimirPieza();
      }
      else{
        pieza.fijarPieza(tableroPrincipal);
        if(pieza.verificarGameOver(tableroPrincipal)){
          gameover(tableroPrincipal.matriz);
          if(pregunta("Volver a jugar?")){
            reset(estadisticas, tetramino, tableroPrincipal, tableroSiguiente, pieza, piezaSiguiente);
            continue;
          }else{
            gameOver = true;
            continue;
          }
        }
        pieza.cambiarPieza();
        if(tableroPrincipal.verificarLineas(estadisticas.nivel,estadisticas.score))
          estadisticas.actualizarScore();
        tableroSiguiente.inicializarTablero();
        piezaSiguiente.asignarForma(tetramino.bolsaTetraminos,tetramino.figuraSiguiente);
        piezaSiguiente.fijarPieza(tableroSiguiente);
        tableroSiguiente.imprimirTablero();
      }
      
    }
    else if (t == ARRIBA) {
      pieza.rotarPieza(tableroPrincipal);
      tableroPrincipal.imprimirTablero();
      pieza.imprimirPieza();
    }
    else if (t == DERECHA) {
      if(!pieza.verificarColision(tableroPrincipal,t)){
        pieza.posicion.x+=1;
        tableroPrincipal.imprimirTablero();
        pieza.imprimirPieza();
      }
    }
    else if (t == IZQUIERDA) {
      if(!pieza.verificarColision(tableroPrincipal,t)){
        pieza.posicion.x-=1;
        tableroPrincipal.imprimirTablero();
        pieza.imprimirPieza();
      }
    }
    
    if(tiempo>=500 - estadisticas.velocidad && t!=ABAJO){
      tiempo = 0;
      if(!pieza.verificarColision(tableroPrincipal,ABAJO)){
        pieza.posicion.y+=1;
        tableroPrincipal.imprimirTablero();
        pieza.imprimirPieza();
      }
      else{
        pieza.fijarPieza(tableroPrincipal);
        if(pieza.verificarGameOver(tableroPrincipal)){
          gameover(tableroPrincipal.matriz);
          if(pregunta("Volver a jugar?")){
            reset(estadisticas, tetramino, tableroPrincipal, tableroSiguiente, pieza, piezaSiguiente);
            continue;
          }else{
            gameOver = true;
            continue;
          }
        }
        pieza.cambiarPieza();
        if(tableroPrincipal.verificarLineas(estadisticas.nivel,estadisticas.score))
          estadisticas.actualizarScore();
        tableroSiguiente.inicializarTablero();
        piezaSiguiente.asignarForma(tetramino.bolsaTetraminos,tetramino.figuraSiguiente);
        piezaSiguiente.fijarPieza(tableroSiguiente);
        tableroSiguiente.imprimirTablero();
      }
    }
    tiempo+=10;
    refresca();
    Sleep(10);
  }
  vcierra();
  return 0;
}

void imprimirMarco(){
  color(NEGRO);
  rectangulo(6,6,214,414);
}

void gameover(std::vector<std::vector<int>> tablero){
  color(NEGRO);
  for(int y=(int)tablero.size()-1; y>=0; y--){
    for(int x=(int)tablero[y].size()-1; x>=0; x--){
        rectangulo((x*cellsize)+MARGIN+GAP,(y*cellsize)+MARGIN+GAP,(x*cellsize)+cellsize+MARGIN-1,(y*cellsize)+ cellsize + MARGIN-1);
        rectangulo_lleno((x*cellsize)+MARGIN+4+GAP,(y*cellsize)+MARGIN+4+GAP,(x*cellsize)+cellsize+MARGIN-4,(y*cellsize)+cellsize+MARGIN-4);
    }
    refresca();
    Sleep(30);
  }
}

void reset(Datos& stats, tetraminos& tetra, Tablero& tmain, Tablero& tnext, pieza_t& pieza, pieza_t& piezanext){
  stats.inicializarDatos();
  tetra.inicializarTetramino();
  pieza.posicion = {4, -2};
  pieza.asignarForma(tetra.bolsaTetraminos, tetra.figuraActual);
  piezanext.asignarForma(tetra.bolsaTetraminos, tetra.figuraSiguiente);
  tmain.inicializarTablero();
  tnext.inicializarTablero();
  tmain.imprimirTablero();
  tnext.imprimirTablero();
  piezanext.fijarPieza(tnext);
  tnext.imprimirTablero();
  refresca();
  Sleep(10);
}
