//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
#include "sock_cli_i_cc.c"
#include "variables_comunes.h"
#define PORT "6020\0"
//Deberia hacer un header.

int check(char* message, char** parametros);


int main( int argc, char *argv[] ) {

	//char * buffer;
	char buffer[TAM];
	char arg0[TAM];
	char arg1[TAM];
	char arg2[TAM];
	char arg3[TAM];
	char* argumentos[4] ={arg0, arg1, arg2, arg3};
	char* parametros[4];
	int bool;
/*
		buffer = (char*) calloc (TAM,sizeof(char));
		if (buffer==NULL)
		{
			printf ("Error allocating requested memory");
			exit (1);
		}
*/
	bool=0;

		
	while(!bool){
		printf("Conectese a un servidor usando el comando: connect usuario@numero_ip:port\n");
		memset( buffer, '\0', TAM );
		fgets(buffer, TAM-1, stdin);
		bool=check(buffer, argumentos);
	}
	//Ahora me tira un segmentation fault y no entiendo por que...
	//https://www.codingunit.com/c-reference-stdlib-h-function-calloc
	//free(buffer);
	//printf("%s\n", argv[0]);

	parametros[0]=argv[0];			//Nombre del programa
	parametros[1]=argumentos[2];	//Direccion ip del servidor
	parametros[2]=argumentos[3];	//Puerto del servidor
	parametros[3]=argumentos[1];	//Nombre de Usuario
	cli_i_cc(3, parametros);

}

int check(char* message, char** argumentos){
	//https://www.tutorialspoint.com/cprogramming/c_strings.htm
	//Se uso codigo de https://www.tutorialspoint.com/c_standard_library/c_function_strtok.htm

 	int i;
 	
 

 	argumentos[0]=strtok(message, " \0");
 	argumentos[1]=strtok(NULL, "@\0");
 	argumentos[2]=strtok(NULL, ":\0");
 	argumentos[3]=strtok(NULL, ":\0");
 	//Separo el comando ingresado en los tokens correspondientes.


 	for(i=0;i<4;i++){
 		if(argumentos[i]==NULL){
 			return 0;
 		}
 	}//Si cualquier de los tokens es NULL, entonces no paso el checkeo.

 	i=strlen(argumentos[3])-1;	//El ultimo token tiene un caracter demas.
 	argumentos[3][i]='\0';		//Por lo tanto pongo ese ultimo caracter en '\0' para poder compararlo mas adelante.


 	if(strcmp(argumentos[0], "connect\0")==0 && argumentos[1]!=NULL /*&& strcmp(argumentos[3], PORT)==0*/){
 		return 1;
 	}
 	else{
 		return 0;
 	}
 	

}
