#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include "sock_srv_i_cc.h"
#include "variables_comunes.h"
#include "sock_srv_i_sc.h"
#ifndef OPENFILE_H
#define OPENFILE_H
#include "openfile.h"
#endif 




#define MAX_LENGTH 10
#define defaultuser "user"	///deberia usar enum
#define defaultpass "pass"



void help(char * message);

void addUser(char * user, char * pass);
/**
 * @brief Agrega usuario y contraseña a la lista.
 *
 * @param user string con nombre de usuario
 * @param pass string con contraseña
 * 
 */
void printTable();
/**
 * @brief Imprime la tabla de usuarios
 *
 * 
 */
int verify_user(char * buffer, char * message);
/**
 * @brief Verifica al usuario y contraseña, devuelve un mensaje de validacion.
 *
 * @param buffer es buffer para recibir texto del cliente.
 * @param message es buffer para enviar mensajes al cliente.
 * 
 */
int commands(char * buffer, char * message);
/**
 * @brief Funcion que elige el comando a ejecutar desde el input del usuario.
 *
 * @param buffer contiene el input del usuario.
 * @param message devuelve lo que el usuario espera, o si hubo algun error.
 * 
 */
void deleteLista();
/**
 * @brief Elimina la lista de usuarios y contraseñas. Libera el espacio alocado.
 *
 * 
 */
struct nodo* buscarUser(char * user);
/**
 * @brief Busca un usuario en la tabla y si existe, devuelve el nodo que lo contiene.
 *
 * @param user string con el nombre de usuario a buscar.
 * @return Nodo que contiene al usuario o NULL
 */
void freeall();
/**
 * @brief Libera todo el espacio alocado
 *
 *
 */

void listar(char * message);
/**
 * @brief Lista todas las estaciones y sus sensores con datos.
 *
 * @param message buffer para enviar mensaje al cliente.
 *
 */
void diario_precipitacion(char* nro_estacion, char *message);
/**
 * @brief Devuelve el acumulado diario de determinada estacion.
 *
 * @param nro_estacion string con numero de la estacion a buscar.
 * @param message buffer en el que se envian los datos al cliente.
 */
void mensual_precipitacion(char* nro_estacion, char *message);
/**
 * @brief Devuelve el acumulado mensual de determinada estacion.
 *
 * @param nro_estacion string con numero de la estacion a buscar.
 * @param message buffer en el que se envian los datos al cliente.
 */
void promedio_variable(char* nro_estacion, char* nombre_variable,char *message);
/**
 * @brief Devuelve el promedio de determinada variable de determinada estacion.
 *
 * @param nro_estacion string con numero de la estacion a buscar.
 * @param nombre_variable string con nombre de la variable a buscar.
 * @param message buffer en el que se envian los datos al cliente.
 */
void descargar(char * nro_estacion, char * message);
/**
 * @brief Transfiere el archivo con los datos de determinada estacion.
 *
 * @param nro_estacion string con numero de la estacion a buscar.
 * @param message buffer en el que se envia mensaje al cliente.
 */

/*
Si usuario y contrasenia son correctos, sigue con la ejecucion del proceso hijo. Sino, envia un mensaje de usuario/contrasenia
y finaliza la ejecucion.
*/

struct nodo{
	char username[MAX_LENGTH+1];
	char password[MAX_LENGTH+1];
	struct nodo* next;
};/**< Estructura nodo que contiene un usuario y su contraseña */
struct tabla_users{
	int size;
	struct nodo* first;
};/**< Estructura que sirve de una lista simplemente enlazada con los nodos que contienen usuarios */


struct tabla_users tabla;/**< Lista simplemente enlazada de usuarios */
int port_number_udp;/**< Int con el numero de puerto que se utilizara para levantar el servidor UDP en caso de necestarse */

