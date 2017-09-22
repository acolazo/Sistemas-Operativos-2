all: cli
	@echo Compilacion realizada. Ejecutar con ./srv y ./cli
srv: sock_srv_i_cc.c openfile.c sock_srv_i_sc.c
	gcc -g -o srv main_srv.c sock_srv_i_cc.c openfile.c sock_srv_i_sc.c
cli: sock_cli_i_cc.c sock_cli_i_sc.c
	gcc -g -o cli main_cli.c sock_cli_i_cc.c sock_cli_i_sc.c
checksrv:
	cppcheck main_srv.c
checkcli:
	cppcheck main_cli.c
clean:
	rm srv
#-Wall -pedantic -Werror 
