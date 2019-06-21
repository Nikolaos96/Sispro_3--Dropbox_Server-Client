/* functions.c */
#include "functions.h"



 void take_argument(int argc,char *argv[],char **dirname, int *portNum, int *threads,
		    int *bufSize, int *postServer, char **serverIp)
 {
     char *arg;
     if(argc != 13 ){
         printf("\nError in arguments command line. \n\n");
         exit(EXIT_FAILURE);
     }

     while(--argc){
          arg = *++argv;
          if(!strcmp(arg, "-d")){
	      *dirname = malloc((strlen(*++argv) + 1) * sizeof(char));
	      strcpy(*dirname, *argv);
          }else if(!strcmp(arg, "-p")){
	      *portNum = atoi(*++argv);
          }else if(!strcmp(arg, "-w")){
	      *threads = atoi(*++argv);
          }else if(!strcmp(arg, "-b")){
	      *bufSize = atoi(*++argv);
	  }else if(!strcmp(arg, "-sp")){
	      *postServer = atoi(*++argv);
	  }else if(!strcmp(arg, "-sip")){
	      *serverIp = malloc((strlen(*++argv) + 1) * sizeof(char));
	      strcpy(*serverIp, *argv);
	  }
          if(argc > 1) argc--;
     }
     return;
 }



 void perror_exit(char *mes){
     perror(mes);
     exit(EXIT_FAILURE);
 }



 // return number of files that the client has
 int number_files(char *my_dir){

     struct dirent *dirEntry;
     DIR *dir = opendir(my_dir);

     int count = 0;
     while( (dirEntry = readdir(dir)) != 0 ){
         if( !strcmp(dirEntry->d_name, ".") || !strcmp(dirEntry->d_name, "..") ) continue;
         count++;
     }
     closedir(dir);
     return count;
 }



 // buffer functions
 //$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 void create_buffer(int size){
     buffer_size = size;
     buffer_head = 0;
     buffer_tail = 0;

     buffer = malloc(size * sizeof(element));
     if(buffer == NULL) perror_exit("Error create.Malloc error.");
 }



 void remove_buffer(void){
     free(buffer);
 }



 int buffer_empty(void){
     if(buffer_head == buffer_tail) return 1;
     else return 0;
 }



 int buffer_full(void){
     if(buffer_head == ((buffer_tail + 1) % buffer_size)) return 1;
     else return 0;
 }



 void add_buffer(void){
     buffer_tail = (buffer_tail + 1) % buffer_size;
 }



 void delete_buffer(void){
     buffer_head = (buffer_head + 1) % buffer_size;
 }
 //$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$



 //return ip from linuxXX.di.uoa.gr
 char * take_ip(char *ip){

     struct hostent *hp = gethostbyname(ip);
     unsigned int i=0;
     char A[30], *B = A;

     while(hp->h_addr_list[i] != NULL){
         sprintf(A, "%s ", inet_ntoa( *( struct in_addr*)( hp -> h_addr_list[i]) ));
         i++;
     }
     return B;
 }



 //return ip client
 char * take_my_ip(void){
     char hostbuffer[50];
     char *IPbuffer;
     struct hostent *host_entry;
     int hostname;

     hostname = gethostname(hostbuffer, sizeof(hostbuffer));
     host_entry = gethostbyname(hostbuffer);
     IPbuffer = inet_ntoa(*((struct in_addr*)host_entry->h_addr_list[0]));

     char *bb = IPbuffer;
     return bb;
 }



 // signal handler
 void stopp(int sig){
     stop = 0;
 }



 // return the size of a folder
 int take_length_file(char *f){
     FILE *fp = fopen(f, "r");

     fseek(fp, 0, SEEK_END);
     int size = ftell(fp);
     fseek(fp, 0, SEEK_SET);

     fclose(fp);
     return size;
 }



 // threads function
 void * worker_function(void *args){

     sleep(2);

     while(stop){
         sem_wait(&sem_buf);
         if(buffer_empty() == 0){ // den einai adios

	     element el = buffer[buffer_head];
	     delete_buffer();
	     sem_post(&sem_buf);

	     sem_wait(&sem_list);
	     int find = find_client(&list, el.IP_address, el.portNum);
	     sem_post(&sem_list);
	     if(find){ // client exist in list
		 int sock, i;

		 struct sockaddr_in server;
                 struct sockaddr *serverptr=(struct sockaddr *) &server;
		 if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) perror_exit("socket GET_FILE_LIST/GET_FILE");
		 server.sin_family = AF_INET;
         	 server.sin_addr.s_addr = inet_addr(el.IP_address);
         	 server.sin_port = htons(el.portNum);
		 if( connect(sock, serverptr, sizeof(server)) < 0 ) perror_exit("connect GET_FILE_LIST/GET_FILE");


	         if( (!strcmp(el.pathname, "NULL")) && (el.version == 0) ){
		     if(write(sock, "GET_FILE_LIST", 13) < 0) perror_exit("write GET_FILE_LIST");
		     char B[10];
		     if(read(sock, B, 9) < 0) perror_exit("read FILE_LIST");
		     int number_of_files;
		     if(read(sock, &number_of_files, sizeof(number_of_files)) < 0) perror_exit("read number_of_files error");


		     int vers, length_name;
		     element *lll = malloc(number_of_files * sizeof(element));

		     for(i = 0 ; i < number_of_files ; i++){
			 if(read(sock, &length_name, sizeof(length_name)) < 0) perror_exit("read file length");
			 char *name_file = malloc(length_name * sizeof(char));

		         if(read(sock, name_file, length_name) < 0) 	       perror_exit("read file name");
			 if(read(sock, &vers, sizeof(vers)) < 0) 	       perror_exit("read version");

			 strcpy(lll[i].pathname, name_file);
			 lll[i].version = vers;
                         strcpy(lll[i].IP_address, el.IP_address);
                         lll[i].portNum = el.portNum;

			 free(name_file);
		     }
                     close(sock);

		     int full;
		     for(i = 0 ; i < number_of_files ; i++){
			 sem_wait(&sem_buf);
			 full = buffer_full();
			 if(full == 0){
			     buffer[buffer_tail] = lll[i];
			     add_buffer();
			 }
			 sem_post(&sem_buf);
			 //else   se periptosi pou o buffer einai gematos
                         //int b = 0;
			 //while(b == 0){
			   //  sem_wait(&sem_buf);
			   //  full = buffer_full();
			   //  if(full == 0){
                           //      buffer[buffer_tail] = lll[i];
                           //      add_buffer();
			   //      b = 1;
			   //  }
			   //  sem_post(&sem_buf);
			   //  sleep(1);
                         //}
		     }
		     free(lll);

	         }else{

		     char file[50];
		     strcpy(file, my_file_clients);
		     strcat(file, "/");
		     strcat(file, el.pathname);
		     FILE *fp = fopen(file, "r");
		     int exist;
		     if(fp == NULL) exist = 0;
		     else{
		         exist = 1;
			 fclose(fp);
		     }

		     if(write(sock, "GET_FILE.....", 13) < 0) perror_exit("write GET_FILE");
		     int k = strlen(el.pathname);
		     if(write(sock, &k, sizeof(k)) < 0) perror_exit("write len_file");
		     if(write(sock, el.pathname, strlen(el.pathname)) < 0) perror_exit("write file name");

		     int go;
		     if( read(sock, &go, sizeof(go)) < 0 ) perror_exit("read go ");
		     if(go == 0){
			 char cc[14];
		         read(sock, cc, 14);
			 printf("%s \n", cc);
		     }

		     printf("\n ------   OK    --------   \n");

                     if(exist == 0 && go == 1){

	                 k = -1; // stelnw tin version = -1 epeidi den yparxei to arxeio
		         if(write(sock, &k, sizeof(k)) < 0) perror_exit("write version");
			 int length_file;
			 if( read(sock, &length_file, sizeof(length_file)) < 0 ) perror_exit("read length_file");
			 int verssion;
			 if( read(sock, &verssion, sizeof(verssion)) < 0 ) perror_exit("read versionn");


			 int filedes;
			 if((filedes = open(file, O_CREAT|O_RDWR|O_APPEND,0644)) == -1) perror_exit("open/create file");


			 sem_wait(&sem_list_v);
			 eisagogi_v(&list_v, el.pathname, verssion);
			 sem_post(&sem_list_v);


			 char c;
			 while( length_file-- > 0 ){
			     if( read(sock, &c, sizeof(c)) < 0 ) perror_exit("read byte from sock");
			     if( write(filedes, &c, sizeof(c)) < 0 ) perror_exit("write byte in file");
			 }
			 close(filedes);


		       }else if(exist == 1 && go == 1){


			 k = take_version_v(&list_v, el.pathname);
			 if(write(sock, &k, sizeof(k)) < 0) perror_exit("write version");
			 int copy;
			 if(read(sock, &copy, sizeof(copy)) < 0) perror_exit("read copy");

			 if(copy == 1){ // antigrafi

 			     printf("New version \n");

			     int length_file;
                             if( read(sock, &length_file, sizeof(length_file)) < 0 ) perror_exit("read length_file");
			     int verssion;
                             if( read(sock, &verssion, sizeof(verssion)) < 0 ) perror_exit("read versionn");
			     int filedes;
                             if((filedes = open(file, O_CREAT|O_RDWR,0644)) == -1) perror_exit("open/create file");


			     change_version_v(&list_v, el.pathname, verssion);


			     char c;
                             for(int j = 0 ; j < length_file ; j++){
                                 if( read(sock, &c, sizeof(c)) < 0 ) perror_exit("read byte from sock");
                                 if( write(filedes, &c, sizeof(c)) < 0 ) perror_exit("write byte in file");
                             }
			     close(filedes);


			 }else{ // den xreiazetai antigrafi einai entaxei
			     char bb[15];
			     if(read(sock, bb, 15) < 0) perror_exit("read FILE_UP_TO_DATE");
			     printf("%s\n", bb);
			 }
		     }

	             close(sock);
		     sleep(2);
	         }
	     }else // client does not exist in list
	         printf("Client on ip %s and port %d leave.", el.IP_address, el.portNum);
	 }else{
	     sem_post(&sem_buf);
	     printf("Buffer is empty. \n");
             sleep(2);
	 }
     }

     pthread_exit(NULL);
 }



 // return the version of the file
 int ver_sion(char *file){
     for(int i = 0 ; i < size_ver ; i++){
         if( !strcmp(names_files[i], file) ) return versions[i];
     }

 }



 // client answer request
 void client_request(char *dirName, int my_port, char *serverIp){
   int sock, newsock;
   struct sockaddr_in server, client;
   socklen_t clientlen;
   struct sockaddr *serverptr =(struct sockaddr *) &server;
   struct sockaddr *clientptr =(struct sockaddr *) &client;


   if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) perror_exit("socket");
   int k = 1;
   if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR,&k, sizeof(int)) < 0) perror_exit("setsockopt");


   server.sin_family = AF_INET;
