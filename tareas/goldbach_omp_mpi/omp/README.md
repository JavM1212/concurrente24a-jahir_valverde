# Descripcion del problema

En esta entrega se busca implementar optimizaciones a la solución de la tarea
de Goldbach Numbers. Partiendo desde la tarea01, la primera optimización es en
el cálculo de las sumas de Goldbach. Posteriormente se cuenta la tarea02 como
una optimización ya que se implementan hilos con mapeo por bloque. Luego
se implementa una tercera optmización mapeo dinámico.

Los resultados (duración, speedup y eficiencia) se apuntan, grafican y analizan
en el documento de reporte.

---

## Manual de uso

Se proveeran el ejemplo de como correr con perf y el corrido basico del programa
Correr desde /concurrente24a-jahir_valverde/tareas/goldbach_serial

### Perf

`sudo perf stat make test ARGS=8 TESTS=tests_large`  
`sudo make clean`

### Corrido normal

`make clean && make && ./bin/omp_mpi 8 < tests_large/input020.txt`

---

## Documento de Reporte

[Design: ./report/readme.md](./report/readme.md)

---

## Creditos

Jahir Valverde - `jahir.valverde@ucr.ac.cr`
