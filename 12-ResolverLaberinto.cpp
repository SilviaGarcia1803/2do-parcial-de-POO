/*Silvia Mariela Garcia Hernandez
Matricula: 362313028
Grupo: 37B-ISC
Carrera: Ingenieria en Sistemas Computacionales
Sexto cuatrimestre

4 de junio de 2025

* Realizar un programa que lea un archivo de texto e imprima su contenido
	incluyedo caracteres especiales: '¿!"#')¨[-]
* El arhcivo de texto debe tener un laberinto hecho de caracteres*/


/*RESOLVER UN LABERINTO CON LA SOLUCION BUSQUEDA EN PROFUNDIDAD (DFS)
este algoritmo busca una ruta a la cassilla final, en cada pasoverifica si hay mas de un camino que se puede recorrer.
Si es el caso, guarda la posicion en una pila para que cuando encuentre un callejón sin salida, regrese a la casilla anterior de la pila
y continue su busqueda desde esa posicion, este proceso se repite hasta encontrar la salida*/

#include <string> //crear cadenas (tipo de dato string)
#include <windows.h> // header file: Permite acceder a las funciones y servicios de la WinAPI, que son la base para la creación de aplicaciones de Windows.
#include <map>
#include <utility>
#include <iostream>
#include <fstream>
//CONSTANTES GLOBALES
#define FILAS 100
#define COLUMNAS 100

using namespace std;
using Coordenada = pair<int, int>;

/*********************************** FUNCION GUARDAR LABERINTO ********************************************/
void GuardarLaberinto(string archivo, string nombre, char lab[FILAS][COLUMNAS], int rec[FILAS][COLUMNAS], int tam[2]){
    int f=0;

    int maxCols=0;
    ifstream Leer(nombre);//leer desde el archivo de texto
    if (!Leer.is_open()) {
    cout << "Error: No se pudo abrir el archivo.\n"<<endl;
    return;
    }
    //LEYENDO EL ARCHIVO
    while (getline(Leer, archivo) && f < FILAS) { //utiliza el bucle while y la funcion getline para leer el archivo linea
        for (int c = 0; c < COLUMNAS; c++) {//este bucle recorre cada linea caracter por caracter
            if (c < archivo.length()) {//este bucle recorre cada linea caracter por caracter
                lab[f][c]=archivo[c];//se guarda el archivo en la matriz
                if (archivo[c] == 'I' || archivo[c] == 'i') {//si es igual a I guarda un 2
                    rec[f][c] = 2; // entrada
                }
                else if (archivo[c] == 'S' || archivo[c] == 's' ||  archivo[c] == 'F') {//si es igual a S o F guarda un 3
                    rec[f][c] = 3; // salida
                }
                else if (archivo[c] == ' ') {//si es igual a ' ' guarda un 1
                    rec[f][c] = 1; // espacio celda vacia
                } else {//si no se cumplen las condiciones anteriores guarda un 0
                    rec[f][c] = 0; // pared
                }
            }
            else {
                lab[f][c]=' ';
                rec[f][c] = 0; // si la linea es menor a 70 rellena lo restante con 0
            }
        }
        f++;
    }
    if (archivo.length() > maxCols) maxCols = archivo.length();
    tam[0]=maxCols; // numero real del total las columnas del laberinto (en caso de que sea menor a 70)
    tam[1]=f;//numero real del total de filas del laberinto (en caso de que sea menor a 70)
    // Rellenar filas faltantes si hay menos de 70 líneas
    for(int F = f; F < FILAS; ++F) {
        for (int c2 = 0; c2 < 70; c2++) {
            rec[F][c2] = 0;
        }
    }
    Leer.close();
    //GUARDANDO EL LABERINTO EN LA MATRIZ LABERINTO AJUSTANDO EL LABERINTO A 100X100 Y GUARDANDO EN LA MATRIZ RECREAR
}

/*********************************** ESTRUCTURA NODO ********************************************/
struct Nodo{
    int GuardarY;
    int GuardarX;
    Nodo* Next;
    Nodo(int gY, int gX) : GuardarY(gY), GuardarX(gX), Next(nullptr) {}
};

