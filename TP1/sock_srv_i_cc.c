#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#define TAM 256
#define MAX_LENGTH 10
#define defaultuser "user"
#define defaultpass "pass"

void addUser(char * user, char * pass);
void printTable();
void verify_user(int newsockfd);
struct nodo* buscarUser(char * user);
/*
Si usuario y contrasenia son correctos, sigue con la ejecucion del proceso hijo. Sino, envia un mensaje de usuario/contrasenia
y finaliza la ejecucion.
*/

struct nodo{
	char username[MAX_LENGTH+1];
	char password[MAX_LENGTH+1];
	struct nodo* next;
};
struct tabla_users{
	int size;
	struct nodo* first;
};


struct tabla_users tabla;

int srv_i_cc( int argc, char *argv[] ) {
	int sockfd, newsockfd, puerto, clilen, pid;
	char buffer[TAM];
	struct sockaddr_in serv_addr, cli_addr;
	int n;

	char user[MAX_LENGTH+1]=defaultuser;
	char pass[MAX_LENGTH+1]=defaultpass;
	tabla.first = NULL;
	addUser(user, pass);


	if ( argc < 2 ) {
		fprintf( stderr, "Uso: %s <puerto>\n", argv[0] );
		exit( 1 );
	}

	sockfd = socket( AF_INET, SOCK_STREAM, 0);
	if ( sockfd < 0 ) { 
		perror( " apertura de socket ");
		exit( 1 );
	}

	memset( (char *) &serv_addr, 0, sizeof(serv_addr) );
	puerto = atoi( argv[1] );
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons( puerto );

	if ( bind(sockfd, ( struct sockaddr *) &serv_addr, sizeof( serv_addr ) ) < 0 ) {
		perror( "ligadura" );
		exit( 1 );
	}

	printf( "Proceso: %d - socket disponible: %d\n", getpid(), ntohs(serv_addr.sin_port) );

	listen( sockfd, 5 );
	clilen = sizeof( cli_addr );

	while( 1 ) {
		newsockfd = accept( sockfd, (struct sockaddr *) &cli_addr, &clilen );
		if ( newsockfd < 0 ) {
			perror( "accept" );
			exit( 1 );
		}

		pid = fork(); 
		if ( pid < 0 ) {
			perror( "fork" );
			exit( 1 );
		}

		if ( pid == 0 ) {  // Proceso hijo
			close( sockfd );
			verify_user(newsockfd);
			
			while ( 1 ) {
				memset( buffer, '\0', TAM );

				n = read( newsockfd, buffer, TAM-1 );
				if ( n < 0 ) {
					perror( "lectura de socket" );
					exit(1);
				}

				printf( "PROCESO %d. ", getpid() );
				printf( "Recibí: %s", buffer );

				n = write( newsockfd, "Obtuve su mensaje", 18 );
				if ( n < 0 ) {
					perror( "escritura en socket" );
					exit( 1 );
				}
				// Verificación de si hay que terminar
				buffer[strlen(buffer)-1] = '\0';
				if( !strcmp( "fin", buffer ) ) {
					printf( "PROCESO %d. Como recibí 'fin', termino la ejecución.\n\n", getpid() );
					exit(0);
				}
			}
		}
		else {
			printf( "SERVIDOR: Nuevo cliente, que atiende el proceso hijo: %d\n", pid );
			close( newsockfd );
		}
	}
	return 0; 
} 

void verify_user(int newsockfd){
	
	int n;
	int verified_user;
	int verified_pass;
	int verified;
	int terminar;
	struct nodo * user;
	char buffer[TAM];
	char message[TAM];

	terminar=0;
	verified_user=0;
	verified_pass=0;
	verified=verified_pass*verified_user;

	while(!verified){
		
		memset( buffer, '\0', TAM );
		n = read( newsockfd, buffer, TAM-1 );
		if ( n < 0 ) {
			perror( "lectura de socket" );
			exit(1);
		}

		printf( "PROCESO %d. ", getpid() );
		buffer[strlen(buffer)-1] = '\0';
		if(verified_user){
			if(strcmp(user->password, buffer)==0){
				strcpy(message,"La contrasenia ingresada es correcta.\n");
				printf("Acceso permitido: Contrasenia correcta.\n");
				verified_pass=1;
			}
			else{
				strcpy(message,"La contrasenia ingresada es incorrecta. Por favor, vuelva a intentar");
				printf("Acceso denegado: Contrasenia incorrecta.\n");
			}
		}
		else{
			printf( "Nombre de usuario recibido: %s", buffer );
			user=buscarUser(buffer);
			if(user!=NULL){
				printf("El usuario existe.\n");
				verified_user=1;
				strcpy(message,"El usuario se ha verificado. Ingrese la contrasenia y presione ENTER\n");
			}
			else{
				printf("El usuario no existe.\n");
				strcpy(message,"El usuario no existe. Por favor vuelva a intentar.\n");
				terminar=1;
			}

		}

		
		n = write( newsockfd, message, 18 );
		if ( n < 0 ) {
			perror( "escritura en socket" );
			exit( 1 );
		}
		
				// Verificación de si hay que terminar
		
		if( !strcmp( "fin", buffer ) ) {
			printf( "PROCESO %d. Como recibí 'fin', termino la ejecución.\n\n", getpid() );
			exit(0);
		}
		if(terminar){
			exit(0);
		}

		verified=verified_user*verified_pass;
	}
}


void addUser(char * user, char * pass){
	struct nodo nuevo;
	int i;
	for(i=0; i<MAX_LENGTH; i++){
		nuevo.username[i]='\0';
		nuevo.password[i]='\0';
	}

	strcpy(nuevo.username, user);
	strcpy(nuevo.password, pass);
	if(tabla.first == NULL){
		nuevo.next = NULL;
	}
	else{
		nuevo.next=tabla.first;
	}
	tabla.first=&nuevo;
	tabla.size++;
}

struct nodo * buscarUser(char * user){	//Busco un usuario en la lista.
	struct nodo * usuario;

	usuario=tabla.first;
	while(usuario!=NULL){
		if(strcmp(usuario->username, user)==0){	//Si el usuario es el mismo.
			return usuario;					//Devuelvo el nodo que lo contiene.
		}
		else{
			usuario=usuario->next; 			//Busco en el siguiente nodo
		}
	}
	return usuario;							//Si es NULL, devuelvo usuario (NULL).
}