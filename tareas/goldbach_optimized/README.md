# Descripcion del problema
En esta entrega se busca implementar la solucion de la anterior pero de manera 
paralela en vez de serial. En mi caso, para implementar la soluciòn concurrente
usè mapeo estàtico, asignàndole a cada hilo una porciòn de la memoria 
compartida.

Midiendo ambas soluciones (la serial y la concurrente), la concurrente resulta
màs ràpida. Lo cual es lo esperado porque el trabajo està siendo divido
y manejado al mismo tiempo por varios trabajadores.

---

# Manual de uso
Se proveeran ejemplos de como compilar (con y sin sanitizers) y como correr
el programa
Correr desde /concurrente24a-jahir_valverde/tareas/goldbach_serial

## Compilado
###### Compilacion Basica
  make
###### Compilacion Addess Sanitizer
  make asan
###### Compilacion Memory Sanitizer
  make msan
###### Compilacion Threads Sanitizer
  make tsan
###### Compilacion Undefined Behavior Sanitizer
  make ubsan
###### Limpiar el compilado
  make clean

## Corrido con el maximo de nucleos de la computadora como la cantidad de hilos
./bin/goldbach_pthread < tests/ex_input.txt

## Corrido con n hilos
./bin/goldbach_pthread n < tests/ex_input.txt

## Inputs
El input debe estar necesariamente en un archivo .txt. Hay un ejemplo en
/concurrente24a-jahir_valverde/tareas/goldbach_serial/tests/ex_input.txt.
Sin embargo se pueden agregar mas, lo importante es la extension .txt y que 
cada numero este separado por salto de linea
###### Input Valido (valores dentro de un .txt)
0\
3\
-4\
8\
11\
-14\
-15\
###### Salida en Input Valido
Total: 7 numbers 8 sums\
\
0: NA\
3: NA\
-4: NA\
8: 1 sums\
11: 2 sums\
-14: 2 sums: 3 + 11, 7 + 7\
-15: 3 sums: 2 + 2 + 11, 3 + 5 + 7, 5 + 5 + 5\
###### Input Invalido (algo diferente a int)
f\
4.3\
ho\
###### Salida en Input Invalido
Error: invalid input. errno: 21
(***El codigo 21 significa que se cayo en la rutina 2 con identificador 1***)

## Errores
Como se usa la variable global errno, y esta se popula automaticamente en 
algunos contextos, decidi seguir sete patron: Para errores de inicializacion
(callocs, reallocs, etc), no modifico la variable errno, esperando que se
se modifique automaticamente. Para otros errores (como entradas invalidas), 
setteo errno = {numero de rutina}{numero de error}. Por ejemplo, el segundo
error de la tercera rutina, implica errno = 32


Ademas, cada subrutina se encarga de imprimir su error, y devolverle al
main el codigo. Luego el main devuelve el error al SO

---

# Creditos
Jahir Valverde - jahir.valverde@ucr.ac.cr 

---

# Design
[Design: ./design/README.md](./design/README.md)

# Documento de Reporte
[Design: ./report/readme.md](./report/readme.md)