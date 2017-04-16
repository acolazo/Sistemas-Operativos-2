	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
#ifndef OPENFILE_H
#define OPENFILE_H
#include "openfile.h"
#endif



	//https://www.tutorialspoint.com/c_standard_library/c_function_fgets.htm
/*
struct datos{
	char numero[tam_char];
	char estacion[tam_char];
	char idlocalidad[tam_char];
	char fecha[tam_char];
	char temperatura[tam_char];
	char humedad[tam_char];
	char rocio[tam_char];
	char precipitacion[tam_char];
	char vel_viento[tam_char];
	char dir_viento[tam_char];
	char rafaga_max[tam_char];
	char presion[tam_char];
	char d12[tam_char];	//Empezaria desde d0
	char d13[tam_char];
	char d14[tam_char];
	char d15[tam_char];
	char d16[tam_char];
	char d17[tam_char];
	char d18[tam_char];
	char d19[tam_char];
	char * punteros[20];
};
*/
void inicializopunteros(struct datos * estructura); /* Esta funcion inicializa los punteros de la estructura */
void addData(struct datos * base, int i, int j, char* token);
/*
void freememory();
void openfile();



	struct datos * base;	//Tiene que ser global asi la pueden usar en otro lados.
	struct datos nombre_columnas;

*/

void openfile()
{
	int i;
	int j;
	FILE* stream;
	char line[tam_line];
	char* token;
	char* prevtoken;
	char prevline[tam_line];
	char actualine[tam_line];

		i=-1;	//Empieza en -1
		j=0;
		stream = fopen("datos_meteorologicos.CSV", "r");

		if(stream==NULL)
		{
			printf("ERROR AL ABRIR ARCHIVO\n");
			exit(1);
		}

		base = (struct datos*) calloc (tam_base,sizeof(struct datos));
		if (base==NULL)
		{
			printf("ERROR AL RESERVAR MEMORIA\n");
			fclose(stream);
			exit (1);	
		} 
		prevtoken = (char *) calloc (100, sizeof(char));
		if(prevtoken==NULL)
		{
			printf("ERROR AL RESERVAR MEMORIA\n");
			free(base);
			fclose(stream);
			exit (1);
		}
		
		while(fgets(line, tam_line, stream))
		{
			strcpy(actualine, line);
			token=strtok(line, ",");
			
			if(i!=-1)
			{
				//printf("%c\n", prevline[tam_line-2]);
				//printf("%c\n", actualine[0]);
				if(prevline[tam_line-2]!=',' && actualine[0]!=',')
				{
					prevtoken=strcat(prevtoken, token);	//concatena s2 al final de s1
					
						//printf("%s\n", prevtoken);
					if(j==0)
					{
						addData(base, i-1, 19, prevtoken);
					}
					else
					{
						addData(base, i, j-1, prevtoken);
					}

					
					token=strtok(NULL, ",");
				}
			}
			while(token!=NULL)
			{

				if(i==-1)
		{	//If first time
			inicializopunteros(&nombre_columnas);
			strcpy(nombre_columnas.numero,"Numero");
			j=1;
			while(strcmp(token, "Estacion")){
				//printf("%s%d\n", token, strlen(token));
				token=strtok(NULL, ",");

			}	//Llega hasta el primer dato...
			
			while(strcmp(token, "Humedad de Hoja [%]"))
			{
				//printf("%s%d\n", token, strlen(token));
				addData(&nombre_columnas, 0, j, token);
				token=strtok(NULL, ",");
				j++;

			}	//Llega hasta el primer dato...
			//printf("%s\n", token);
			addData(&nombre_columnas, 0, j, token);
			j=0;
			i++;
		}
		else
		{
			
			
				//printf("%s\n", token);
			addData(base, i, j, token);
			if(j==19)
			{
				inicializopunteros(&base[i]);
				j=0;
				i++;
			}
			else
			{
				j++;

			}

			

			

		}
		strcpy(prevtoken, token);
		token=strtok(NULL, ",");
	}
	strcpy(prevline, actualine);
}


/*
i=0;
for(i=0; i<18304; i++){
	printf("%s\n", base[i].d15);
	i++;
}
*/



fclose(stream);
free(prevtoken);
prevtoken=NULL;
	//freememory();
}


void freememory()
{
	free(base);
	base=NULL;
}

void addData(struct datos * base, int i, int j, char* token)
{
	switch(j)
	{
		case 0:
		strcpy(base[i].numero, token);
		break;
		case 1:
		strcpy(base[i].estacion, token);
		break;
		case 2:
		strcpy(base[i].idlocalidad, token);
		break;
		case 3:
		strcpy(base[i].fecha, token);
		break;
		case 4:
		strcpy(base[i].temperatura, token);
		break;
		case 5:
		strcpy(base[i].humedad, token);
		break;
		case 6:	
		strcpy(base[i].rocio, token);
		break;
		case 7:
		strcpy(base[i].precipitacion, token);
		break;
		case 8:
		strcpy(base[i].vel_viento, token);
		break;
		case 9:
		strcpy(base[i].dir_viento, token);
		break;
		case 10:
		strcpy(base[i].rafaga_max, token);
		break;
		case 11:
		strcpy(base[i].presion, token);
		break;
		case 12:
		strcpy(base[i].d12, token);
		break;
		case 13:
		strcpy(base[i].d13, token);
		break;
		case 14:
		strcpy(base[i].d14, token);
		break;
		case 15:
		strcpy(base[i].d15, token);
		break;
		case 16:
		strcpy(base[i].d16, token);
		break;
		case 17:
		strcpy(base[i].d17, token);
		break;
		case 18:
		strcpy(base[i].d18, token);
		break;
		case 19:
		strcpy(base[i].d19, token);
		break;
		default:
		break;


	}

}



void inicializopunteros(struct datos * estructura)
{
	estructura->punteros[0] = estructura->numero;
	estructura->punteros[1] = estructura->estacion;
	estructura->punteros[2] = estructura->idlocalidad;
	estructura->punteros[3] = estructura->fecha;
	estructura->punteros[4] = estructura->temperatura;
	estructura->punteros[5] = estructura->humedad;
	estructura->punteros[6] = estructura->rocio;
	estructura->punteros[7] = estructura->precipitacion;
	estructura->punteros[8] = estructura->vel_viento;
	estructura->punteros[9] = estructura->dir_viento;
	estructura->punteros[10] = estructura->rafaga_max;
	estructura->punteros[11] = estructura->presion;
	estructura->punteros[12] = estructura->d12;
	estructura->punteros[13] = estructura->d13;
	estructura->punteros[14] = estructura->d14;
	estructura->punteros[15] = estructura->d15;
	estructura->punteros[16] = estructura->d16;
	estructura->punteros[17] = estructura->d17;
	estructura->punteros[18] = estructura->d18;
	estructura->punteros[19] = estructura->d19;
}