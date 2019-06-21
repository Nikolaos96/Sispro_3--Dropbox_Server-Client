/* dropbox_server.c */
#include <stdio.h>
#include <pthread.h>
#include <poll.h>
#include "lista.h"
#include "functions.h"


 char log_on[] = "LOG_ON.....";
 char get_clients[] = "GET_CLIENTS";
 char log_off[] = "LOG_OFF....";
 char user_off[] = "USER_OFF...";



int main(int argc, char *argv[]){
  int portNum, sock, newsock, status = 0;
  pid_t child_pid, wpid;


  // SIGINT signal stop basic loop
  // if exist connected clients can not terminate.
  // else terminate
  signal(SIGINT, stopp);

  struct sockaddr_in server, client;
  socklen_t clientlen;
  struct sockaddr *serverptr =(struct sockaddr *) &server;
  struct sockaddr *clientptr =(struct sockaddr *) &client;

  stop = 1;
  take_argument(argc, argv, &portNum);
  // create list with connected clients
  list = LIST_dimiourgia(&list);



  if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) perror_exit("socket");
  int k = 1;
  if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR,&k, sizeof(int)) < 0) perror_exit("setsockopt");


  server.sin_family = AF_INET ; 			/* Internet domain */
  server.sin_addr.s_addr = htonl(INADDR_ANY);
  server.sin_port = htons(portNum); 			/* The given port     htons  */


  if(bind(sock, serverptr, sizeof(server)) < 0) perror_exit("bind");
  if(listen(sock, SOMAXCONN) < 0) perror_exit("listen");

  struct pollfd pol[1];
  pol[0].fd = sock;
  pol[0].events = POLLIN;

  char buf[11];
  while(stop == 1){
      int j = poll(pol, 1, 0);
      if(j == 0){
           //printf("Wait for new client. \n");
           usleep(10);
      }else if(j == -1){
           printf("Poll error \n");
      }else if( (j == 1) && (pol[0].revents == POLLIN) && (pol[0].fd == sock) ){

          if((newsock = accept(sock, clientptr, &clientlen)) < 0) perror_exit("accept");
          if( read(newsock, buf, sizeof(buf)) < 0) perror_exit("read");


          if(!strcmp(buf, log_on)){ // LOG_ON

 	      char ip[50];
	      int ip_b;
	      int port;
	      if( read(newsock, &ip_b, sizeof(ip_b)) < 0) perror_exit("read ip log_on");
	      if( read(newsock, &port, sizeof(port)) < 0) perror_exit("read port log_on");

	      inet_ntop(AF_INET, (void *)&ip_b, ip, 50);
	      port = ntohl(port);

	      if(find_client(&list, ip, port) == 0){
	          int ok = 1;
	          ok = htonl(ok);
	          if( write(newsock, &ok, sizeof(ok)) < 0) perror_exit("write ok 1");

	          eisagogi(&list, ip, port);
	          if(s_list(&list) > 1){

		      switch( (child_pid = fork()) ){
		          case -1:
		              perror_exit("fork error ");
		          case 0:{
		              send_log_on(&list, ip, port);   // stelnei se olous tous client to ip kai port tou neou
			      lista_diagrafi(&list);
			      exit(0);
		          }
		      }
	          }
	          printf("new client insert with IP: %s  and port %d \n", ip, port);

	          int crowd_client = s_list(&list);
	          if( write(newsock, "CLIENT_LIST", 11) < 0) perror_exit("write CLIENT_LIST");
	          int cc = crowd_client-1;
	          if( write(newsock, &cc, sizeof(cc)) < 0) perror_exit("write crowd_client");
	          int i;
	          for(i = 1 ; i < crowd_client ; i++){
		      tuple t = take_element(&list, i);
		      inet_pton(AF_INET, t.Ip, (void *)&ip_b);
		      port = htonl(t.port);

		      if( write(newsock, &ip_b, sizeof(ip_b)) < 0) perror_exit("write ip");
		      if( write(newsock, &port, sizeof(port)) < 0) perror_exit("write port");
	          }
	          if(crowd_client > 1) printf("server send CLIENT_LIST \n");

	      }else{
                  int ok = -1;
	          ok = htonl(ok);
	          if( write(newsock, &ok, sizeof(ok)) < 0) perror_exit("write ok -1");
	          printf("client exist in list. \n");
	      }

          }else if(!strcmp(buf, log_off)){ // LOG_OFF

	      struct sockaddr_in *sin = (struct sockaddr_in *) clientptr;
  	      char *iip = inet_ntoa(sin->sin_addr);
	      int port;
              if( read(newsock, &port, sizeof(port)) < 0) perror_exit("read ip log_on");
	      port = ntohl(port);
	      printf("Client on %s  and port  %d  log_off  \n", iip, port);

	      if(find_client(&list,iip, port)){
	          int f_c = find_client(&list,iip, port);
	          diagrafi_komvou(&list, f_c);
	          if(s_list(&list) > 0){

		      switch( (child_pid = fork()) ){
                          case -1:
                              perror_exit("fork error ");
                          case 0:{
                              send_log_off(&list, iip, port);
                              lista_diagrafi(&list);
                              exit(0);
                          }
                      }

	              printf("Send LOG_OFF in all clients. \n");
	          }
	      }else{
	          if( write(newsock, "ERROR_IP_PORT_NOT_FOUND_IN_LIST", 31) < 0)
		      perror_exit("write  ERROR_IP_PORT_NOT_FOUND_IN_LIST");
	      }
          }
          close(newsock);
      }
      usleep(50000);
  }


  while ((wpid = wait(&status)) > 0);
  // delete list with clients
  lista_diagrafi(&list);
  exit(0);
}
