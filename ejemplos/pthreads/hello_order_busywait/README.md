# hello_order_busywait
Haga que los threads saluden siempre en orden. Es decir, si se crean w threads,
la salida sea siempre en orden


Hello from thread 0 of w
Hello from thread 1 of w
Hello from thread 2 of w
...
Hello from thread w of w
Utilice espera activa como mecanismo de sincronización (control de 
concurrencia).


Ejecute varias veces su solución de hello_order_busywait con la cantidad máxima
de threads que el su sistema operativo le permite. ¿Es consistente la duración
de su solución con espera activa?


Describa el comportamiento del programa y explique la razón de este 
comportamiento en el archivo readme.md dentro de su carpeta 
hello_order_busywait. Indique en qué condiciones sería apremiante 
utilizar espera activa, o en caso de no haberlas, sugiera soluciones 
alternativas.


## Comportamiento del programa
En mi caso el programa corre un maximo de 32000 hilos con una duracion
promedio aproximada de 2.5 

El programa ahora va a imprimir los hilos en orden. Esto se debe a que se 
implemetna el concepto de espera activa. En la que gracias a la memoria 
compartida, los hilos pueden saber cual es el hilo que sigue, comparando
este dato con el thread number en la memoria privada. Una vez que pasa
la espera activa, modifica la memoria compartida para que otro hilo pueda
"escuchar" el cambio y ejecutarse


La espera activa puede generar problemas si se presentan eventos con mayor
prioridad, por lo que es bueno usarla cuando es necesario (cuando el orden de
ejecucion importa) y no en todos los contextos