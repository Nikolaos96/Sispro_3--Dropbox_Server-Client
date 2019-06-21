dropbox_client: lista.o dropbox_client.o functions.o lista2.o
	gcc lista.o dropbox_client.o functions.o lista2.o -o dropbox_client -lpthread

dropbox_client.o:dropbox_client.c
	gcc -c dropbox_client.c

lista.o:lista.c
	gcc -c lista.c

functions.o:functions.c
	gcc -c functions.c

lista2.o:lista2.c
	gcc -c lista2.c

clean:
	rm -f dropbox_client dropbox_client.o lista.o functions.o lista2.o

run:
	dropbox_client -d client -p 9005 -w 10 -b 100 -sp 9002 -sip linux01.di.uoa.gr

run1:
	dropbox_client -d client1 -p 9006 -w 10 -b 100 -sp 9002 -sip linux01.di.uoa.gr

run2:
	dropbox_client -d client2 -p 9007 -w 10 -b 100 -sp 9002 -sip linux01.di.uoa.gr

run3:
	dropbox_client -d client3 -p 9008 -w 10 -b 100 -sp 9002 -sip linux01.di.uoa.gr
