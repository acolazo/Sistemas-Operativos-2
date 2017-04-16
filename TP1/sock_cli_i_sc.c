#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#define TAM 256
#define FILENAME "received.csv"

void receive_file(struct hostent * server_ip, int puerto_udp) {
	int sockfd, puerto, n, tamano_direccion;
	struct sockaddr_in dest_addr;
	struct hostent *server;
	char buffer[TAM];
	char buffer_send[TAM];
	FILE* f_received;

	f_received=fopen(FILENAME, "w");
	if(f_received==NULL)
	{
		printf("ERROR AL ABRIR EL ARCHIVO.");
		exit(0);
	}

	/*
	if (argc < 3) {
		fprintf( stderr, "Uso %s host puerto\n", argv[0] );
		exit(0);
	}
*/
	/*
	printf("%s\n", argv[1]);
	server = gethostbyname( argv[1] );
	*/
	server=server_ip;
	if ( server == NULL ) {
		fprintf( stderr, "ERROR, no existe el host\n");
		exit(0);
	}
	

	puerto = puerto_udp;
	sockfd = socket( AF_INET, SOCK_DGRAM, 0 );
	if (sockfd < 0) {
		perror( "apertura de socket" );
		exit( 1 );
	}

	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons( puerto );
	dest_addr.sin_addr = *( (struct in_addr *)server->h_addr );
	memset( &(dest_addr.sin_zero), '\0', 8 );

	
	memset( buffer, 0, TAM );
	strcpy(buffer, "Please send file.");

	tamano_direccion = sizeof( dest_addr );
	n = sendto( sockfd, (void *)buffer, TAM, 0, (struct sockaddr *)&dest_addr, tamano_direccion );
	if ( n < 0 ) {
		perror( "Escritura en socket" );
		exit( 1 );
	}
	while(1)
	{
		memset( buffer, 0, sizeof( buffer ) );
		n = recvfrom( sockfd, (void *)buffer, TAM, 0, (struct sockaddr *)&dest_addr, &tamano_direccion );
		if ( n < 0 ) {
			perror( "Lectura de socket" );
			exit( 1 );
		}
		//printf("%s\n", buffer);
		
		if(!strcmp(buffer, "END_CODE"))
			break;
			
		fprintf(f_received, "%s", buffer);
		memset( buffer_send, 0, sizeof( buffer ) );
		strcpy(buffer_send, "Send more please");
		n = sendto( sockfd, (void *)buffer_send, TAM, 0, (struct sockaddr *)&dest_addr, tamano_direccion );
		if ( n < 0 ) {
			perror( "Escritura en socket" );
			exit( 1 );
		}
	}
	fclose(f_received);
	close(sockfd);
	return;
} 