/*********************************** CLASE PILA ENLAZADA ********************************************/
class PilaEnlazada{
private:
    Nodo* Top;
public:
    //constructor de la clase
    PilaEnlazada(){
        Top=nullptr;
    }
    //metodos
    bool Empty(){
        return Top==nullptr;
    }
    void Insertar(int gY, int gX){
        Nodo* NodoNuevo = new Nodo(gY, gX);
        NodoNuevo->Next = Top;
        Top = NodoNuevo;
    }
    bool Retirar(int &gY, int &gX) {
        if (Empty()) return false;
        Nodo* temp = Top;
        gY = Top->GuardarY;
        gX = Top->GuardarX;
        Top = Top->Next;
        delete temp;
        return true;
    }

    bool VerUltimo(int &gY, int &gX){
        if (Empty()) return false;
        Nodo* temp = Top;
        gY = Top->GuardarY;
        gX = Top->GuardarX;
        return true;
    }
    //destructor de la clase
    ~PilaEnlazada() {
        int y, x;
        while(Retirar(y, x));
    }
};

/*********************************** FUNCION IMPRIMIR CAMINO ********************************************/
void ImprimirCamino(char lab[FILAS][COLUMNAS], map<Coordenada, Coordenada>& padre_mapa, int Yinicio, int Xinicio, int Yfin, int Xfin, int tamReal[2], int rec[FILAS][COLUMNAS]){
    Coordenada celda_actual = {Yfin, Xfin};
    //se crea un bucle
    while (celda_actual != Coordenada{Yinicio, Xinicio}){
        int y = celda_actual.first;
        int x = celda_actual.second;

        if(rec[y][x]!=2 && rec[y][x]!=3){//
            lab[y][x]='o';
        }
        auto it = padre_mapa.find(celda_actual);
        if (it != padre_mapa.end()) { // Si el padre existe en el mapa
             celda_actual = it->second; // Obtener el valor (el padre)
        } else {
             break; // Si por alguna razón no tiene padre y no es el inicio, se rompe el bucle.
        }
    }
    //imprimir laberinto
    for (int i = 0; i < tamReal[1]; ++i) {
        for (int j = 0; j < tamReal[0]; ++j) {
            cout << lab[i][j];
        }
        cout << endl;
    }
}

