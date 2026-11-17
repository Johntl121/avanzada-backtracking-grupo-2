#include <iostream>
#include <chrono>

using namespace std;
using namespace std::chrono;

#define MAXCANDIDATES 2

// Métricas de complejidad
long long llamadas_con_poda = 0;
long long soluciones_con_poda = 0;

// Estructura de datos con punteros
struct Data {
    int* elementos;   // Arreglo {1, 2, ..., n}
    int* sufijo_suma; // Suma acumulada de elementos restantes
    int n;            // Tamaño del conjunto
    int objetivo;     // Suma objetivo
    bool mostrar;     // Flag para imprimir en pantalla
};

// Calcula la suma acumulada actual de la rama
int calcular_suma_actual(int a[], int k, const Data* input) {
    int suma = 0;
    for (int i = 1; i <= k; i++) {
        if (a[i] == 1) {
            suma += input->elementos[i - 1];
        }
    }
    return suma;
}

// Comprueba si la rama actual alcanzó exactamente la suma buscada
bool is_a_solution(int a[], int k, const Data* input) {
    return (calcular_suma_actual(a, k, input) == input->objetivo);
}

// Procesa la solución si coincide con el objetivo
void process_solution(int a[], int k, const Data* input) {
    soluciones_con_poda++;
    int suma = calcular_suma_actual(a, k, input);

    if (input->mostrar && soluciones_con_poda <= 10) {
        cout << "Solucion " << soluciones_con_poda << ": { ";
        for (int i = 1; i <= k; i++) {
            if (a[i] == 1) cout << input->elementos[i - 1] << " ";
        }
        cout << "} -> Suma: " << suma << "\n";
    }
}

// CON PODA: Genera candidatos evaluando cotas en O(1)
void construct_candidates(int a[], int k, const Data* input, int c[], int* ncandidates) {
    *ncandidates = 0;

    int suma_actual = calcular_suma_actual(a, k - 1, input);
    int elem = input->elementos[k - 1];
    int suma_restante = (k <= input->n) ? input->sufijo_suma[k - 1] : 0;

    // PODA 1 (Por exceso): Si al sumar el elemento supera el objetivo, no generamos candidato 1
    if (suma_actual + elem <= input->objetivo) {
        c[*ncandidates] = 1; // Incluir
        (*ncandidates)++;
    }

    // PODA 2 (Por defecto): Si sin este elemento lo que queda no alcanza el objetivo, no generamos candidato 0
    if (suma_actual + (suma_restante - elem) >= input->objetivo) {
        c[*ncandidates] = 0; // Descartar
        (*ncandidates)++;
    }
}

// Algoritmo general de Backtracking con poda (Skiena & Revilla)
void backtrack(int a[], int k, const Data* input) {
    llamadas_con_poda++;

    int c[MAXCANDIDATES];
    int ncandidates;

    // Si ya completamos la suma, procesamos y podas el resto del subárbol
    if (is_a_solution(a, k, input)) {
        process_solution(a, k, input);
        return;
    }

    // Si llegamos al final del conjunto sin alcanzar el objetivo, retrocedemos
    if (k == input->n) {
        return;
    }

    k = k + 1;
    construct_candidates(a, k, input, c, &ncandidates);
    for (int i = 0; i < ncandidates; i++) {
        a[k] = c[i];
        backtrack(a, k, input);
    }
}

int main() {
    int n, objetivo;

    cout << "===============================================\n";
    cout << "   SUBCONJUNTOS CON PODA (OPTIMIZADO)\n";
    cout << "===============================================\n";
    cout << "Ingrese n (conjunto {1, 2, ..., n}): ";
    if (!(cin >> n) || n <= 0) n = 8;

    cout << "Ingrese la suma objetivo: ";
    if (!(cin >> objetivo) || objetivo <= 0) objetivo = (n * (n + 1)) / 4;

    // Asignación de memoria dinámica mediante punteros
    Data input;
    input.n = n;
    input.objetivo = objetivo;
    input.mostrar = true;
    input.elementos = new int[n];
    input.sufijo_suma = new int[n];

    // Asume el conjunto {1, 2, ..., n}
    for (int i = 0; i < n; i++) {
        input.elementos[i] = i + 1;
    }

    // Precalculo de sumas sufijas para poda por defecto en O(1)
    input.sufijo_suma[n - 1] = input.elementos[n - 1];
    for (int i = n - 2; i >= 0; i--) {
        input.sufijo_suma[i] = input.sufijo_suma[i + 1] + input.elementos[i];
    }

    // a[1..n] para guardar decisiones
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

    if (soluciones_con_poda > 10) {
        cout << "... (se omitieron las demas soluciones en consola para no sesgar el tiempo)\n";
    }

    cout << "\n--------------- METRICAS ----------------\n";
    cout << "Soluciones validas      : " << soluciones_con_poda << "\n";
    cout << "Llamadas recursivas (Nodos): " << llamadas_con_poda << "\n";
    cout << "Tiempo de ejecucion     : " << duracion_us << " microsegundos (" << duracion_ms << " ms)\n";
    cout << "-----------------------------------------\n";

    // Liberación estricta de memoria (evita memory leaks)
    delete[] input.elementos;
    delete[] input.sufijo_suma;
    delete[] a;

    return 0;
}
