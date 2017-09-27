all: srv
	@echo Compilacion realizada. Ejecutar con ./srv y ./cli
srv: sock_srv_i_cc.c openfile.c sock_srv_i_sc.c
	gcc -g -Wall -pedantic -std=gnu99 -Werror -o srv main_srv.c sock_srv_i_cc.c openfile.c sock_srv_i_sc.c
checksrv:
	cppcheck main_srv.c
clean:
	rm srv
#-Wall -pedantic -Werror 
