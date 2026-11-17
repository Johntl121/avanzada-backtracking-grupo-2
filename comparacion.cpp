#include <iostream>
#include <chrono>
#include <iomanip>

using namespace std;
using namespace std::chrono;

#define MAXCANDIDATES 2

long long llamadas_sin_poda = 0;
long long soluciones_sin_poda = 0;
long long llamadas_con_poda = 0;
long long soluciones_con_poda = 0;

struct Data {
    int* elementos;
    int* sufijo_suma;
    int n;
    int objetivo;
};

// =============================================================
// 1. VERSION SIN PODA (FUERZA BRUTA)
// =============================================================
bool is_solution_sin_poda(int a[], int k, const Data* in) {
    return (k == in->n);
}

void process_sin_poda(int a[], int k, const Data* in) {
    int suma = 0;
    for (int i = 1; i <= k; i++) {
        if (a[i] == 1) suma += in->elementos[i - 1];
    }
    if (suma == in->objetivo) soluciones_sin_poda++;
}

void candidates_sin_poda(int a[], int k, const Data* in, int c[], int* ncand) {
    c[0] = 1;
    c[1] = 0;
    *ncand = 2;
}

void backtrack_sin_poda(int a[], int k, const Data* in) {
    llamadas_sin_poda++;
    int c[MAXCANDIDATES];
    int ncand;

    if (is_solution_sin_poda(a, k, in)) {
        process_sin_poda(a, k, in);
    } else {
        k = k + 1;
        candidates_sin_poda(a, k, in, c, &ncand);
        for (int i = 0; i < ncand; i++) {
            a[k] = c[i];
            backtrack_sin_poda(a, k, in);
        }
    }
}

// =============================================================
// 2. VERSION CON PODA (OPTIMIZADO)
// =============================================================
int sum_actual(int a[], int k, const Data* in) {
    int s = 0;
    for (int i = 1; i <= k; i++) {
        if (a[i] == 1) s += in->elementos[i - 1];
    }
    return s;
}

void candidates_con_poda(int a[], int k, const Data* in, int c[], int* ncand) {
    *ncand = 0;
    int s_act = sum_actual(a, k - 1, in);
    int elem = in->elementos[k - 1];
    int s_rest = (k <= in->n) ? in->sufijo_suma[k - 1] : 0;

    // Poda por exceso
    if (s_act + elem <= in->objetivo) {
        c[*ncand] = 1;
        (*ncand)++;
    }
    // Poda por defecto
    if (s_act + (s_rest - elem) >= in->objetivo) {
        c[*ncand] = 0;
        (*ncand)++;
    }
}

void backtrack_con_poda(int a[], int k, const Data* in) {
    llamadas_con_poda++;
    int c[MAXCANDIDATES];
    int ncand;

    int s_act = sum_actual(a, k, in);
    if (s_act == in->objetivo) {
        soluciones_con_poda++;
        return; // Poda del subárbol restante
    }

    if (k == in->n) return;

    k = k + 1;
    candidates_con_poda(a, k, in, c, &ncand);
    for (int i = 0; i < ncand; i++) {
        a[k] = c[i];
        backtrack_con_poda(a, k, in);
    }
}

// =============================================================
// RUTINA DE EXPERIMENTACIÓN Y BENCHMARK PARA EL INFORME
// =============================================================
void ejecutar_prueba(int n) {
    int objetivo = (n * (n + 1)) / 4; // Objetivo proporcional a n

    // Gestión dinámica de memoria
    Data in;
    in.n = n;
    in.objetivo = objetivo;
    in.elementos = new int[n];
    in.sufijo_suma = new int[n];

    for (int i = 0; i < n; i++) in.elementos[i] = i + 1;

    in.sufijo_suma[n - 1] = in.elementos[n - 1];
    for (int i = n - 2; i >= 0; i--) {
        in.sufijo_suma[i] = in.sufijo_suma[i + 1] + in.elementos[i];
    }

    int* a1 = new int[n + 1]();
    int* a2 = new int[n + 1]();

    llamadas_sin_poda = 0;
    soluciones_sin_poda = 0;
    llamadas_con_poda = 0;
    soluciones_con_poda = 0;

    // 1. Medir Sin Poda
    auto t1_ini = high_resolution_clock::now();
    backtrack_sin_poda(a1, 0, &in);
    auto t1_fin = high_resolution_clock::now();
    double t_sin_ms = duration<double, milli>(t1_fin - t1_ini).count();

    // 2. Medir Con Poda
    auto t2_ini = high_resolution_clock::now();
    backtrack_con_poda(a2, 0, &in);
    auto t2_fin = high_resolution_clock::now();
    double t_con_ms = duration<double, milli>(t2_fin - t2_ini).count();

    double reduccion = (1.0 - (double)llamadas_con_poda / llamadas_sin_poda) * 100.0;
    double speedup = (t_con_ms > 0.0001) ? (t_sin_ms / t_con_ms) : 1.0;

    cout << left << setw(4) << n
         << setw(8) << objetivo
         << setw(16) << llamadas_sin_poda
         << setw(14) << llamadas_con_poda
         << fixed << setprecision(3) << setw(14) << t_sin_ms
         << fixed << setprecision(3) << setw(14) << t_con_ms
         << fixed << setprecision(1) << setw(12) << reduccion << "%\n";

    // Liberación estricta de memoria (cero leaks)
    delete[] in.elementos;
    delete[] in.sufijo_suma;
    delete[] a1;
    delete[] a2;
}

int main() {
    cout << "========================================================================================\n";
    cout << "          BATERIA DE PRUEBAS DE COMPLEJIDAD Y RENDIMIENTO (INFORME)\n";
    cout << "========================================================================================\n";
    cout << "Conjunto base para cada N: S = {1, 2, 3, ..., N}\n";
    cout << "Objetivo dinamico        : K = (N * (N + 1)) / 4\n\n";

    cout << left << setw(4) << "N"
         << setw(8) << "Obj"
         << setw(16) << "Nodos (Sin)"
         << setw(14) << "Nodos (Con)"
         << setw(14) << "Tiempo Sin(ms)"
         << setw(14) << "Tiempo Con(ms)"
         << setw(12) << "% Reduccion" << "\n";
    cout << string(88, '-') << "\n";

    // Pruebas progresivas para observar la explosión exponencial
    int valores_n[] = {10, 14, 18, 20, 22, 24};
    int total_pruebas = sizeof(valores_n) / sizeof(valores_n[0]);

    for (int i = 0; i < total_pruebas; i++) {
        ejecutar_prueba(valores_n[i]);
    }

    cout << "========================================================================================\n";
    cout << "Nota: Observe como para N >= 22 el tiempo 'Sin Poda' se dispara exponencialmente, \n";
    cout << "      mientras que 'Con Poda' se mantiene en el orden de milisegundos.\n";

    return 0;
}
