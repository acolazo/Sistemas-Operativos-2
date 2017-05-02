#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#define cantidad_datos 17
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

float autocorrelacion(int nro_gate, int size_filas, int size_columnas, float matrix[size_filas][size_columnas]);
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
	float matrix_v_f[gates][list.size];
	float matrix_v_c[gates][list.size];
	float matrix_h_f[gates][list.size];
	float matrix_h_c[gates][list.size];
	int samples_per_gate;
	int j;
	int g;
	
	i=0;
	actual = list.first;
	while(actual != NULL)
	{
		samples_per_gate = 0.5*actual->valid_samples/250;
		for(g=0; g<gates; g++)
		{
			matrix_v_f[g][i]=0;
			matrix_v_c[g][i]=0;
			matrix_h_f[g][i]=0;
			matrix_h_c[g][i]=0;	/* Inicializo en cero */
			for(j=0; j<samples_per_gate; j++)
			{
				matrix_v_f[g][i]+=actual->v[g*samples_per_gate+j*2];
				matrix_v_c[g][i]+=actual->v[g*samples_per_gate+((j*2)+1)];
				matrix_h_f[g][i]+=actual->h[g*samples_per_gate+j*2];
				matrix_h_c[g][i]+=actual->h[g*samples_per_gate+((j*2)+1)];	/* sumatoria */
			}
			matrix_v_f[g][i]/=samples_per_gate;
			matrix_v_c[g][i]/=samples_per_gate;
			matrix_h_f[g][i]/=samples_per_gate;
			matrix_h_c[g][i]/=samples_per_gate;	/* media aritmetica */
		}	/* Armo cada gate del nodo i */
		actual = actual->next;
		i++;
	}

	/* Calculo la autocorrelacion de cada matriz */
	float r; /* Autocorrelacion de las distintas matrices */

	for(i=0; i<gates; i++)
	{
		r=autocorrelacion(i, gates, list.size, matrix_h_c);
		printf("Autocorrelacion del gate %d es %f\n", i+1, r);
	}

	/*Prueba*/
	actual=list.first;
	while(actual->next != NULL)
	{
		printf("%d\n", actual->valid_samples);
		actual=actual->next;
	}

	printf("Numero float es %f\n", /*list.first->v[0]*/matrix_v_f[0][0]);
	/*


	for(i=0; i<gates; i++)
	{


		for(j=0; j<list.size; j++)
		{
			printf("%f ", matrix_h_c[i][j]);
		}
		printf("\n");
	}
	*/
	/*Prueba*/

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

float autocorrelacion(int nro_gate, int size_filas, int size_columnas, float matrix[size_filas][size_columnas])
{
	/* size_columas es la cantidad de pulsos */
	float r;
	int i;

	r=0;
	for(i=0; i<size_columnas-1; i++)
	{
		r+=matrix[nro_gate][i]*matrix[nro_gate][i+1];
	}
	r/=size_columnas;
	return r;
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