/*********************************** FUNCION SOLUCION BUSQUEDA PROFUNDA (DFS) ********************************************/
bool SolucionDFS(bool visit[FILAS][COLUMNAS], int Yinicio, int Xinicio, int Yfin, int Xfin, int rec[FILAS][COLUMNAS], int tamReal[2], char lab[FILAS][COLUMNAS]){
    //son arreglos que guardan las direcciones en orden: derecha, abajo, arriba, izquierda
    //int DirY[4]={0,1,-1,0};//direccion en el eje y (filas)
    //int DirX[4]={1,0,0,-1};//direccion en eje x (columnas)

    //cambio de direcciones abajo, derecha, arriba, izquierda
    int DirY[4]={1,0,-1,0};//direccion en el eje y (filas)
    int DirX[4]={0,1,0,-1};//direccion en eje x (columnas)
    //cout<<"CREAR DICCIONARIO \"PADRE\""<<endl;
    map<Coordenada, Coordenada> padre;
     PilaEnlazada PILA;//instanciar objeto

    //inicializar las celdas de la matriz en falso (ninguna celda ha sido visitada)
    for(int fila=0;fila<FILAS;fila++){
        for(int columna=0;columna<COLUMNAS;columna++){
            visit[fila][columna]=false;
        }
    }

    //LLAMAR AL METODO "INSERTAR" DE LA CLASE "PILA-ENLAZADA" Y GUARDAR LAS POSICION DE INICIO
    PILA.Insertar(Yinicio, Xinicio);
    //MARCAR LA POSICION DE INICIO COMO VERDADERO EN LA MATRIZ "VISITADO"
    visit[Yinicio][Xinicio]=true;

    //CREAR UN BUCLE PARA RECORRER EL LABERINTO LLAMANDO AL METODO "EMPTY" DE LA CLASE "PILA-ENLAZADA": "MIENTRAS LA PILA NO ESTE VACIA"
    while(!PILA.Empty()){
        int y, x;
        //LLAMAR AL METODO "VER-ULTIMO" DE LA CLASE "PILA-ENLAZADA": PARA ACCEDER A LOS DATOS SIN SACARLOS DE LA PILA
        PILA.VerUltimo(y, x);

        //CREA UN CONDICIONAL PARA DETECTAR SI SE LLEGO AL FINAL, SALIR DEL BUCLE E IMPRIMIR LA SOLUCION
        if(y==Yfin && x==Xfin){
            cout<<"Camino encontrado"<<endl;
            ImprimirCamino(lab, padre, Yinicio, Xinicio, Yfin, Xfin, tamReal, rec);
            return true;
        }

        //SE LE CREA Y ASIGNA EL VALOR FALSO A LA VARIABLE  "VECINO-ENCONTRADO"
            bool VecinoEncontrado=false;

            //SE CREA UN BUCLE FOR PARA ELEGIR UNA DIRECCION PARA MOVERSE
            for(int Direccion=0; Direccion<4; Direccion++){
                //se crean las variables nfY y ncX para asignar las direciones en Y (filas) & X (columans) usando los arreglos de direcion
                int nfY=y+DirY[Direccion];
                int ncX=x+DirX[Direccion];
                //el condicional se asegura de no salirse de los limites, que en la direccion ho haya una pared y que la celda no este visitada
                if(nfY>=0 && nfY<FILAS && ncX>=0 && ncX<COLUMNAS && (rec[nfY][ncX]==1 || rec[nfY][ncX]==3) && !visit[nfY][ncX]){
                    //avanza hacia la nueva celda y la marca como visitada
                    visit[nfY][ncX]=true;
                    PILA.Insertar(nfY, ncX);
                    //crea un nuev elemento de padre para guardar la nueva posicion (coordenada)
                    padre[{nfY, ncX}] = {y,x};
                    //marca la casilla como visitado
                    VecinoEncontrado=true;
                    break;
                }
            }
            //CONDICIONAL: si no encuentra mas vecinos (llega a un callejon sin salida)
            if(!VecinoEncontrado){
                //se retira el ultimo elemento de la pila
                int auxY, auxX;
                PILA.Retirar(auxY, auxX);
            }
    }
        //en algunos casos si visito todas las celdas y no encontro la salida se rompe el bucle y el programa termina
        cout<<"no se encontro una salida"<<endl;
        return false;
}

/*********************************** FUNCION PRINCIPAL ********************************************/
int main(){
    SetConsoleOutputCP(CP_UTF8); // para mostrar correctamete cualquier carácter Unicode. ☻ ••••••
    //DECLARAR VARIABLES
	string Archivo, Nombre;
	char laberinto[FILAS][COLUMNAS]; //guarda los caracteres del archivo txt en una matriz
	bool visitado[FILAS][COLUMNAS];//marca las celdas que ya han sido visitadas
	int recrear[FILAS][COLUMNAS];//traduce el codigo a 0=pared, 1=espacio, 2=inicio, y 3=salida
	int TamanoReal[2];//[0]=columna y [1]=fila
	int Xi, Yi, Xf, Yf;//, x, y;//movimientos y=fila, x=columna

	cout<<"Ingresa el nombre del archivo"<<endl;
    getline(cin, Nombre);
    //llamada a la funcion GuardarLaberinto
    GuardarLaberinto(Archivo,Nombre,laberinto,recrear, TamanoReal);

    //encontrar el inicio y final del laberinto
    for(int fila=0;fila<FILAS;++fila){
        for(int columna=0;columna<COLUMNAS;++columna){
            if(recrear[fila][columna]==2){
                Yi=fila;
                Xi=columna;
            }
            else if(recrear[fila][columna]==3){
                Yf=fila;
                Xf=columna;
            }
        }
    }

    //resolver el laberinto e imprimirlo
    SolucionDFS(visitado, Yi, Xi, Yf, Xf, recrear, TamanoReal, laberinto);

    return 0;
}
