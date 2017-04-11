#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define tam_base 20000
#define tam_char 30
#define tam_line 1024


//https://www.tutorialspoint.com/c_standard_library/c_function_fgets.htm
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
	

};

void addData(struct datos * base, int i, int j, char* token);
void freememory();
void openfile();
struct datos * base;	//Tiene que ser global asi la pueden usar en otro lados.

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

	if(stream==NULL){
		printf("ERROR AL ABRIR ARCHIVO\n");
		exit(1);
	}

	base = (struct datos*) calloc (tam_base,sizeof(struct datos));
	if (base==NULL){
		printf("ERROR AL RESERVAR MEMORIA\n");
		fclose(stream);
		exit (1);	
	} 
	prevtoken = (char *) calloc (100, sizeof(char));
	if(prevtoken==NULL){
		printf("ERROR AL RESERVAR MEMORIA\n");
		free(base);
		fclose(stream);
		exit (1);
	}
	
	while(fgets(line, tam_line, stream)){
		strcpy(actualine, line);
		token=strtok(line, ",");
		
		if(i!=-1){
			//printf("%c\n", prevline[tam_line-2]);
			//printf("%c\n", actualine[0]);
			if(prevline[tam_line-2]!=',' && actualine[0]!=','){
				prevtoken=strcat(prevtoken, token);	//concatena s2 al final de s1
				if(strcmp(prevtoken, "--")!=0){
					//printf("%s\n", prevtoken);
					if(j>11){
						addData(base, i-1, j-1, prevtoken);
					}
					else{
						addData(base, i, j-1, prevtoken);
					}
					
				}
				token=strtok(NULL, ",");
			}
		}
		while(token!=NULL){
	
	if(i==-1){	//If first time
		while(strcmp(token, "Humedad de Hoja [%]")){
			//printf("%s%d\n", token, strlen(token));
			token=strtok(NULL, ",");

		}	//Llega hasta el primer dato...
		printf("%s\n", token);
		i++;
	}
	else{
		
		if(strcmp(token, "--")==0 || strcmp(token, "-")==0){
			j=0;
			//Do nothing
		}

		else{
			
			//printf("%s\n", token);
			addData(base, i, j, token);
			if(j==11){

				i++;
			}
			j++;
			
		}

		

	}
	strcpy(prevtoken, token);
	token=strtok(NULL, ",");
}
strcpy(prevline, actualine);
}



i=0;
for(i=0; i<18304; i++){
	printf("%s\n", base[i].estacion);
	i++;
}



fclose(stream);
free(prevtoken);
prevtoken=NULL;
//freememory();
}


void freememory(){
	free(base);
	base=NULL;
}

void addData(struct datos * base, int i, int j, char* token){
	switch(j){
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
		default:
		break;


	}

}



/*
const char* getfield(char* line, int num)
{
	const char* tok;
	for (tok = strtok(line, ";");
		tok && *tok;
		tok = strtok(NULL, ";\n"))
	{
		if (!--num)
			return tok;
	}
	return NULL;
}
*/	
/*
	
		char* tmp = strdup(line);
		printf("Field 3 would be %s\n", getfield(tmp, 3));
        // NOTE strtok clobbers tmp
		free(tmp);

	}
	*/