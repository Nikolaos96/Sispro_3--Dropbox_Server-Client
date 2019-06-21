/* functions.h */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include "lista.h"
#include "lista2.h"
#include <semaphore.h>
#include <sys/ipc.h>
#include <poll.h>
#include <sys/time.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>



 sem_t sem_list;
 info_deikti list;
 int stop;

 sem_t sem_list_v;
 info_deikti2 list_v;


 int size_ver;
 int *versions;
 char **names_files;


 char my_file_clients[50];
 char dir_name[50];


 typedef struct{
     char pathname[128];
     int version;
     char IP_address[50];
     int portNum;
 }element;



 sem_t sem_buf;
 int buffer_size;
 int buffer_head;
 int buffer_tail;
 element *buffer;


 void take_argument(int argc,char *argv[],char **dirname, int *portNum, int *threads,
                    int *bufSize, int *postServer, char **serverIp);

 void perror_exit(char *);
 int compareFile(char *file1, char *file2);
 int number_files(char *);

 void create_buffer(int size);
 void remove_buffer(void);
 int buffer_empty(void);
 int buffer_full(void);
 void add_buffer(void);
 void delete_buffer(void);


 char * take_ip(char *ip);
 char * take_my_ip(void);
 void stopp(int sig);

 int take_length_file(char *f);
 void * worker_function(void *args);

 int ver_sion(char *file);
 void client_request(char *dirName, int my_port, char *serverIp);

 char * hostname(char *ip);
 void create_dir_for_client(char *ip, int port);

 void my_files(char *my_dir, int vv);
 void del_my_files(void);
