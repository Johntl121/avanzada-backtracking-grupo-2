#include <iostream>
#include <chrono>

using namespace std;
using namespace std::chrono;

#define MAXCANDIDATES 2

// Métricas de complejidad
long long llamadas_sin_poda = 0;
long long soluciones_sin_poda = 0;

// Estructura de datos con punteros
struct Data {
    int* elementos; // Arreglo {1, 2, ..., n}
    int n;          // Tamaño del conjunto
    int objetivo;   // Suma objetivo
    bool mostrar;   // Flag para imprimir soluciones en pantalla
};

// Comprueba si se llegó a una hoja del árbol (decisión tomada para cada elemento)
bool is_a_solution(int a[], int k, const Data* input) {
    return (k == input->n);
}

// Procesa la solución si la suma de los elementos seleccionados coincide con el objetivo
void process_solution(int a[], int k, const Data* input) {
    int suma = 0;
    for (int i = 1; i <= k; i++) {
        if (a[i] == 1) {
            suma += input->elementos[i - 1];
        }
    }

    if (suma == input->objetivo) {
        soluciones_sin_poda++;
        if (input->mostrar && soluciones_sin_poda <= 10) {
            cout << "Solucion " << soluciones_sin_poda << ": { ";
            for (int i = 1; i <= k; i++) {
                if (a[i] == 1) cout << input->elementos[i - 1] << " ";
            }
            cout << "} -> Suma: " << suma << "\n";
        }
    }
}

// SIN PODA: Siempre genera los dos candidatos (1 = incluir, 0 = descartar)
void construct_candidates(int a[], int k, const Data* input, int c[], int* ncandidates) {
    c[0] = 1;
    c[1] = 0;
    *ncandidates = 2;
}

// Algoritmo general de Backtracking (Skiena & Revilla)
void backtrack(int a[], int k, const Data* input) {
    llamadas_sin_poda++;

    int c[MAXCANDIDATES];
    int ncandidates;

    if (is_a_solution(a, k, input)) {
        process_solution(a, k, input);
    } else {
        k = k + 1;
        construct_candidates(a, k, input, c, &ncandidates);
        for (int i = 0; i < ncandidates; i++) {
            a[k] = c[i];
            backtrack(a, k, input);
        }
    }
}

int main() {
    int n, objetivo;

    cout << "===============================================\n";
    cout << "   SUBCONJUNTOS SIN PODA (FUERZA BRUTA)\n";
    cout << "===============================================\n";
    cout << "Ingrese n (conjunto {1, 2, ..., n}): ";
    if (!(cin >> n) || n <= 0) n = 8;

    cout << "Ingrese la suma objetivo: ";
    if (!(cin >> objetivo) || objetivo <= 0) objetivo = (n * (n + 1)) / 4;

    // Asignación dinámica de memoria con punteros
    Data input;
    input.n = n;
    input.objetivo = objetivo;
    input.mostrar = true;
    input.elementos = new int[n];

    // Asume el conjunto {1, 2, 3, ..., n}
    for (int i = 0; i < n; i++) {
        input.elementos[i] = i + 1;
    }

    // Vector de soluciones a[1..n]
    int* a = new int[n + 1]();

    cout << "\nConjunto S = { ";
    for (int i = 0; i < n; i++) cout << input.elementos[i] << " ";
    cout << "}\nObjetivo = " << input.objetivo << "\n\n";

    // Medición de tiempo de alta precisión con <chrono>
    auto t_inicio = high_resolution_clock::now();
    backtrack(a, 0, &input);
    auto t_fin = high_resolution_clock::now();

    auto duracion_us = duration_cast<microseconds>(t_fin - t_inicio).count();
    double duracion_ms = duration<double, milli>(t_fin - t_inicio).count();

    if (soluciones_sin_poda > 10) {
        cout << "... (se omitieron las demas soluciones en consola para no sesgar el tiempo)\n";
    }

    cout << "\n--------------- METRICAS ----------------\n";
    cout << "Soluciones validas      : " << soluciones_sin_poda << "\n";
    cout << "Llamadas recursivas (Nodos): " << llamadas_sin_poda << "\n";
    cout << "Tiempo de ejecucion     : " << duracion_us << " microsegundos (" << duracion_ms << " ms)\n";
    cout << "-----------------------------------------\n";

    // Liberación estricta de memoria dinámica
    delete[] input.elementos;
    delete[] a;

    return 0;
}
