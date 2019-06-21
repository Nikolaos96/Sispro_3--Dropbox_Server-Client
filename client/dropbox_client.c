/* dropbox_client.c */
#include <stdio.h>
#include "functions.h"
#include "lista.h"
#include "lista2.h"



 char log_on[] = "LOG_ON.....";
 char get_clients[] = "GET_CLIENTS";
 char log_off[] = "LOG_OFF....";
 char user_off[] = "USER_OFF...";



 void main(int argc, char *argv[]){
  int portNum, workerThreads, bufferSize, serverPort, sock;
  char *dirName, *serverIP;

  srand(time(NULL));
  take_argument(argc, argv, &dirName, &portNum, &workerThreads, &bufferSize, &serverPort, &serverIP);
  signal(SIGINT, stopp);


  stop = 1;
  // init with random version my files
  my_files(dirName, rand() % 100);


  // create dir for files from others clients
  create_dir_for_client(take_my_ip(), portNum);
  strcpy(dir_name, dirName);


  // create list
  list = LIST_dimiourgia(&list);
  // create list for version files
  list_v = LIST_dimiourgia_v(&list_v);


  struct sockaddr_in server;
  struct sockaddr *serverptr=(struct sockaddr *) &server;
  struct hostent *rem;
  if((sock = socket(AF_INET , SOCK_STREAM , 0)) < 0)  perror_exit("socket");
  if(( rem = gethostbyname(serverIP)) == NULL){
      herror("gethostbyname");
      exit(1);
  }


  server.sin_family = AF_INET;
  memcpy(&server.sin_addr, rem->h_addr, rem->h_length);
  server.sin_port = htons(serverPort);
  if(connect(sock, serverptr, sizeof(server)) < 0) perror_exit("connect  send LOG_ON");


  if(write(sock, log_on, 11) < 0) perror_exit("write LOG_ON");
  char *my_ip_string = take_my_ip();
  int my_ip;
  inet_pton(AF_INET, my_ip_string, (void *)&my_ip);
  int my_port;
  my_port = htonl(portNum);



  if(write(sock, &my_ip, sizeof(my_ip)) < 0) perror_exit("write my_ip");
  if(write(sock, &my_port, sizeof(my_port)) < 0) perror_exit("write my_port");


  int ok;
  if(read(sock, &ok, sizeof(ok)) < 0) perror_exit("read  ok");
  if(ntohl(ok) == -1){
      close(sock);
      perror_exit("Error.I can't continue.");
  }


  char buf1[11];
  if(read(sock, buf1, 11) < 0) perror_exit("read  GET_CLIENTS");
  int crowd_clientss;
  if(read(sock, &crowd_clientss, sizeof(crowd_clientss)) < 0) perror_exit("read  crowd_clientss");



  int c_ip, c_port, i;
  for(i = 0 ; i < crowd_clientss ; i++){

      if(read(sock, &c_ip, sizeof(c_ip)) < 0) perror_exit("read IP");
      if(read(sock, &c_port, sizeof(c_port)) < 0) perror_exit("read portNum");

      char str_c_ip[50];
      inet_ntop(AF_INET, (void *)&c_ip, str_c_ip, 50);

      printf("%s   %d \n", str_c_ip, ntohl(c_port));
      eisagogi(&list, str_c_ip, ntohl(c_port));
  }
  close(sock);



  // init semaphore for list and buffer  and list with versions
  int retval = sem_init(&sem_list, 1, 1);
  if(retval) perror_exit("error  sem_list  init. \n");
  retval = sem_init(&sem_buf, 1, 1);
  if(retval) perror_exit("error  sem_buf  init. \n");
  retval = sem_init(&sem_list_v, 1, 1);			/////////////////////////////
  if(retval) perror_exit("error  sem_list_v  init. \n");////////////////////////////



  // create buffer
  create_buffer(bufferSize);
  for(i = 1 ; i <= s_list(&list) ; i++){
      tuple t = take_element(&list,i);

      strcpy(buffer[buffer_tail].IP_address, t.Ip);
      buffer[buffer_tail].portNum = t.port;
      strcpy(buffer[buffer_tail].pathname, "NULL");
      buffer[buffer_tail].version = 0;

      add_buffer();
  }


  // create threads
  pthread_t *tids;
  if((tids = malloc(workerThreads * sizeof(pthread_t))) == NULL)   perror_exit("malloc threads");
  int err;
  for(i = 0 ; i < workerThreads ; i++){
      if(err = pthread_create(tids+i, NULL, worker_function, NULL)) perror_exit("error pthread_create");
  }



  client_request(dirName, portNum, serverIP);    // Log-on, Log-off, Get cliets file, Get files



  if((sock = socket(AF_INET , SOCK_STREAM , 0)) < 0)  perror_exit("socket");
  if(( rem = gethostbyname(serverIP)) == NULL){
      herror("gethostbyname");
      exit(1);
  }
  server.sin_family = AF_INET;
  memcpy(&server.sin_addr, rem->h_addr, rem->h_length);
  server.sin_port = htons(serverPort);
  if(connect(sock, serverptr, sizeof(server)) < 0) perror_exit("connect  send LOG_ON");



  // send LOG_OFF
  if(write(sock, log_off, 11) < 0) perror_exit("write LOG_ON");
  my_port = htonl(portNum);
  if(write(sock, &my_port, sizeof(my_port)) < 0) perror_exit("write LOG_ON");
  close(sock);



  // wait all threads
  for(i = 0 ; i < workerThreads ; i++){
      if(err = pthread_join(*(tids+i), NULL))
	  perror_exit("error pthread_join");
  }


  // delete mutex list, mutex buf and mutex foe list with versions
  sem_destroy(&sem_buf);
  sem_destroy(&sem_list);
  sem_destroy(&sem_list_v);


  free(tids);
  free(dirName);
  free(serverIP);


  // delete buffer
  remove_buffer();
  // delete list
  lista_diagrafi(&list);
  // delete list_v
  lista_diagrafi_v(&list_v);
  del_my_files();
 }