int srv_i_cc( int argc, char *argv[] ) {
	int sockfd, newsockfd, puerto, pid;
	socklen_t clilen;
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
	
	
	//prueba;
	/*
	port_number=atoi(argv[1]);
	char prueba[TAM];
	char message[TAM];
	strcpy(prueba, "descargar 30099");
	commands(prueba, message);
	printf("%s\n", message);
	//listar(prueba);
	//diario_precipitacion("30135", prueba);
	//mensual_precipitacion("30135", prueba);
	//for(n=0;n<20;n++)
	//{
	//	printf("%s\n", nombre_columnas.punteros[n]);
	//}

	//promedio_variable("30135", "Humedad", prueba);
	freeall();
	return 0;
	//prueba;

*/
	
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
	port_number_udp=puerto+1;
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
	char * token1;
	char * token2;
	char * token3;
	char elemento[TAM];
	strcpy(elemento, buffer);

	token1=NULL;
	token2=NULL;
	token3=NULL;
	memset(message, '\0', TAM);
	elemento[strlen(elemento)-1]='\0';
	printf("PROCESO %d. ", getpid());
	if( !strcmp( "desconectar", elemento ) ) {
		printf( "Como recibí 'desconectar', termino la ejecución.\n\n");
		strcpy(message, "Se ejecuto el comando desconectar");
		return -1;
	}
	else if(!strcmp("free", elemento)){
		freeall();
		strcpy(message, "Se libera la memoria");
		printf("Se libera la memoria\n");
	}
	else if(!strcmp("help", elemento)){
		help(message);
		printf("Se ejecuto el comando help\n");
	}
	else if(!strcmp("listar", elemento)){
		listar(message);
		printf("Se muestra listado de estaciones\n");
	}
	else
	{
		token1=strtok(elemento, " ");
		token2=strtok(NULL, " ");
		token3=strtok(NULL, "\0");

		if(!strcmp(token1, "diario_precipitacion"))
		{
			diario_precipitacion(token2, message);
		}
		else if(!strcmp(token1, "mensual_precipitacion"))
		{
			mensual_precipitacion(token2, message);
		}
		else if(!strcmp(token1, "promedio_variable"))
		{
			promedio_variable(token2, token3, message);
		}
		else if(!strcmp(token1, "descargar"))
		{
			descargar(token2, message);
		}
		else
		{
			strcpy(message, "Obtuve su respuesta");	//Si no hago nada.	
		}
		
	}

	return 0;
}



