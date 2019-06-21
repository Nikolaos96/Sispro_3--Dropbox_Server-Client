#ifndef __LISTA2__
#define __LISTA2__


typedef struct info_node * info_deikti2;
typedef struct typos_komvou * typos_deikti2;


info_deikti2 LIST_dimiourgia_v(info_deikti2*);
void eisagogi_v(info_deikti2* linfo, char *file, int vers);

void emfanisi_v(info_deikti2* linfo);
void lista_diagrafi_v(info_deikti2* linfo);

void change_version_v(info_deikti2* linfo, char *file, int vers);
int take_version_v(info_deikti2* linfo, char *file);

#endif
