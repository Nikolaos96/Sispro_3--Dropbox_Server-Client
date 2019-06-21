/* functions.h */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <netdb.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "lista.h"

 int stop;
 info_deikti list;


 void take_argument(int argc, char *argv[], int *port);
 void perror_exit(char *);
 void send_log_on(info_deikti *list, char *ip, int port);
 void send_log_off(info_deikti *list, char *ip, int port);
 void stopp(int sig);
