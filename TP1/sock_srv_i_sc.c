#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#ifndef OPENFILE_H
#define OPENFILE_H
#include "openfile.h"
#endif 
#include "sock_srv_i_sc.h"
#define TAM 256


void sendfile_sc( int port_number ) {
	int sockfd, puerto, tamano_direccion;
	char buffer[ TAM ];
	char buffer_send [TAM];
	struct sockaddr_in serv_addr;
	int n;
	FILE * file_send;


	file_send=fopen(FILE_TO_SEND, "r");

	

	if(file_send==NULL)
	{
		printf("ERROR AL ABRIR EL ARCHIVO.\n");
		exit(0);
	}




	
	sockfd = socket( AF_INET, SOCK_DGRAM, 0 );
	if (sockfd < 0) { 
		perror("ERROR en apertura de socket");
		exit( 1 );
	}

	memset( &serv_addr, 0, sizeof(serv_addr) );
	puerto = port_number;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons( puerto );
	memset( &(serv_addr.sin_zero), '\0', 8 );

	if( bind( sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr) ) < 0 ) {
		perror( "ERROR en binding" );
		exit( 1 );
	}

        printf( "Socket disponible: %d\n", ntohs(serv_addr.sin_port) );

	tamano_direccion = sizeof( struct sockaddr );

	while(fgets(buffer_send, TAM, file_send))	
	{	

		memset( buffer, 0, TAM );
		n = recvfrom( sockfd, buffer, TAM-1, 0, (struct sockaddr *)&serv_addr, &tamano_direccion );
		if ( n < 0 ) {
			perror( "lectura de socket" );
			exit( 1 );
		}
		printf( "Recibí: %s\n", buffer );
		n = sendto( sockfd, (void *)buffer_send, TAM, 0, (struct sockaddr *)&serv_addr, tamano_direccion  );
		if ( n < 0 ) {
			perror( "escritura en socket" );
			exit( 1 );
		}
	}
		n = sendto( sockfd, (void *)"END_CODE", 9, 0, (struct sockaddr *)&serv_addr, tamano_direccion  );
		if ( n < 0 ) {
			perror( "escritura en socket" );
			exit( 1 );
		}	

	printf("FILE SENT\n");
	fclose(file_send);
	close(sockfd);
	return; 
} 
