En 1742 Christian Goldbach de Prusia le escribió una carta al matemático suizo
Leonhard Euler con una conjetura:

### Todo número entero mayor que 5:
1. Si es par se puede escribir como suma de dos números primos 
(conjetura fuerte). Por ejemplo: 6=3+3, 8=3+5, 14=3+11=7+7.
2. Si es impar se puede escribir como suma de tres números primos 
(conjetura débil). Por ejemplo.: 7=2+2+3, 9=2+2+5, 9=3+3+3.

Euler no pudo demostrar la conjetura, ni tampoco cientos de matemáticos
convirtiéndolo en el problema abierto más intrigante en la historia de la
teoría de números. Algunas novelas y películas hacen alusión a la conjetura,
y entre los años 2000 y 2002 se ofreció un premio de un millón de dólares 
a quien lograse demostrarla, premio que no fue reclamado. En el 2013, tras 
271 años de su aparición, el matemático peruano Harald Helfgott propuso una 
demostración para la conjetura débil que está actualmente sometida a 
revisión de pares, y en caso de validarse convertiría la conjetura débil 
en teorema.

Para esta entrega se necesita un programa procedimental en C que reciba una 
lista de números enteros en la entrada estándar y calcule todas las sumas de
Goldbach que equivalen a cada número, de manera serial. Su programa producirá 
como resultado en la salida estándar un resumen de cuántos números fueron 
ingresados y cuántas sumas en total se encontraron.

Luego imprimirá una lista con los números en el el mismo orden en que fueron 
ingresados. Cada línea de la lista contiene un número ingresado seguido por la 
cantidad de sumas de Goldbach que tiene. Si el número ingresado es negativo, 
se considerará como que el usuario quiere que el programa además liste todas 
las sumas del correspondiente número positivo. A continuación se muestra un 
ejemplo de la salida esperada para na entrada dada.

### Ejemplo de entrada:
1
2
6
7
-8
-9
10
11
12
13
-14
-21

### Salida esperada:
Total: 12 numbers 19 sums

1: NA
2: NA
6: 1 sums
7: 1 sums
-8: 1 sums: 3 + 5
-9: 2 sums: 2 + 2 + 5, 3 + 3 + 3
10: 2 sums
11: 2 sums
12: 1 sums
13: 2 sums
-14: 2 sums: 3 + 11, 7 + 7
-21: 5 sums: 2 + 2 + 17, 3 + 5 + 13, 3 + 7 + 11, 5 + 5 + 11, 7 + 7 + 7

Note que las sumas deben estar ordenadas por sus valores, del menor a mayor.
Su programa debe poder calcular números de 64 bits con signo, es decir,
entre 0 y 2^63-1 . Si se provee un número fuera de este rango, el programa
debe imprimir el texto "NA" en la salida estándar. Si se provee un texto
que no sea un número, el programa debe reportar el mensaje "invalid input"
en el error estándar, y terminar su ejecución. Es decir, no continuar 
procesando más números.
