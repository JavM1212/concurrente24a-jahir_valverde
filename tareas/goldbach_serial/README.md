# Descripcion del problema
Se desea crear un programa que sea capaz de recibir numeros enteros de entrada,
y hallarle las sumas de Goldbach segun la conjetura de Goldbach. La cual 
dice que:


Todo número entero mayor que 5:
1. Si es par se puede escribir como suma de dos números primos (conjetura 
fuerte). Por ejemplo: 6=3+3, 8=3+5, 14=3+11=7+7
2. Si es impar se puede escribir como suma de tres números primos (conjetura
débil). Por ejemplo.: 7=2+2+3, 9=2+2+5, 9=3+3+3


La salida debe mostrar los numeros en el mismo orden en que fueron ingresados,
ademas las sumas deben estar ordenadas de menor a mayor

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

## Corrido
./bin/goldbach_serial < tests/input001.txt

## Inputs
El input debe estar necesariamente en un archivo .txt. Hay un ejemplo en
/concurrente24a-jahir_valverde/tareas/goldbach_serial/tests/input001.txt.
Sin embargo se pueden agregar mas, lo importante es la extension .txt y que 
cada numero este separado por salto de linea
###### Input Valido (valores dentro de un .txt)
0
3
-4
8
11
-14
-15
###### Salida en Input Valido
Total: 7 numbers 8 sums

0: NA
3: NA
-4: NA
8: 1 sums
11: 2 sums
-14: 2 sums: 3 + 11, 7 + 7
-15: 3 sums: 2 + 2 + 11, 3 + 5 + 7, 5 + 5 + 5
###### Input Invalido (algo diferente a int)
f
4.3
ho
###### Salida en Input Invalido
Error: invalid input. errno: 21
***El codigo 21 significa que se cayo en la rutina 2 con identificador 1***

## Errores
Como se usa la variable global errno, y esta se popula automaticamente en 
algunos contextos, decidi seguir sete patron: Para errores de inicializacion
(callocs, reallocs, etc), no modifico la variable errno, esperando que se
se modifique automaticamente. Para otros errores (como entradas invalidas), 
setteo errno = {numero de rutina}{numero de error}. Por ejemplo, el segundo
error de la tercera rutina, implica errno = 32

---

# Creditos
Jahir Valverde - jahir.valverde@ucr.ac.cr 

---

# Design
[Design README.md] (/design/README.md)