# Backtracking: Subconjuntos con y sin Poda

Implementación del problema de la **Suma de Subconjuntos (*Subset Sum*)** en C++ bajo el esquema canónico de Steven S. Skiena & Miguel A. Revilla (*Programming Challenges*, Cap. 8).

## Características de la Implementación
- **Sin `std::vector`:** Implementado exclusivamente con **arreglos nativos y punteros**.
- **Gestión eficiente de memoria:** Uso estricto de `new` y `delete[]` libre de fugas de memoria (*memory leaks*).
- **Entrada dinámica de $N$:** Genera el conjunto $S = \{1, 2, 3, \dots, n\}$.
- **Medición de tiempo:** Medición de alta precisión con `<chrono>` (microsegundos y milisegundos).

## Archivos del Repositorio

1. **[subconjuntos_sin_poda.cpp](file:///c:/Users/Jeferson/Desktop/avanzada-backtracking-grupo-2/subconjuntos_sin_poda.cpp)**:
   - Permite ingresar $N$ y la suma objetivo por consola.
   - Genera candidatos incondicionales $\{1, 0\}$, explorando el árbol completo de $2^{n+1}-1$ nodos.
   - Mide llamadas recursivas y tiempo de respuesta.

2. **[subconjuntos_con_poda.cpp](file:///c:/Users/Jeferson/Desktop/avanzada-backtracking-grupo-2/subconjuntos_con_poda.cpp)**:
   - Permite ingresar $N$ y la suma objetivo por consola.
   - Aplica poda por exceso y por defecto en `construct_candidates` en $\mathcal{O}(1)$.
   - Detiene la exploración al alcanzar el objetivo.

3. **[comparacion.cpp](file:///c:/Users/Jeferson/Desktop/avanzada-backtracking-grupo-2/comparacion.cpp)**:
   - Batería de pruebas automatizada para valores crecientes de $N$ ($10, 14, 18, 20, 22, 24$).
   - Genera directamente la tabla comparativa de nodos explorados, tiempos y porcentaje de reducción para el informe.
