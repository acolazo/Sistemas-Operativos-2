#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <omp.h>


#define gates 500
#define resolution 0.5
#define rango 250
#define THREADS 4




struct nodo
{
	//datos
	uint16_t valid_samples;
	float * v;
	/* v[par] son muestras de polaridad vertical en fase */
	/* v[impar] son muestras de polaridad vertical en cuadratura */
	float * h;
	/* h[par] son muestras de polaridad horizontal en fase */
	/* h[impar] son muestras de polaridad hortizontal en cuadratura */
	struct nodo * next;
};
struct lista
{
	struct nodo * first;
	int size;
};
struct lista list;

void autocorrelacion(int size_columnas, float matrix[gates][size_columnas], float * r);
struct nodo * allocate_memory();
void free_memory();

int main (int argc, char ** argv)
{
	double start_time = omp_get_wtime();

	FILE * fp;
	int i;
	uint16_t auxiliar;
	struct nodo * actual;
	


	list.size=0;
	list.first=NULL;
	fp = fopen ("pulsos.iq","rb");
	if (fp==NULL)
	{
		printf("ERROR\n");
		exit(0);
	}

	
	/* Leo pulsos.iq y guardo en cada nodo los datos de un pulso */
	int a;
	int b;
	while(fread(&auxiliar, sizeof(uint16_t), 1, fp) == 1)
	{
		actual=allocate_memory();
		actual->valid_samples=auxiliar;
		actual->v = (float *)calloc(2*auxiliar, sizeof(float));
		actual->h = (float *)calloc(2*auxiliar, sizeof(float));
		a=fread(actual->v, sizeof(float), 2*auxiliar, fp);
		b=fread(actual->h, sizeof(float), 2*auxiliar, fp);
		if(a != 2*auxiliar || b != 2*auxiliar){
			printf("Error leyendo archivo\n");
			exit(0);
		}

	}

	fclose (fp);
	fp=NULL;
	/* Armo las matrices de list.size filas (cantidad de pulsos) y 500 columnas (cantidad de gates). */
	/* Aca ya se puede empezar a usar paralelismo y es un viaje*/
	/* Matriz (gate, pulso) */ 
	
	struct nodo * index [list.size];
	
	int j;
	int g;
	//Doubles
	float matrix_h[gates][list.size];
	float matrix_v[gates][list.size];
	float resv[gates];
	float resh[gates];
	float pow1;
	float pow2;
	float sqr;
	int position;
	int samples_per_gate;
	int cantidad_pulsos;
	/* Armo arreglo de indices que apunta a cada nodo (pulso) */
	i=0;
	actual = list.first;
	while(actual != NULL){ //Secuencial
		index [i] = actual;
		actual = actual->next;
		i++;
	}
	
	omp_set_num_threads(THREADS);
#pragma omp parallel private(i, j, g, pow1, pow2, sqr, position, samples_per_gate, cantidad_pulsos) shared(matrix_v, matrix_h, resv, resh, index)
	{

		cantidad_pulsos = list.size;


#pragma omp for
	for(i=0; i<cantidad_pulsos; i++){ //Paralelizo aca.
		samples_per_gate = resolution * (index[i] -> valid_samples) / rango;
		for(g=0; g<gates; g++){
			matrix_v[g][i] = 0;
			matrix_h[g][i] = 0;/* Inicializo en cero */
			/* Media aritmetica */
			position = g * samples_per_gate * 2; /* el 2 es porque tenemos la parte real y la imaginaria */
			for(j=0; j<samples_per_gate; j++)
			{
				//position=(g*samples_per_gate*2)+(j*2);
				pow1 = index[i]->v[position] * index[i]->v[position];
				pow2 = index[i]->v[position+1] * index[i]->v[position+1];
				sqr = sqrt(pow1 + pow2);
				matrix_v[g][i]+= sqr;

				pow1 = index[i]->h[position] * index[i]->h[position];
				pow2 = index[i]->h[position+1] * index[i]->h[position+1];
				sqr = sqrt(pow1 + pow2);
				matrix_h[g][i]+= sqr;

				position += 2;
			}


			matrix_h[g][i]= matrix_h[g][i] / samples_per_gate;
			matrix_v[g][i]= matrix_v[g][i] / samples_per_gate;
		}	/* Armo cada gate del nodo i */
		}

		/* Calculo la autocorrelacion de cada matriz (cada canal) */
		/* Autocorrelacion de las distintas matrices */
#pragma omp for
		for(i = 0; i < gates; i++) //Aca tambien paralelizo
		{
			resv[i]=0;
			resh[i]=0;
			for(j=0; j<cantidad_pulsos-1; j++)
			{
				resv[i]=resv[i]+(matrix_v[i][j]*matrix_v[i][j+1]);
				resh[i]=resh[i]+(matrix_h[i][j]*matrix_h[i][j+1]);
			}
			resv[i]=resv[i]/cantidad_pulsos;		
			resh[i]=resh[i]/cantidad_pulsos;		
		}
	}
		//autocorrelacion(list.size, matrix_h, res);
		//autocorrelacion(list.size, matrix_v, res2);


		/* Termino la parte paralela */
		/* Termino el calculo de autocorrelacion */

		/* Escribo el archivo de salida */

	FILE * file_to_write = fopen("result_paralelo.iq", "w");
	fwrite(resh, sizeof(float), gates, file_to_write);
	fwrite(resv, sizeof(float), gates, file_to_write);
	fclose(file_to_write);
	file_to_write=NULL;


		/* Imprimir en consola */	
		/* (gate, pulso) */	
		
		/*
		for(i=0; i<gates; i++)
		{
			printf("Autocorrelacion del gate %d es %e\n", i+1, resh[i]);
		}
		*/
		

		/*
		for(i=0; i<list.size; i++){
			printf("%d: %f\n", i, matrix_v[0][i]);
		}
		*/



	free_memory();

	double time = omp_get_wtime() - start_time;
	printf("El tiempo de ejecucion es: %lf segundos\n", time);
	return 0;

}

struct nodo * allocate_memory()
{
	static struct nodo * anterior;
	struct nodo * actual;


	actual = (struct nodo *)malloc(sizeof(struct nodo));	/* Reservo la memoria necesaria */
	if(actual == NULL)
		printf("ERROR AL RESERVAR BLOQUE DE MEMORIA\n");
	if(list.first == NULL)	/* Si no hay elemento en la lista, este sera el primero */
	{
		list.first = actual;
	}
	else					/* Sino el anterior apuntara al nuevo nodo */
	{
		anterior -> next = actual;
	}
	actual->next = NULL;	/* El nuevo nodo apunta a NULL */
	anterior = actual;

	list.size++;			/* Incrementa en uno el tamanio de la lista */
	return actual;
}


void free_memory(){
	struct nodo * first;
	struct nodo * next;
	first=list.first;
	if(first==NULL)
		return;
	next=first->next;
	while(next!=NULL){
		free(first);
		first=next;
		next=first->next;
	}
	free(first);
	list.first=NULL;

	return;
}

/*
void autocorrelacion(int size_columnas, float matrix[gates][size_columnas], float * r)
{
	//size_columas es la cantidad de pulsos 
	int i;
	int j;



	for(i=0; i<gates; i++)
	{
		r[i]=0;
		for(j=0; j<size_columnas-1; j++)
		{
			r[i]=r[i]+(matrix[i][j]*matrix[i][j+1]);
		}
		r[i]=r[i]/size_columnas;		
	}
	return;
}

*/