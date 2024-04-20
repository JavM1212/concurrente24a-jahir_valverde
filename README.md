# Glosario
1. Programación serial -> Cada instruccion se ejecuta solo cuando la pasada
ya termino
---
2. Programación concurrente -> Lo contrario a programacion serial. Puede
ser paralela, o simplemente a destiempo
---
3. Programación paralela -> Se ejecutan varios procedimientos a la vez
--- 
4. Concurrencia de tareas -> Es una tecnica utilizada para encargarse de 
distintos asuntos concurrentemente
---
5. Paralelismo de datos -> Es una tecnica utilizado para analizar varios datos
concurrentemente, promoviendo el incremento de desempeno
---
6. Hilo de ejecución -> Arreglo de valores que el sistema operativo carga en 
los registros de la CPU y permiten ejecutar codigo
---
7. Indeterminismo -> El indeterminismo es una caracteristica de la concurrencia
, en la cual no se puede predecir que va a pasar, aun introduciendo las mismas
entradas
--- 
8. Memoria privada y compartida -> Memoria compartida: se declaran variables
de forma que todos los hilos pueden usar el mismo recurso. Esto tambien implica
que cuando se hace una modificacion en la memoria compartida, todos los demas
hilos pueden "darse cuenta" del cambio
Memoria privada: En cambio, en la memoria privada, cada hilo tiene su registro
privado (como una copia) y solo puede modificar lo que le corresponde. Es decir,
si un valor cambia, los demas hilos "no se van a dar cuenta"
---
9. Espera activa -> La espera activa (busy waiting) es un ciclo que hace a un
hilo de ejecución esperar repetitivamente hasta que una condición se haga falsa.
Por ejemplo, si la variable next_thread indica el número del próximo thread que 
puede realizar una tarea que debe ser serializada en orden, el código
---
10. Condición de carrera -> Modificacion concurrente de memoria compartida
---
11. Control de concurrencia -> Tecnicas de programacion concurrente que 
permiten controlar los hilos para evitar condiciones de carrera. Por ejemplo
el mutex, semaforos, espera activa, etc
---
12. Seguridad condicional -> La seguridad condicional permite asignarle a cada
hilo una porcion de la data con la que trabajar. Esto permite obtener el 
maximo potencial de la concurrencia porque los hilos no ocupan esperarse entre
si
---
13. Exclusión mutua -> Propiedad de impedir que varios hilos ejecuten a la misma
vez una region critica
---
14. Semáforo -> Entero con signo con tres caracteristicas:
- Se inicializa en cualquier entero, pero luego solo se permite incrementar
en uno (signal) o decrementar en uno (wait). Esto incluye no poder leer el
valor actual del semaforo
- Cuando un hilo decrementa el semaforo, si el resultado es negativo, 
el hilo es bloqueado, y no puede continuar hasta que otro hilo lo incremente
- Cuando un hilo incrementa un semaforo, si hay otros esperando, uno de ellos
sera desbloqueado. Tanto el que incrementa como el que es desbloqueado 
continuan concurrentemente
No hay forma de saber cual hilo sera desbloqueado por el scheduler. Ademas, 
tampoco se puede saber si al incrementar un semaforo, se desbloqueara o no un
hilo en espera, dado que no se puede leer el valor del semaforo
---
15. Barrera
---
16. Variable de condición
---
17. Candado de lectura y escritura
---
18. Descomposición
---
19. Mapeo
---
20. Incremento de velocidad
---
21. Comunicación punto a punto entre procesos
---
22. Comunicación colectiva entre procesos
---
23. Reducción
---