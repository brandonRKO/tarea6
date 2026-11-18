#include <iostream>
#include <queue>
#include <algorithm>
using namespace std;

struct Nodo {
    int dato;
    Nodo* izq;
    Nodo* der;

    Nodo(int val) : dato(val), izq(nullptr), der(nullptr) {}
};
// Un árbol binario es completo si todos sus niveles están llenos excepto 
// posiblemente el último, donde todos los nodos están lo más a la izquierda posible.
bool esArbolCompleto(Nodo* raiz) {
    if (raiz == nullptr) return true;

    queue<Nodo*> cola;
    cola.push(raiz);
    bool seVioNulo = false;

    while (!cola.empty()) {
        Nodo* actual = cola.front();
        cola.pop();

        if (actual == nullptr) {
            seVioNulo = true;
        } else {
            // i ya apareció un hijo nulo antes, no puede haber más nodos después
            if (seVioNulo) {
                return false;
            }
            cola.push(actual->izq);
            cola.push(actual->der);
        }
    }
    return true;
}

//2. Calcular el ancho máximo del árbol usando EXCLUSIVAMENTE queue
int calcularAnchoMaximo(Nodo* raiz) {
    if (raiz == nullptr) return 0;

    queue<Nodo*> cola;
    cola.push(raiz);
    int anchoMaximo = 0;

    while (!cola.empty()) {
        int tamNivel = cola.size();
        anchoMaximo = max(anchoMaximo, tamNivel);

        for (int i = 0; i < tamNivel; ++i) {
            Nodo* actual = cola.front();
            cola.pop();

            if (actual->izq != nullptr) cola.push(actual->izq);
            if (actual->der != nullptr) cola.push(actual->der);
        }
    }
    return anchoMaximo;
}

// (Número de nodos o niveles desde la raíz hasta la hoja más cercana; 
// aquí expresado en aristas: raíz = 0, o niveles = 1 según convención. Usaremos niveles/nodos = 1 para raíz).
int calcularProfundidadMinima(Nodo* raiz) {
    if (raiz == nullptr) return 0;

    queue<Nodo*> cola;
    cola.push(raiz);
    int profundidad = 1; //La raíz está en profundidad 1 (cantidad de nodos en el camino)

    while (!cola.empty()) {
        int tamNivel = cola.size();

        for (int i = 0; i < tamNivel; ++i) {
            Nodo* actual = cola.front();
            cola.pop();

            // Al ser recorrido por niveles, la primera hoja encontrada garantiza la profundidad mínima
            if (actual->izq == nullptr && actual->der == nullptr) {
                return profundidad;
            }

            if (actual->izq != nullptr) cola.push(actual->izq);
            if (actual->der != nullptr) cola.push(actual->der);
        }
        profundidad++;
    }
    return profundidad;
}

void liberarArbol(Nodo* raiz) {
    if (!raiz) return;
    queue<Nodo*> q;
    q.push(raiz);
    while (!q.empty()) {
        Nodo* curr = q.front();
        q.pop();
        if (curr->izq) q.push(curr->izq);
        if (curr->der) q.push(curr->der);
        delete curr;
    }
}

/*
JUSTIFICACIÓN FORMAL DE POR QUÉ BFS ES EL ENFOQUE NATURAL PARA ESTAS MÉTRICAS:

Por inducción sobre la distancia d desde la raíz, el algoritmo BFS (usando una 
cola FIFO) garantiza que todos los nodos a profundidad d son desencolados y 
procesados estrictamente antes que cualquier nodo a profundidad d + 1. Esta 
invariante de orden por niveles hace que BFS sea óptimo para las tres métricas:

1. Completitud del árbol: La definición formal de árbol binario completo exige 
   que en el recorrido por niveles (level-order), ningún nodo válido aparezca 
   después del primer puntero nulo (nullptr). BFS visita los nodos exactamente 
   en ese orden lineal de izquierda a derecha por nivel, permitiendo detectar 
   huecos en O(n) tiempo sin cálculos de índices.

2. Ancho máximo: El ancho de un nivel d es la cardinalidad del conjunto de nodos 
   a distancia d de la raíz: W(d) = |{v in V : dist(raiz, v) = d}|. Dado que 
   BFS agrupa en la cola todos los nodos de un mismo nivel simultáneamente al 
   inicio de cada iteración externa, W(d) se obtiene directamente mediante 
   cola.size() en tiempo O(1) por nivel.

3. Profundidad mínima: Se define como min_{v in Hojas} dist(raiz, v). Como BFS 
   explora distancias de forma monótonamente no decreciente (d = 1, 2, 3, ...), 
   el primer nodo hoja encontrado es matemáticamente el más cercano a la raíz. 
   Esto permite detener la ejecución anticipadamente (early exit) sin explorar 
   subárboles más profundos innecesariamente, a diferencia de DFS que tendría 
   que recorrer todo el árbol para comparar profundidades.
*/

int main() {
    // Construcción de árbol de prueba
    /*
            1
           / \
          2   3
         / \   \
        4   5   6  -> No es completo (falta hijo izq de 3), ancho max = 3, prof min = 3
    */
    Nodo* arbol1 = new Nodo(1);
    arbol1->izq = new Nodo(2);
    arbol1->der = new Nodo(3);
    arbol1->izq->izq = new Nodo(4);
    arbol1->izq->der = new Nodo(5);
    arbol1->der->der = new Nodo(6);

    cout << "ANALISIS ARBOL 1 :\n";
    cout << "Es completo?: " << (esArbolCompleto(arbol1) ? "SI" : "NO") << "\n";
    cout << "Ancho maximo: " << calcularAnchoMaximo(arbol1) << "\n";
    cout << "Profundidad minima (nodos): " << calcularProfundidadMinima(arbol1) << "\n\n";

    // Construcción de un árbol completo
    /*
            10
           /  \
          20   30
         /  \
        40  50     -> Completo, ancho max = 2, prof min = 2 (hoja 30)
    */
    Nodo* arbol2 = new Nodo(10);
    arbol2->izq = new Nodo(20);
    arbol2->der = new Nodo(30);
    arbol2->izq->izq = new Nodo(40);
    arbol2->izq->der = new Nodo(50);

    cout << "ANALISIS ARBOL 2 :\n";
    cout << "Es completo?: " << (esArbolCompleto(arbol2) ? "SI" : "NO") << "\n";
    cout << "Ancho maximo: " << calcularAnchoMaximo(arbol2) << "\n";
    cout << "Profundidad minima (nodos): " << calcularProfundidadMinima(arbol2) << "\n";

    liberarArbol(arbol1);
    liberarArbol(arbol2);
    return 0;
}