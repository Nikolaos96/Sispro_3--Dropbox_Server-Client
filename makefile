dropbox_server: lista.o dropbox_server.o functions.o
	gcc lista.o dropbox_server.o functions.o -o dropbox_server

dropbox_server.o:dropbox_server.c
	gcc -c dropbox_server.c

lista.o:lista.c
	gcc -c lista.c

functions.o:functions.c
	gcc -c functions.c

clean:
	rm -f dropbox_server dropbox_server.o lista.o functions.o

run:
	dropbox_server -p 9002
