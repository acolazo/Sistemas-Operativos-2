#define tam_base 20000
#define tam_char 30
#define tam_line 1024

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



void freememory();
void openfile();



struct datos * base;	//Tiene que ser global asi la pueden usar en otro lados.
struct datos nombre_columnas;