// server.sin_addr.s_addr = inet_addr(take_my_ip());
   server.sin_addr.s_addr = htonl(INADDR_ANY);
   server.sin_port = htons(my_port);

   if(bind(sock, serverptr, sizeof(server)) < 0) perror_exit("bind");
   if(listen(sock, 5) < 0) perror_exit("listen");


   struct pollfd pol[1];
   pol[0].fd = sock;
   pol[0].events = POLLIN;


   char buf[13];
   while(stop){

       int j = poll(pol, 1, 0);
       if(j == 0){
	   usleep(5000);
       }else if(j == -1){
	   printf("Poll error \n");
       }else if( (j == 1) && (pol[0].revents == POLLIN) && (pol[0].fd == sock) ){
           if((newsock = accept(sock, clientptr, &clientlen)) < 0) perror_exit("accept");
           if( read(newsock, buf, sizeof(buf)) < 0) perror_exit("read");



           if( !strcmp(buf, "USER_OFF.....") ){


	       char ip[50];
	       int ip_b;
	       if( read(newsock, &ip_b, sizeof(ip_b)) < 0) perror_exit("read ip LOG_OFF");
 	       inet_ntop(AF_INET, (void *)&ip_b, ip, 50);

	       int pport;
	       if( read(newsock, &pport, sizeof(pport)) < 0) perror_exit("read port LOG_OFF");
	       pport = ntohl(pport);

	       sem_wait(&sem_list);
	       int kk = find_client(&list,ip, pport);
	       diagrafi_komvou(&list, kk);
	       sem_post(&sem_list);
	       printf("Client with ip: %s  and port: %d  LOG_OFF \n", ip, pport);
	       printf("Delete from my list client on IP: %s and port:  %d \n", ip, pport);



           }else if( !strcmp(buf, "USER_ON......") ){


  	       char ip[50];
	       int ip_b;
	       int pport;
	       if( read(newsock, &ip_b, sizeof(ip_b)) < 0) perror_exit("read ip USER_ON ip");
	       if( read(newsock, &pport, sizeof(pport)) < 0) perror_exit("read ip USER_ON port");
	       inet_ntop(AF_INET, (void *)&ip_b, ip, 50);
	       pport = ntohl(pport);

	       sem_wait(&sem_list);
	       eisagogi(&list, ip, pport);
	       sem_post(&sem_list);


	       sem_wait(&sem_buf);
  	       if( buffer_full() == 0 ){
	           buffer[buffer_tail].version = 0;
	           buffer[buffer_tail].portNum = pport;
	           strcpy(buffer[buffer_tail].IP_address, ip);
	           strcpy(buffer[buffer_tail].pathname, "NULL");
	           add_buffer();
	           sem_post(&sem_buf);

	       }else{ // o buffer einai gematos
		   sem_post(&sem_buf);
		   // ....
	       }
	       printf("Client with ip: %s  and port: %d  LOG_ON \n", ip, pport);
	       printf("Insert in my list client on ip:  %s  and port:  %d \n", ip, pport);



           }else if( !strcmp(buf, "GET_FILE_LIST") ){

	       if( write(newsock, "FILE_LIST", 9) < 0 ) perror_exit("write FILE_LIST");
	       int number_of_files = number_files(dirName);
	       if( write(newsock, &number_of_files, sizeof(number_of_files)) < 0 ) perror_exit("write number_of_files");

	       DIR *dir = opendir(dirName);
	       struct dirent *dirEntry;

	       while( (dirEntry = readdir(dir)) != 0 ){
	           if( !strcmp(dirEntry->d_name, ".") || !strcmp(dirEntry->d_name, "..") ) continue;

		   char B[50];
                   strcpy(B, dirEntry->d_name);
		   int ss = strlen(B);
		   int ver = versions[ver_sion(B)];

		   if( write(newsock, &ss, sizeof(ss)) < 0 ) perror_exit("write length_file");
		   if( write(newsock, B, ss) < 0 ) perror_exit("write name_file");
		   if( write(newsock, &ver, sizeof(ver)) < 0 ) perror_exit("write version");

     	       }
	       closedir(dir);


           }else if( !strcmp(buf, "GET_FILE.....") ){


	       char file[50], f_file[50];
	       int size_name, go;
	       if( read(newsock, &size_name, sizeof(size_name)) < 0 ) perror_exit("read file size");
	       if( read(newsock, file, size_name) < 0 ) perror_exit("read file name");

               strcpy(f_file, dir_name);
	       strcat(f_file, "/");
	       strcat(f_file, file);
	       FILE *f = fopen(f_file, "r");

	       if(f == NULL){
		   go = 0;
		   if( write(newsock, &go, sizeof(go)) < 0 ) perror_exit("write go = 0");
		   if( write(newsock, "FILE_NOT_FOUND", 14) < 0 ) perror_exit("write  FILE_NOT_FOUND");
	       }else{
		   fclose(f);
		   go = 1;
		   if( write(newsock, &go, sizeof(go)) < 0 ) perror_exit("write go = 1");
		   int ver;
		   if( read(newsock, &ver, sizeof(ver)) < 0 ) perror_exit("read version");

		   if(ver == -1){

		       int length_file = take_length_file(f_file);
		       ver = ver_sion(file);
		       if( write(newsock, &length_file, sizeof(length_file)) < 0 ) perror_exit("write length file");
		       if( write(newsock, &ver, sizeof(ver)) < 0 ) perror_exit("write vesion");

		       int file_sen = open(f_file, O_RDWR);
		       char c;
		       while( length_file-- > 0 ){
			   read(file_sen, &c, sizeof(c));
			   write(newsock, &c, sizeof(c));
		       }
		       close(file_sen);


		   }else{


		       int copy;
		       int vers = ver_sion(file);
		       if(vers == ver) copy = 0;
		       else copy = 1;
		       if( write(newsock, &copy, sizeof(copy)) < 0 ) perror_exit("write copy");


		       if(copy == 1){

			   printf("New version \n");

			   int length_file = take_length_file(f_file);
			   if( write(newsock, &length_file, sizeof(length_file)) < 0 ) perror_exit("write length file");
			   if( write(newsock, &vers, sizeof(vers)) < 0 ) perror_exit("write vesion");

	                   int file_sen = open(f_file, O_RDWR);
                           char c;
			   for(int j = 0 ; j < length_file ; j++){
                               read(file_sen, &c, sizeof(c));
                               write(newsock, &c, sizeof(c));
                           }
                           close(file_sen);

		       }else{
		           if( write(newsock, "FILE_UP_TO_DATE", 15) < 0 ) perror_exit("");
		       }
		   }
	       }
           }
	   close(newsock);
       }
   }
 }



 //take ip and return hostname
 char * hostname(char *ip){
     struct hostent *foundhost;
     struct in_addr myaddress;
     inet_aton(ip, &myaddress);
     foundhost = gethostbyaddr((const char *)&myaddress, sizeof(myaddress), AF_INET);

     return foundhost->h_name;
 }



 //create dir for folders from the other clients
 void create_dir_for_client(char *ip, int port){
     char A[50];
     strcpy(A, "Client_");
     strcat(A, hostname(ip));

     strcat(A,"_");
     char B[50];
     sprintf(B, "%d", port);
     strcat(A, B);

     strcpy(my_file_clients, A);

     DIR *dir;
     if( (dir = opendir(A)) == NULL ){
         mkdir(A, 0700);
         return;
     }
     closedir(dir);
 }



 // first version for each folder that the clent has
 void my_files(char *my_dir, int vv){

     struct dirent *dirEntry;
     DIR *dir = opendir(my_dir);

     int count = 0, i;
     while( (dirEntry = readdir(dir)) != 0 ){
         if( !strcmp(dirEntry->d_name, ".") || !strcmp(dirEntry->d_name, "..") ) continue;
         count++;
     }
     closedir(dir);

     size_ver = count;
     versions = malloc(size_ver * sizeof(int));
     if(versions == NULL) perror_exit("malloc versions error");
     for(i = 0 ; i < size_ver ; i++) versions[i] = vv;


     names_files = malloc(size_ver * sizeof(char*));
     if(names_files == NULL) perror_exit("malloc names_files error");
     for(i = 0 ; i < size_ver ; i++)
         names_files[i] = malloc(50 * sizeof(char));


     dir = opendir(my_dir);
     i = 0;
     while( (dirEntry = readdir(dir)) != 0 ){
        if( !strcmp(dirEntry->d_name, ".") || !strcmp(dirEntry->d_name, "..") ) continue;
        strcpy(names_files[i], dirEntry->d_name);
	i++;
     }
     closedir(dir);

     return;
 }



 void del_my_files(void){
     free(versions);

     int i;
     for(i = 0 ; i < size_ver ; i++) free(names_files[i]);
     free(names_files);
     return;
 }

