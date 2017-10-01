all: main
	@echo Compilacion realizada. Ejecutar con ./sec o ./par
main: 
	gcc -g -o sec secuencial.c -lm -fopenmp
	gcc -g -o par paralelo.c -lm -fopenmp -O3
check:
	cppcheck secuencial.c
	cppcheck paralelo.c
clean:
	rm sec
	rm par

