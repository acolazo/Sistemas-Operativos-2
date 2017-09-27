#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h> 
#include "variables_comunes.h"
#include "sock_cli_i_cc.h"
#include "sock_cli_i_sc.h"

void autenticacion_usuario(int sockfd, char* username);
/**
 * @brief Autentica al usuario.
 *
 * @param sockfd es el socket con el que se comunica el cliente.
 * @param username es el nombre de usuario.
 * 
 */


int cli_i_cc( int argc, char *argv[] ) {
	int sockfd, puerto, n, a;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	int * wait_time;
	int terminar = 0;

	a = 10;
	wait_time = &a;

	char buffer[TAM];
	if ( argc < 3 ) {
		fprintf( stderr, "Uso %s host puerto\n", argv[0]);
		exit( 0 );
	}

	puerto = atoi( argv[2] );
	sockfd = socket( AF_INET, SOCK_STREAM, 0 );
	if ( sockfd < 0 ) {
		perror( "ERROR apertura de socket" );
		exit( 1 );
	}

	server = gethostbyname( argv[1] );
	if (server == NULL) {
		fprintf( stderr,"Error, no existe el host\n" );
		exit( 0 );
	}
	memset( (char *) &serv_addr, '0', sizeof(serv_addr) );
	serv_addr.sin_family = AF_INET;
	bcopy( (char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length );
	serv_addr.sin_port = htons( puerto );
	if ( connect( sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr ) ) < 0 ) {
		perror( "conexion" );
		exit( 1 );
	}

	autenticacion_usuario(sockfd, argv[3]);

	argv[3][strlen(argv[3])-1]='\0';
	while(1) {
		printf("%s@%s: ", argv[3], argv[2]);
		memset( buffer, '\0', TAM );
		fgets( buffer, TAM-1, stdin );

		while(strlen(buffer)==1)
		{
			printf("Please type something.\n");
			printf("%s@%s: ", argv[3], argv[2]);
			memset( buffer, '\0', TAM );
			fgets( buffer, TAM-1, stdin );
		}

		n = write( sockfd, buffer, strlen(buffer) );
		if ( n < 0 ) {
			perror( "escritura de socket" );
			exit( 1 );
		}

		// Verificando si se escribió: fin
		buffer[strlen(buffer)-1] = '\0';
		if( !strcmp( "desconectar", buffer ) ) {
			terminar = 1;
		}

		memset( buffer, '\0', TAM );
		n = read( sockfd, buffer, TAM );
		if ( n < 0 ) {
			perror( "lectura de socket" );
			exit( 1 );
		}
		printf( "Respuesta: %s\n", buffer );
		buffer[strlen(buffer)-1] = '\0';
		if(!strcmp(buffer, "Downloading..."))
		{
			printf("Se bloquea la interfaz hasta que se descargue el archivo\n");
			wait(wait_time);
			receive_file(server, puerto+1);
			printf("\nSe termino de descargar el archivo\n");
		}
		if( terminar ) {
			printf( "Finalizando ejecución\n" );
			exit(0);
		}

	}
	return 0;
} 


void autenticacion_usuario(int sockfd, char* username){	
	char buffer[TAM];
	int i;
	int n;
	i=strlen(username);
	username[i]='\n';
	username[i+1]='\0';
	printf("\nPara comenzar el proceso de autenticacion de usuario presione ENTER");
	fgets(buffer, TAM-1, stdin);

	memset(buffer, '\0', TAM);
	strcpy(buffer, username);
	for(i=0; i<2; i++){

		n = write(sockfd, buffer, strlen(buffer));
		if ( n < 0 ) {
			perror( "escritura de socket" );
			exit( 1 );
		}
		memset(buffer, '\0', TAM);
		n=read(sockfd, buffer, TAM);
		if ( n < 0 ) {
			perror( "lectura de socket" );
			exit( 1 );
		}
		printf("Respuesta: %s\n", buffer );



		if(strstr(buffer, "denegado")!=NULL){
			printf("Al denegarse el acceso, se termina la ejecucion.\n");
			exit(0);
		}
		if(i==0){
			memset(buffer, '\0', TAM);
			fgets(buffer, TAM-1, stdin);
		}

	}

}