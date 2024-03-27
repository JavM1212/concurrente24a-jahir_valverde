# Analisis

---

## Descripcion del problema
Se desea crear un programa que sea capaz de recibir numeros enteros de entrada,
y hallarle las sumas de Goldbach segun la conjetura de Goldbach. La cual 
dice que:


###### Todo número entero mayor que 5:
1. Si es par se puede escribir como suma de dos números primos (conjetura 
fuerte). Por ejemplo: 6=3+3, 8=3+5, 14=3+11=7+7.
2. Si es impar se puede escribir como suma de tres números primos (conjetura
débil). Por ejemplo.: 7=2+2+3, 9=2+2+5, 9=3+3+3.


La salida debe mostrar los numeros en el mismo orden en que fueron ingresados,
ademas las sumas deben estar ordenadas de menor a mayor.

---

## Manual de uso
Se proveeran ejemplos de como compilar (con y sin sanitizers) y como correr
el programa. 
Correr desde /concurrente24a-jahir_valverde/tareas/goldbach_serial

### Compilado
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

### Corrido
./bin/goldbach_serial < tests/input001.txt

### Inputs
El input debe estar necesariamente en un archivo .txt. Hay un ejemplo en
/concurrente24a-jahir_valverde/tareas/goldbach_serial/tests/input001.txt.
Sin embargo se pueden agregar mas, lo importante es la extension .txt y que 
cada numero este separado por salto de linea.
###### Input Valido (valores dentro de un .txt)
0
4
-1
7
###### Input Invalido
