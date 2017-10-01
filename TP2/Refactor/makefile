all: main
	@echo Compilacion realizada. Ejecutar con ./sec o ./par
main: 
	gcc -g -std=gnu99 -Wall -pedantic -Werror -o sec secuencial.c -lm -fopenmp
	gcc -g -std=gnu99 -Wall -pedantic -Werror -o par paralelo.c -lm -fopenmp -O3
check:
	cppcheck secuencial.c
	cppcheck paralelo.c
clean:
	rm sec
	rm par

