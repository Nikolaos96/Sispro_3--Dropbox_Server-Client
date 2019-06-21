/* functions.c */
#include "functions.h"


 // take arguments from command line
 void take_argument(int argc, char *argv[], int *port){
     if(argc != 3){
         printf("Error.Give me port number. \n");
         exit(1);
     }
     if( !strcmp(argv[1], "-p") )  *port = atoi(argv[2]);
     else{
	 printf("Error with port. \n");
	 exit(1);
     }
     return;
 }



 void perror_exit(char *mes){
     perror(mes);
     exit(EXIT_FAILURE);
 }



 // this function send in all clients log on the new client
 void send_log_on(info_deikti *list, char *ip, int port){
     int sock, i;
     struct sockaddr_in server;
     struct sockaddr *serverptr=(struct sockaddr *) &server;

     int size_list = s_list(list);
     for(i = 1 ; i < size_list ; i++){
	 tuple t = take_element(list, i);

	 if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) perror_exit("socket LOG_ON");
         server.sin_family = AF_INET;
 	 server.sin_addr.s_addr = inet_addr(t.Ip);
         server.sin_port = htons(t.port);
         if(connect(sock, serverptr, sizeof(server)) < 0) perror_exit("connect LOG_ON");

	 if(write(sock,"USER_ON......", 13) < 0) perror_exit("write Log_ON");

	 int ipp;
	 inet_pton(AF_INET, ip, (void *)&ipp);
         int portt = htonl(port);

	 if(write(sock, &ipp, sizeof(ipp)) < 0) perror_exit("write IP");
	 if(write(sock, &portt, sizeof(portt)) < 0) perror_exit("write port");
	 close(sock);
     }
 }




 // send log off in all clients the client that leave
 void send_log_off(info_deikti *list, char *ip, int port){
     int sock, i;
     struct sockaddr_in server;
     struct sockaddr *serverptr=(struct sockaddr *) &server;

     int size_list = s_list(list);
     for(i = 1 ; i <= size_list ; i++){
         tuple t = take_element(list, i);

	 if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) perror_exit("socket LOG_OFF");
         server.sin_family = AF_INET;
         server.sin_addr.s_addr = inet_addr(t.Ip);
         server.sin_port = htons(t.port);
         if(connect(sock, serverptr, sizeof(server)) < 0) perror_exit("connect LOG_OFF");

         if(write(sock,"USER_OFF.....", 13) < 0) perror_exit("write Log_OFF");

	 int ipp;
         inet_pton(AF_INET, ip, (void *)&ipp);
         if(write(sock, &ipp, sizeof(ipp)) < 0) perror_exit("write IP LOG_OFF");

	 int pport = htonl(port);
	 if(write(sock, &pport, sizeof(pport)) < 0) perror_exit("write port LOG_OFF");
         close(sock);
     }
 }



 // signal handler
 void stopp(int sig){

     if(s_list(&list) == 0)
	 stop = 0;
     else
         printf("\nI can not terminate because there are connected clients. \n");

 }


