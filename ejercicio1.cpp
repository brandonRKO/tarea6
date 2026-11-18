#include <iostream>
#include <stack>
#include <queue>
#include <vector>
using namespace std;

struct NodoBST {
    int dato;
    NodoBST* izq;
    NodoBST* der;

    NodoBST(int val) : dato(val), izq(nullptr), der(nullptr) {}
};

class BST {
    NodoBST* raiz;
public:
    BST() : raiz(nullptr) {}

    ~BST() {
        if (!raiz) return;
        queue<NodoBST*> q;
        q.push(raiz);
        while (!q.empty()) {
            NodoBST* actual = q.front();
            q.pop();
            if (actual->izq) q.push(actual->izq);
            if (actual->der) q.push(actual->der);
            delete actual;
        }
    }

    void insertar(int val) {
        NodoBST* nuevo = new NodoBST(val);
        if (!raiz) {
            raiz = nuevo;
            return;
        }

        NodoBST* actual = raiz;
        NodoBST* padre = nullptr;

        while (actual != nullptr) {
            padre = actual;
            if (val < actual->dato) {
                actual = actual->izq;
            } else if (val > actual->dato) {
                actual = actual->der;
            } else {
                delete nuevo; // No se permiten duplicados
                return;
            }
        }

        if (val < padre->dato) {
            padre->izq = nuevo;
        } else {
            padre->der = nuevo;
        }
    }

    bool eliminar(int val) {
        NodoBST* actual = raiz;
        NodoBST* padre = nullptr;

        // Buscar el nodo y su padre
        while (actual != nullptr && actual->dato != val) {
            padre = actual;
            if (val < actual->dato) actual = actual->izq;
            else actual = actual->der;
        }

        if (actual == nullptr) return false; // No encontrado

        // Caso 1 y 2: El nodo tiene a lo sumo un hijo
        if (actual->izq == nullptr || actual->der == nullptr) {
            NodoBST* nuevoHijo = (actual->izq != nullptr) ? actual->izq : actual->der;

            if (padre == nullptr) {
                raiz = nuevoHijo;
            } else if (padre->izq == actual) {
                padre->izq = nuevoHijo;
            } else {
                padre->der = nuevoHijo;
            }
            delete actual;
        } 
        // Caso 3: El nodo tiene dos hijos
        else {
            NodoBST* padreSucesor = actual;
            NodoBST* sucesor = actual->der;

            while (sucesor->izq != nullptr) {
                padreSucesor = sucesor;
                sucesor = sucesor->izq;
            }

            actual->dato = sucesor->dato;

            if (padreSucesor->izq == sucesor) {
                padreSucesor->izq = sucesor->der;
            } else {
                padreSucesor->der = sucesor->der;
            }
            delete sucesor;
        }
        return true;
    }

    //3. Recorrido in-order usando stack
    void inOrderIterativo() const {
        stack<NodoBST*> pila;
        NodoBST* actual = raiz;

        cout << "In-Order: ";
        while (actual != nullptr || !pila.empty()) {
            while (actual != nullptr) {
                pila.push(actual);
                actual = actual->izq;
            }
            actual = pila.top();
            pila.pop();
            cout << actual->dato << " ";
            actual = actual->der;
        }
        cout << "\n";
    }

    // 4. Método para calcular la altura del árbol (iterativo por niveles)
    // Convención: árbol vacío = -1, árbol con solo raíz = 0
    int calcularAltura() const {
        if (raiz == nullptr) return -1;

        queue<NodoBST*> q;
        q.push(raiz);
        int altura = -1;

        while (!q.empty()) {
            int tamNivel = q.size();
            altura++;
            for (int i = 0; i < tamNivel; ++i) {
                NodoBST* nodo = q.front();
                q.pop();
                if (nodo->izq) q.push(nodo->izq);
                if (nodo->der) q.push(nodo->der);
            }
        }
        return altura;
    }
};

/*
EXPLICACIÓN MATEMÁTICA DE LA DEGRADACIÓN A O(n) AL INSERTAR n ELEMENTOS:

Sea una secuencia estrictamente creciente S = {x_1, x_2, ..., x_n} donde 
x_1 < x_2 < ... < x_n.

1. Por la invariante de orden del BST, para todo nodo u:
   - Todos los elementos en el subárbol izquierdo son < u.
   - Todos los elementos en el subárbol derecho son > u.

2. Al insertar x_1, se establece como raíz en el nivel 0 (altura h = 0).
3. Para cada inserción sucesiva x_k (con 2 <= k <= n), dado que x_k > x_i 
   para todo i < k, el algoritmo de búsqueda descenderá exclusivamente por 
   los punteros derechos:
       raiz -> raiz->der -> raiz->der->der -> ...
   creando una cadena lineal (árbol sesgado a la derecha) donde cada nodo 
   tiene un único hijo derecho y su subárbol izquierdo es nulo.

4. La altura h(n) del árbol tras n inserciones satisface la recurrencia:
       h(1) = 0
       h(n) = h(n - 1) + 1  =>  h(n) = n - 1

5. El costo temporal T(n) de insertar el k-ésimo elemento requiere k - 1 
   comparaciones. El costo total de construir el árbol es la serie aritmética:
       T(n) = sum_{k=1}^{n} (k - 1) = n(n - 1) / 2 = Theta(n^2)
   Y el costo de una operación individual (búsqueda, inserción o eliminación) 
   en el peor caso es proporcional a la altura del árbol:
       T_op(n) = O(h(n)) = O(n - 1) = O(n).
*/

int main() {
    BST arbol;

    // 1. Insertar 20 elementos
    vector<int> elementos = {50, 30, 70, 20, 40, 60, 80, 10, 25, 35, 
                                  45, 55, 65, 75, 85, 5, 15, 22, 28, 90};
    for (int val : elementos) {
        arbol.insertar(val);
    }

    cout << "Altura inicial tras 20 inserciones: " << arbol.calcularAltura() << "\n";
    arbol.inOrderIterativo();

    // 2. Eliminar 2 elementos (ej. una hoja y un nodo con dos hijos)
    cout << "\nEliminando 5 (hoja) y 30 (nodo con 2 hijos)...\n";
    arbol.eliminar(5);
    arbol.eliminar(30);

    // 3. Recorrido in-order y altura final
    arbol.inOrderIterativo();
    cout << "Altura final tras eliminar 2 elementos: " << arbol.calcularAltura() << "\n";

    // Demostración experimental de degradación con 20 elementos ordenados
    BST arbolDegradado;
    for (int i = 1; i <= 20; ++i) {
        arbolDegradado.insertar(i);
    }
    cout << "\nAltura del BST con 20 elementos ordenados (1..20): " 
              << arbolDegradado.calcularAltura() << " (n - 1 = 19)\n";

    return 0;
}