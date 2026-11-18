#include <iostream>
#include <stack>
#include <vector>
using namespace std;

struct Nodo {
    int dato;
    Nodo* izq;
    Nodo* der;

    Nodo(int val) : dato(val), izq(nullptr), der(nullptr) {}
};

struct EstadoDFS {
    Nodo* nodo;
    int sumaAcumulada;
    vector<int> caminoActual;
};

bool encontrarCaminoSumaK(Nodo* raiz, int K, vector<int>& caminoResultado) {
    if (raiz == nullptr) return false;

    stack<EstadoDFS> pila;
    pila.push({raiz, raiz->dato, {raiz->dato}});

    while (!pila.empty()) {
        EstadoDFS actual = pila.top();
        pila.pop();

        Nodo* n = actual.nodo;

        // Verificar si es un nodo hoja
        if (n->izq == nullptr && n->der == nullptr) {
            if (actual.sumaAcumulada == K) {
                caminoResultado = actual.caminoActual;
                return true;
            }
        }

        // Apilar hijo derecho primero para procesar el izquierdo primero (LIFO)
        if (n->der != nullptr) {
            vector<int> nuevoCamino = actual.caminoActual;
            nuevoCamino.push_back(n->der->dato);
            pila.push({n->der, actual.sumaAcumulada + n->der->dato, nuevoCamino});
        }

        if (n->izq != nullptr) {
            vector<int> nuevoCamino = actual.caminoActual;
            nuevoCamino.push_back(n->izq->dato);
            pila.push({n->izq, actual.sumaAcumulada + n->izq->dato, nuevoCamino});
        }
    }

    return false;
}

void liberarArbol(Nodo* raiz) {
    if (!raiz) return;
    stack<Nodo*> s;
    s.push(raiz);
    while (!s.empty()) {
        Nodo* curr = s.top();
        s.pop();
        if (curr->izq) s.push(curr->izq);
        if (curr->der) s.push(curr->der);
        delete curr;
    }
}

/*
EXPLICACIÓN DE POR QUÉ DFS ES ADECUADO PARA ESTE PROBLEMA:

1. Exploración completa de ramas (Raíz -> Hoja): El problema exige evaluar 
   caminos completos desde la raíz hasta una hoja. DFS avanza en profundidad 
   siguiendo una única rama hasta alcanzar una hoja antes de retroceder 
   (backtracking), lo que coincide exactamente con la estructura de un camino 
   raíz-hoja.

2. Eficiencia espacial O(h): En comparación con BFS (que almacena todos los 
   nodos de un nivel completo en memoria, requiriendo O(w) = O(n) espacio en 
   árboles balanceados), DFS solo mantiene en la pila los estados correspondientes 
   a la rama actual y sus bifurcaciones pendientes, acotando la memoria de la 
   pila a O(h) (donde h es la altura del árbol).

3. Terminación temprana: Si el camino válido se encuentra en las primeras ramas 
   exploradas, DFS alcanza la hoja objetivo y finaliza inmediatamente sin tener 
   que procesar los niveles superiores de todo el árbol.
*/

int main() {
    /*
            5
           / \
          4   8
         /   / \
        11  13  4
       /  \      \
      7    2      1
    */
    Nodo* raiz = new Nodo(5);
    raiz->izq = new Nodo(4);
    raiz->der = new Nodo(8);
    raiz->izq->izq = new Nodo(11);
    raiz->izq->izq->izq = new Nodo(7);
    raiz->izq->izq->der = new Nodo(2);
    raiz->der->izq = new Nodo(13);
    raiz->der->der = new Nodo(4);
    raiz->der->der->der = new Nodo(1);

    int K = 22; // Camino esperado: 5 -> 4 -> 11 -> 2
    vector<int> camino;

    cout << "Buscando camino raiz-hoja con suma K = " << K << "...\n";
    if (encontrarCaminoSumaK(raiz, K, camino)) {
        cout << "Camino encontrado: ";
        for (size_t i = 0; i < camino.size(); ++i) {
            cout << camino[i] << (i + 1 < camino.size() ? " -> " : "");
        }
        cout << "\n";
    } else {
        cout << "No existe un camino con suma " << K << ".\n";
    }

    liberarArbol(raiz);
    return 0;
}