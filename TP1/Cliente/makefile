all: cli
	@echo Compilacion realizada. Ejecutar con ./srv y ./cli
cli: sock_cli_i_cc.c sock_cli_i_sc.c
	gcc -g  -Wall -pedantic -std=gnu99 -Werror -o cli main_cli.c sock_cli_i_cc.c sock_cli_i_sc.c
checkcli:
	cppcheck main_cli.c
clean:
	rm cli
#-Wall -pedantic -Werror 