int verify_user(char * buffer, char * message){
	
	static int verified=0;
	static struct nodo * user;
	char elemento[TAM];
	//memset(elemento, '\0', TAM);


	printf("Proceso %d. ", getpid());
	

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
	nuevo=NULL; //Esta bien? Hace falta ?
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


void listar(char * message)	
/*Esta funcion hace el listado de las estaciones disponibles y sus sensores activos */
/*El argumento message es un puntero a un buffer que servira para transmitir el mensaje al cliente*/
/*Estoy bastante seguro que en esta funcion me podria ahorrar un poco de duplicacion de codigo...*/
{		//hay un espacio \n cagando todo...
	int i;
	int j;
	int sensores_activos [20];


	for(j=0; j<20; j++){
		sensores_activos[j]=0;
	}
	i=1;
	strcpy(message, "\n");

	strcat(message, base[0].numero);
	strcat(message, ": ");
	strcat(message, base[0].estacion);
	strcat(message, "\n");
	for(j=4; j<20; j++)
	{
		if((sensores_activos[j]==0)
			&& strcmp(base[0].punteros[j], "--"))
		{
			sensores_activos[j]=1;
			strcat(message, nombre_columnas.punteros[j]);
			strcat(message, ", ");
		}
	}
	while(strcmp(base[i].numero, "\0"))
 	{	//Mientras base[i].numero no sea un string vacio.
 		printf("%d\n", i);
 		if(!strcmp(base[i-1].numero, base[i].numero))
 		{

		}	//Si son iguales no hago nada.
		else
		{	
			strcat(message, "\n\n");
			strcat(message, base[i].numero);
			strcat(message, ": ");
			strcat(message, base[i].estacion);
			strcat(message, "\nSensores activos: ");
			for(j=4; j<20; j++) /* Reseteo los sensores_activos */
			{
				sensores_activos[j]=0;
			}
		}	//Me pone un espacio vacio no se porque...

		for(j=4; j<20; j++)
		{
			if((sensores_activos[j]==0)
				&& strcmp(base[i].punteros[j], "--"))
			{
				sensores_activos[j]=1;
				strcat(message, nombre_columnas.punteros[j]);
				strcat(message, ", ");
			}
		}

		i++;
	}
	printf("%s\n", message);
	return;
}

void diario_precipitacion(char* nro_estacion, char *message)
{
	//base[x].punteros[7] es precipitacion.
	int i;
	double acumulado;
	double acumulado_total;
	double numero;
	char s_acumulado[20];
	i=0;
	acumulado=0;
	acumulado_total=0;

	//int num=strlen(base[100].numero[1]);
	//int num2=strlen(nro_estacion);
	//printf("comparo %d con %d\n", strlen(base[0].precipitacion), acumulado);

	if(message==NULL)
	{
		printf("ERROR BUFFER MESSAGE");
		exit(0);
	}
	else if(nro_estacion==NULL)
	{
		strcpy(message, "El comando es invalido.");
		return;
	}

	while(strcmp(base[i].numero, "\0") 
		&& strcmp(&base[i].numero[1], nro_estacion))
	{
		//printf("%d\n", i);
		i++;
	}	/*Itero hasta encontrarme con el nro_estacion o terminar de correr toda la base*/
		
		if(!strcmp(&base[i].numero[1], nro_estacion))
		{
			strcpy(message, "La estacion numero ");
			strcat(message, base[i].numero);
			strcat(message, " tiene las siguientes precipitaciones diarias:\n\n");
			while(!strcmp(&base[i].numero[1], nro_estacion))
			{
				sscanf(base[i].precipitacion, "%lf", &acumulado);	//http://stackoverflow.com/questions/10075294/converting-string-to-a-double-variable-in-c
				while(!strcmp(base[i].numero, base[i+1].numero)
					&& !strcmp(strtok(base[i].fecha, " "), strtok(base[i+1].fecha, " ")))					
				{
					sscanf(base[i+1].precipitacion, "%lf", &numero);
					acumulado=acumulado+numero;
					i++;					
				}
				acumulado_total=acumulado+acumulado_total;
				printf("%lf\n", acumulado_total);
				printf("%lf\n", acumulado);
				snprintf(s_acumulado, 20, "%lf", acumulado);	/*Convierte acumulado en string en decimal*/
				strcat(message, base[i].fecha);
				strcat(message, ": ");
				strcat(message, s_acumulado);	
				strcat(message, " [mm]\n");
				

				i++;
			}


		}
		else
		{
			strcpy(message, "No se encontro esa estacion en la base de datos.");
		}

		printf("%s\n", message);
		
		return;
	}


	void mensual_precipitacion(char* nro_estacion, char *message)
	{
	//base[x].punteros[7] es precipitacion.
		int i;
		double acumulado;
		double numero;
		char s_acumulado[20];
		char *token1;
		char *token2;
		i=0;
		acumulado=0;

	//int num=strlen(base[100].numero[1]);
	//int num2=strlen(nro_estacion);
	//printf("comparo %d con %d\n", strlen(base[0].precipitacion), acumulado);
		if(message==NULL)
		{
			printf("ERROR BUFFER MESSAGE");
			exit(0);
		}
		else if(nro_estacion==NULL)
		{
			strcpy(message, "El comando es invalido.");
			return;
		}
		

		while(strcmp(base[i].numero, "\0") 
			&& strcmp(&base[i].numero[1], nro_estacion))
		{
		//printf("%d\n", i);
			i++;
	}	/*Itero hasta encontrarme con el nro_estacion o terminar de correr toda la base*/

			if(!strcmp(&base[i].numero[1], nro_estacion))
			{
				strcpy(message, "La estacion numero ");
				strcat(message, base[i].numero);
				strcat(message, " tiene las siguientes precipitaciones mensuales:\n\n");
				while(!strcmp(&base[i].numero[1], nro_estacion))
				{
				sscanf(base[i].precipitacion, "%lf", &acumulado);	//http://stackoverflow.com/questions/10075294/converting-string-to-a-double-variable-in-c
				token1=strtok(base[i].fecha, "/");
				token1=strtok(base[i].fecha, " ");
				token2=strtok(base[i+1].fecha, "/");
				token2=strtok(base[i+1].fecha, " ");
				while(!strcmp(base[i].numero, base[i+1].numero)
					&& !strcmp(token1, token2))					
				{
					sscanf(base[i].precipitacion, "%lf", &numero);
					acumulado=acumulado+numero;
					i++;					
				}
				printf("%lf\n", acumulado);
				snprintf(s_acumulado, 20, "%lf", acumulado);	/*Convierte acumulado en string en decimal*/
				strcat(message, token1);
				strcat(message, ": ");
				strcat(message, s_acumulado);	
				strcat(message, " [mm]\n");
				

				i++;
			}


		}
		else
		{
			strcpy(message, "No se encontro esa estacion en la base de datos.");
		}

		printf("%s\n", message);
		
		return;
	}


	void promedio_variable (char * nro_estacion, char * nombre_variable, char * message)
	{
		//base[x].punteros[7] es precipitacion.
		int i;
		double j;	//No me acuerdo como se traba (double/int)
		int variable;
		double promedio;
		double numero;
		char s_promedio[20];
		char *token;
		variable=-1;
		promedio=0;
		numero=0;
		j=0;

	//int num=strlen(base[100].numero[1]);
	//int num2=strlen(nro_estacion);
	//printf("comparo %d con %d\n", strlen(base[0].precipitacion), acumulado);
		if(message==NULL)
		{
			printf("ERROR BUFFER MESSAGE");
			exit(0);
		}
		else if(nro_estacion==NULL || nombre_variable==NULL)
		{
			strcpy(message, "El comando es invalido.");
			return;
		}

		for(i=0; i<20; i++)
		{
			token=strstr(nombre_columnas.punteros[i], nombre_variable);
			if(token!=NULL)
			{
				variable=i;
				break;	//Corta el loop?
			}
		}	//Busco la variable que corresponde o dejo en -1 si no corresponde ninguna.
		i=0;			//Pongo a i en "cero" para el siguiente bucle.


		if(variable!=-1)
		{
			while(strcmp(base[i].numero, "\0") 
				&& strcmp(&base[i].numero[1], nro_estacion))
			{
		//printf("%d\n", i);
				i++;
			}	/*Itero hasta encontrarme con el nro_estacion o terminar de correr toda la base*/

				if(!strcmp(&base[i].numero[1], nro_estacion))	//Si se encontro la estacion.
				{
					strcpy(message, "El promedio de la variable ");
					strcat(message, nombre_columnas.punteros[variable]);
					strcat(message, " de la estacion numero ");
					strcat(message, &base[i].numero[1]);
					strcat(message, " es:\n\n");

					while(!strcmp(&base[i].numero[1], nro_estacion))
					{
						if(strcmp(base[i].punteros[variable], "--"))
						{
							sscanf(base[i].punteros[variable], "%lf", &numero);	
							promedio=promedio+numero;
							j++;	
						}
						else
						{

						}
						i++;					
					}
					promedio=promedio/j;
					snprintf(s_promedio, 20, "%lf", promedio);	/*Convierte promedio en string en decimal*/
					strcat(message, s_promedio);
					strcat(message, "\n");
				}
				else
				{
					strcpy(message, "No se encontro esa estacion en la base de datos.");
				}
			}
			else
			{
				strcpy(message, "Esa variable no existe.");
			}


			printf("\n\n%s\n", message);

			return;
		}




		void descargar(char * nro_estacion, char * message)
		{
			int i;
			int j;
			int pid;
			FILE * fp;

			i=0;
			if(nro_estacion==NULL)
			{
				strcpy(message, "ERROR: NO SE INGRESO NUMERO DE ESTACION");
				return;
			}
			while(strcmp(base[i].numero, "\0") 
				&& strcmp(&base[i].numero[1], nro_estacion))
			{
				i++;
			}	//Itero hasta encontrar la estacion correcta.
			if(!strcmp(&base[i].numero[1], nro_estacion))	/* Si encuentro la estacion correcta entonces mando el archivo */
			{

			/* Creation of file */
				fp=fopen(FILE_TO_SEND, "w");
				for(j=0; j<20; j++)
				{
					fwrite(nombre_columnas.punteros[j] , 1 , tam_char , fp );
					fwrite(",", 1, 1, fp);
				}

				while(!strcmp(&base[i].numero[1], nro_estacion))
				{
					for(j=0; j<20; j++)
					{
						fwrite(base[i].punteros[j] , 1 , tam_char , fp );
						fwrite(",", 1, 1, fp);
					}
					i++;
				}
				fclose(fp);
				/*  Synchronization with pipe */
				char message_pipe[10];
				int ret, fd[2];

				ret = pipe(fd);
				if(ret == -1)
				{
					printf("ERROR AL CREAR PIPE\n");
					exit(0);
				}
				/* Sending file */	

				
				pid=fork();
				if ( pid < 0 ) {
					perror( "fork" );
					exit( 1 );
				}
				if(pid==0)	/* Proceso hijo */ /* Pipe Writer */
				{
					sendfile_sc(port_number_udp, fd);
					exit(1);
				}
				else
    {	/* Parent Reader */
    	close(fd[1]); //reader closes unused ch.
    while( read(fd[0], message_pipe, 30) > 0 )
    	printf("Message: %s", message_pipe);
    close(fd[0]);

  	/* Se podria usar un puerto distinto cada vez y mandar el puerto en cuestion al cliente */
    strcpy(message, "Downloading...");	
    printf("\n%s\n", message);
}

}
else	
{
	strcpy(message, "No se encontro la estacion");
}
return;
}
/*
diario_precipitacion no_estación: muestra el acumulado diario de la variable
precipitación de no_estación (no_día: acumnulado mm).

• mensual_precipitacion no_estación: muestra el acumulado mensual de la
variable precipitación (no_día: acumnulado mm).

• promedio variable: muestra el promedio de todas las muestras de la variable
de cada estación (no_estacion: promedio.
*/


//Hacer comando help y lista de comandos.

void help(char * message){
	strcpy(message, "Comandos:\nlistar: lista todas las estaciones\nfree: libera la memoria\ndesconectar: el usuario se desconecta del servidor\ndescargar nro_estacion\ndiario_precipitacion nro_estacion\nmensual_precipitacion nro_estacion\npromedio_variable nro_estacion nombre_variable\n");
	return;
}