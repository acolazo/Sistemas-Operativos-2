//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include "openfile.c"
#define TAM 256
#define MAX_LENGTH 10
#define defaultuser "user"
#define defaultpass "pass"

void addUser(char * user, char * pass);
void printTable();
int verify_user(char * buffer, char * message);
int commands(char * buffer, char * message);
void deleteLista();
struct nodo* buscarUser(char * user);
void freeall();
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

	openfile();
	char user[MAX_LENGTH+1]=defaultuser;
	char pass[MAX_LENGTH+1]=defaultpass;
	tabla.first = NULL;
	addUser(user, pass);
	addUser("agus", "colazo");
	addUser("admin", "admin");
	printf("%s\n", user);
	printf("%s\n", tabla.first->username);


	if ( argc < 2 ) {
		fprintf( stderr, "Uso: %s <puerto>\n", argv[0] );
		freeall();
		exit( 1 );
	}

	sockfd = socket( AF_INET, SOCK_STREAM, 0);
	if ( sockfd < 0 ) { 
		perror( " apertura de socket ");
		freeall();
		exit( 1 );
	}

	memset( (char *) &serv_addr, 0, sizeof(serv_addr) );
	puerto = atoi( argv[1] );
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons( puerto );

	if ( bind(sockfd, ( struct sockaddr *) &serv_addr, sizeof( serv_addr ) ) < 0 ) {
		perror( "ligadura" );
		freeall();
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

			int verified;
			int comando;
			char message[TAM];
			verified=0;
			comando=0;
			while ( 1 ) {
				memset(message, '\0', TAM);
				memset( buffer, '\0', TAM );


				n = read( newsockfd, buffer, TAM-1 );
				if ( n < 0 ) {
					perror( "lectura de socket" );
					exit(1);
				}

				printf( "PROCESO %d. ", getpid() );
				printf( "Recibí: %s", buffer );

				if(verified==0){
					verified=verify_user(buffer, message);
				}
				else if (verified==1){	//Acceso permitido
					comando=commands(buffer, message);
				}

				

				message[strlen(message)]='\n';
				message[strlen(message)]='\0';
				n = write( newsockfd, message, strlen(message) );
				if ( n < 0 ) {
					perror( "escritura en socket" );
					exit( 1 );
				}

				if(verified==-1 || comando==-1){ //Acceso denegado
					//Hacer algo mas...? Esto debe seguir en el cliente...Cerrar cliente?

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

void freeall(){
	deleteLista();
	freememory();
}

int commands(char * buffer, char * message){
	char elemento[TAM];
	strcpy(elemento, buffer);

	memset(message, '\0', TAM);
	elemento[strlen(elemento)-1]='\0';
	printf("PROCESO%d.", getpid());
	if( !strcmp( "fin", elemento ) ) {
		printf( "Como recibí 'fin', termino la ejecución.\n\n");
		strcpy(message, "Se ejecuto el comando fin");
		return -1;
	}
	else if(!strcmp("free", elemento)){
		freeall();
		strcpy(message, "Se libera la memoria");
		printf("Se libera la memoria\n");
	}
	else{
		strcpy(message, "Obtuve su respuesta");	//Si no hago nada.
	}

	return 0;
}

int verify_user(char * buffer, char * message){
	
	static int verified=0;
	static struct nodo * user;
	char elemento[TAM];
	//memset(elemento, '\0', TAM);


	printf("Proceso %d.", getpid());
	

	strcpy(elemento, buffer);
	elemento[strlen(elemento)-1] = '\0';
	if(verified==0){
		printf("Verificando usuario y contrasenia\n");
		printf( "Nombre de usuario recibido: %s", buffer );
		user=buscarUser(elemento);
		if(user!=NULL){
			printf("El usuario existe.\n");
			verified=1;
		}
		else{
			printf("El usuario no existe.\n");
			verified=-1;
		}
		strcpy(message,"Ingrese la contrasenia y presione ENTER.");
		return 0;	//La verificacion aun no termino
	}
	else if(verified==1){
		if(strcmp(user->password, elemento)==0){
			strcpy(message, "Acceso permitido");
			printf("Acceso permitido: Contrasenia correcta.\n");
			verified=1;	//Acceso permitido
		}
		else{
			strcpy(message, "Acceso denegado: Usuario y/o contrasenia incorrectos");
			printf("Acceso denegado: Contrasenia incorrecta.\n");
			verified=-1;
		}		
	}
	else{
		strcpy(message, "Acceso denegado: Usuario y/o contrasenia incorrectos");
		verified=-1;
	}
	return verified;
}	



void addUser(char * user, char * pass){
	//https://www.programiz.com/c-programming/c-dynamic-memory-allocation
	struct nodo * nuevo;
	nuevo = (struct nodo*) malloc(sizeof(struct nodo));
	if(nuevo == NULL)
	{
		printf("Error! memory not allocated.");
		exit(0);
	}
	memset(nuevo->username,  '\0', MAX_LENGTH+1);
	memset(nuevo->password, '\0', MAX_LENGTH+1);
	
	strcpy(nuevo->username, user);
	strcpy(nuevo->password, pass);
	printf("%s\n", nuevo->username);
	if(tabla.first == NULL){
		nuevo->next = NULL;
	}
	else{
		nuevo->next=tabla.first;
	}
	tabla.first=nuevo;
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

void deleteLista(){
	struct nodo * first;
	struct nodo * next;
	first=tabla.first;
	next=first->next;
	while(next!=NULL){
		free(first);
		first=next;
		next=first->next;
	}
	free(first);
	tabla.first=NULL;
}