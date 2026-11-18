#include <iostream>
#include <algorithm>
using namespace std;

enum Color { ROJO, NEGRO };

struct NodoRB {
    int dato;
    Color color;
    NodoRB* izq;
    NodoRB* der;
    NodoRB* padre;

    NodoRB(int val) : dato(val), color(ROJO), izq(nullptr), der(nullptr), padre(nullptr) {}
};

class RedBlackTree {
    NodoRB* raiz;
    NodoRB* TNULL; // Nodo centinela negro

    void rotacionIzquierda(NodoRB* x) {
        NodoRB* y = x->der;
        x->der = y->izq;
        if (y->izq != TNULL) {
            y->izq->padre = x;
        }
        y->padre = x->padre;
        if (x->padre == nullptr) {
            raiz = y;
        } else if (x == x->padre->izq) {
            x->padre->izq = y;
        } else {
            x->padre->der = y;
        }
        y->izq = x;
        x->padre = y;
    }

    void rotacionDerecha(NodoRB* y) {
        NodoRB* x = y->izq;
        y->izq = x->der;
        if (x->der != TNULL) {
            x->der->padre = y;
        }
        x->padre = y->padre;
        if (y->padre == nullptr) {
            raiz = x;
        } else if (y == y->padre->der) {
            y->padre->der = x;
        } else {
            y->padre->izq = x;
        }
        x->der = y;
        y->padre = x;
    }

    void corregirInsercion(NodoRB* z) {
        while (z->padre != nullptr && z->padre->color == ROJO) {
            NodoRB* abuelo = z->padre->padre;
            if (z->padre == abuelo->izq) {
                NodoRB* tio = abuelo->der;
                // Caso 1: El tío es ROJO -> Recoloración
                if (tio->color == ROJO) {
                    z->padre->color = NEGRO;
                    tio->color = NEGRO;
                    abuelo->color = ROJO;
                    z = abuelo;
                } else {
                    // Caso 2: z es hijo derecho (Triángulo) -> Rotación Izquierda
                    if (z == z->padre->der) {
                        z = z->padre;
                        rotacionIzquierda(z);
                    }
                    // Caso 3: z es hijo izquierdo (Línea) -> Recoloración + Rotación Derecha
                    z->padre->color = NEGRO;
                    abuelo->color = ROJO;
                    rotacionDerecha(abuelo);
                }
            } else {
                NodoRB* tio = abuelo->izq;
                // Caso 1 simétrico: El tío es ROJO -> Recoloración
                if (tio->color == ROJO) {
                    z->padre->color = NEGRO;
                    tio->color = NEGRO;
                    abuelo->color = ROJO;
                    z = abuelo;
                } else {
                    // Caso 2 simétrico: z es hijo izquierdo -> Rotación Derecha
                    if (z == z->padre->izq) {
                        z = z->padre;
                        rotacionDerecha(z);
                    }
                    // Caso 3 simétrico: z es hijo derecho -> Recoloración + Rotación Izquierda
                    z->padre->color = NEGRO;
                    abuelo->color = ROJO;
                    rotacionIzquierda(abuelo);
                }
            }
        }
        // Propiedad 2: La raíz siempre es negra
        raiz->color = NEGRO;
    }

    void preOrdenHelper(NodoRB* nodo) const {
        if (nodo != TNULL) {
            cout << nodo->dato << (nodo->color == ROJO ? "(R) " : "(N) ");
            preOrdenHelper(nodo->izq);
            preOrdenHelper(nodo->der);
        }
    }

    int alturaHelper(NodoRB* nodo) const {
        if (nodo == TNULL) return -1;
        return 1 + max(alturaHelper(nodo->izq), alturaHelper(nodo->der));
    }

    // Verifica propiedades 1, 3, 4 y 5; retorna la altura negra (black-height) o -1 si falla
    int verificarHelper(NodoRB* nodo, bool& valido) const {
        // Propiedad 5: Las hojas nulas son negras (aportan 1 a la altura negra)
        if (nodo == TNULL) {
            if (nodo->color != NEGRO) valido = false;
            return 1;
        }

        // Propiedad 1: Cada nodo es rojo o negro
        if (nodo->color != ROJO && nodo->color != NEGRO) {
            valido = false;
        }

        // Propiedad 3: Ningún nodo rojo puede tener hijo rojo
        if (nodo->color == ROJO) {
            if (nodo->izq->color == ROJO || nodo->der->color == ROJO) {
                valido = false;
            }
        }

        int bhIzq = verificarHelper(nodo->izq, valido);
        int bhDer = verificarHelper(nodo->der, valido);

        // Propiedad 4: Mismo número de nodos negros en todo camino hacia hojas nulas
        if (bhIzq != bhDer) {
            valido = false;
        }

        return bhIzq + (nodo->color == NEGRO ? 1 : 0);
    }

    void destruirHelper(NodoRB* nodo) {
        if (nodo != TNULL) {
            destruirHelper(nodo->izq);
            destruirHelper(nodo->der);
            delete nodo;
        }
    }

public:
    RedBlackTree() {
        TNULL = new NodoRB(0);
        TNULL->color = NEGRO;
        TNULL->izq = nullptr;
        TNULL->der = nullptr;
        raiz = TNULL;
    }

