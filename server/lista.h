#ifndef __LISTA__
#define __LISTA__

typedef struct info_node * info_deikti;
typedef struct typos_komvou * typos_deikti;

typedef struct{
    char Ip[51];
    int  port;
}tuple;


info_deikti LIST_dimiourgia(info_deikti*);
void eisagogi(info_deikti* linfo, char *ip, int portt);
void emfanisi(info_deikti* linfo);
void lista_diagrafi(info_deikti* linfo);
void diagrafi_komvou(info_deikti* linfo, int komvos);
int find_client(info_deikti* linfo, char *ip, int port);
int s_list(info_deikti* linfo);
tuple  take_element(info_deikti *linfo, int elem);

#endif
