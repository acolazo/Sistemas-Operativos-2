#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>

#define gates 500



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

	FILE * fp;
	int i;
	uint16_t auxiliar;
	struct nodo * actual;
	struct nodo * anterior;

	list.size=0;
	list.first=NULL;
	fp = fopen ("pulsos.iq","rb");
	if (fp==NULL)
	{
		printf("ERROR\n");
		exit(0);
	}

	
	/* Leo pulsos.iq y guardo en cada nodo los datos de un pulso */
	while(fread(&auxiliar, sizeof(uint16_t), 1, fp) == 1)
	{
		actual=allocate_memory();
		actual->valid_samples=auxiliar;
		actual->v = (float *)calloc(2*auxiliar, sizeof(float));
		actual->h = (float *)calloc(2*auxiliar, sizeof(float));
		fread(actual->v, sizeof(float), 2*auxiliar, fp);
		fread(actual->h, sizeof(float), 2*auxiliar, fp);
	}

	/* Armo las matrices de list.size filas (cantidad de pulsos) y 500 columnas (cantidad de gates). */
	/* Aca ya se puede empezar a usar paralelismo y es un viaje*/
	/* Matriz (gate, pulso) */ 
	
	float matrix_h[gates][list.size];
	float matrix_v[gates][list.size];
	int samples_per_gate;
	int j;
	int g;
	int position;
	
	i=0;
	actual = list.first;
	while(actual != NULL)
	{
		samples_per_gate = 0.5*actual->valid_samples/250;
		for(g=0; g<gates; g++)
		{
			matrix_v[g][i]=0;
			matrix_h[g][i]=0;	/* Inicializo en cero */
			/* Media aritmetica */
			for(j=0; j<samples_per_gate; j++)
			{
				position=(g*samples_per_gate)+(j*2);
				matrix_v[g][i]=matrix_v[g][i]+sqrt(pow(actual->v[position],2)+pow(actual->v[position+1],2));
				matrix_h[g][i]=matrix_h[g][i]+sqrt(pow(actual->h[position],2)+pow(actual->h[position+1],2));
			}
			matrix_h[g][i]/=samples_per_gate;
			matrix_v[g][i]/=samples_per_gate;	
			
		}	/* Armo cada gate del nodo i */
			actual = actual->next;
			i++;
		}

	/* Calculo la autocorrelacion de cada matriz */
	/* Autocorrelacion de las distintas matrices */

		float res[gates];
		float res2[gates];
		autocorrelacion(list.size, matrix_h, res);
		autocorrelacion(list.size, matrix_v, res2);
		for(i=0; i<gates; i++)
		{
			printf("Autocorrelacion del gate %d es %f\n", i+1, res[i]);
		}

		printf("\n\n***********Vertical***********\n\n");
		for(i=0; i<gates; i++)
		{
			printf("Autocorrelacion del gate %d es %f\n", i+1, res2[i]);
		}

	
		free_memory();
		fclose (fp);
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

	void autocorrelacion(int size_columnas, float matrix[gates][size_columnas], float * r)
	{
	/* size_columas es la cantidad de pulsos */
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