    ~RedBlackTree() {
        destruirHelper(raiz);
        delete TNULL;
    }

    void insertar(int val) {
        NodoRB* nuevo = new NodoRB(val);
        nuevo->izq = TNULL;
        nuevo->der = TNULL;

        NodoRB* y = nullptr;
        NodoRB* x = raiz;

        while (x != TNULL) {
            y = x;
            if (nuevo->dato < x->dato) {
                x = x->izq;
            } else if (nuevo->dato > x->dato) {
                x = x->der;
            } else {
                delete nuevo; // Evitar duplicados
                return;
            }
        }

        nuevo->padre = y;
        if (y == nullptr) {
            raiz = nuevo;
        } else if (nuevo->dato < y->dato) {
            y->izq = nuevo;
        } else {
            y->der = nuevo;
        }

        if (nuevo->padre == nullptr) {
            nuevo->color = NEGRO;
            return;
        }

        if (nuevo->padre->padre == nullptr) {
            return;
        }

        corregirInsercion(nuevo);
    }

    bool verificarPropiedades() const {
        if (raiz == TNULL) return true;
        // Propiedad 2: La raíz es negra
        if (raiz->color != NEGRO) return false;

        bool valido = true;
        verificarHelper(raiz, valido);
        return valido;
    }

    int calcularAltura() const {
        return alturaHelper(raiz);
    }

    void imprimirPreOrden() const {
        cout << "Pre-Orden RB-Tree: ";
        preOrdenHelper(raiz);
        cout << "\n";
    }
};

/*
================================================================================
DEMOSTRACIÓN MATEMÁTICA DE QUE LA ALTURA DE UN RED-BLACK TREE ES O(log n):
================================================================================
Definición: Sea bh(x) la "altura negra" (black-height) de un nodo x, definida 
como el número de nodos negros en cualquier camino simple desde x (sin incluirlo) 
hasta una hoja nula TNULL (incluyéndola). Por la Propiedad 4, bh(x) está bien 
definida.

Lema: El subárbol con raíz en cualquier nodo x contiene al menos 2^{bh(x)} - 1 
nodos internos.

Demostración del Lema (por inducción sobre la altura de x):
- Caso base: Si la altura de x es 0, entonces x es una hoja nula TNULL, 
  bh(x) = 0, y el subárbol tiene 2^0 - 1 = 0 nodos internos. Se cumple.
- Paso inductivo: Considere un nodo interno x con altura h > 0 y dos hijos 
  (pueden ser TNULL). Cada hijo tiene altura negra bh(x) (si el hijo es rojo) 
  o bh(x) - 1 (si el hijo es negro). Como la altura de cada hijo es menor que 
  la de x, por hipótesis inductiva cada hijo tiene al menos 2^{bh(x) - 1} - 1 
  nodos internos. Así, el subárbol con raíz x tiene al menos:
      (2^{bh(x) - 1} - 1) + (2^{bh(x) - 1} - 1) + 1 = 2^{bh(x)} - 1 nodos internos.

Cota de la altura total h:
1. Por la Propiedad 3 (ningún nodo rojo tiene hijo rojo), en cualquier camino 
   simple desde la raíz hasta una hoja, al menos la mitad de los nodos (excluyendo 
   la raíz) deben ser negros. Por lo tanto:
       bh(raiz) >= h / 2

2. Aplicando el Lema a la raíz de un árbol con n nodos internos:
       n >= 2^{bh(raiz)} - 1 >= 2^{h / 2} - 1

3. Despejando h:
       n + 1 >= 2^{h / 2}
       log_2(n + 1) >= h / 2
       h <= 2 * log_2(n + 1)

Conclusión: La altura h está estrictamente acotada superiormente por 
2 * log_2(n + 1), lo que demuestra que h = O(log n).
================================================================================
*/

int main() {
    RedBlackTree rbt;

    // Insertar 20 elementos en orden ascendente (1 al 20)
    for (int i = 1; i <= 20; ++i) {
        rbt.insertar(i);
    }

    cout << "EVALUACION DEL RED-BLACK TREE (1..20):\n";
    cout << "Cumple las 5 propiedades formales?: " 
              << (rbt.verificarPropiedades() ? "SI (Valido)" : "NO (Invalido)") << "\n";
    
    int alturaRBT = rbt.calcularAltura();
    int alturaBST = 19; // Para n=20 insertados ordenadamente en BST: h = n - 1 = 19

    cout << "Altura del Red-Black Tree: " << alturaRBT << "\n";
    cout << "Altura del BST (Problema 1) con mismos datos ordenados: " << alturaBST << "\n";
    cout << "Reduccion de altura: " << (alturaBST - alturaRBT) << " niveles menos.\n\n";

    // Imprimir recorrido Pre-Orden para graficar el árbol
    rbt.imprimirPreOrden();

    return 0;
}