# Descripcion del problema

En esta entrega se busca implementar la solucion de la anterior pero de manera 
paralela en vez de serial. En mi caso, para implementar la soluciòn concurrente
usè mapeo estàtico, asignàndole a cada hilo una porciòn de la memoria 
compartida.

Midiendo ambas soluciones (la serial y la concurrente), la concurrente resulta
màs ràpida. Lo cual es lo esperado porque el trabajo està siendo divido
y manejado al mismo tiempo por varios trabajadores.

---

## Manual de uso

Se proveeran el ejemplo de como correr con perf y el corrido basico del programa
Correr desde /concurrente24a-jahir_valverde/tareas/goldbach_serial

### Perf

`sudo perf stat make test ARGS=8 TESTS=tests_large`
`sudo make clean`

### Corrido normal

`make clean && make && ./bin/goldbach_optimized 8 < tests_large/input020.txt`

---

## Creditos

Jahir Valverde - `jahir.valverde@ucr.ac.cr`

---

## Design
[Design: ./design/README.md](./design/README.md)

## Documento de Reporte
[Design: ./report/readme.md](./report/readme.md)