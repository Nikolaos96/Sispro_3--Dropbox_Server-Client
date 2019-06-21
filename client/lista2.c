//File: lista.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista2.h"


 typedef struct info_node{
 	int size;
	typos_deikti2 arxi;
	typos_deikti2 telos;
 }info_node;



 typedef struct typos_komvou{
 	char name_file[50];
	int version;

	typos_deikti2 epomenos;
	typos_deikti2 proigoumenos;
 }typos_komvou;



 info_deikti2 LIST_dimiourgia_v(info_deikti2* lista){

 	info_deikti2 linfo;
 	linfo = malloc(sizeof(info_node));
 	linfo->size = 0;
 	linfo->arxi = NULL;
	linfo->telos = NULL;
 	return linfo;
 }



 void eisagogi_v(info_deikti2* linfo,char *file, int vers){

 	typos_deikti2 prosorinos;
 	prosorinos = malloc(sizeof(typos_komvou));

	strcpy(prosorinos->name_file, file);
 	prosorinos->version = vers;

 	if((*linfo)->size == 0){
 		(*linfo)->arxi = prosorinos;
		(*linfo)->telos = prosorinos;
		prosorinos->epomenos = NULL;
		prosorinos->proigoumenos = NULL;

 	}
 	if((*linfo)->size > 0){
		prosorinos->proigoumenos = (*linfo)->telos;
		prosorinos->epomenos = NULL;
 		(*linfo)->telos->epomenos = prosorinos;
		(*linfo)->telos = prosorinos;
 	}
 	(*linfo)->size++;
 }



 // for debug
 void emfanisi_v(info_deikti2* linfo){

	typos_deikti2 mapas = (*linfo)->arxi;
	printf(" \n ");
 	while(mapas != NULL){
 		printf("%s    %d\n", mapas->name_file, mapas->version);
		mapas = mapas->epomenos;
 	}
	printf(" \n ");
 }



 void lista_diagrafi_v(info_deikti2* linfo){

 	typos_deikti2 mapas = (*linfo)->arxi;
	typos_deikti2 mapas2;

 	while(mapas != NULL){
		mapas2 = mapas;
		mapas = mapas->epomenos;
		free(mapas2);
	}
	(*linfo)->arxi = NULL;
	(*linfo)->telos = NULL;
	(*linfo)->size = 0;
	free(*linfo);
	(*linfo) = NULL;
 }



 void change_version_v(info_deikti2* linfo, char *file, int vers){

     typos_deikti2 mapas = (*linfo)->arxi;
     while(mapas != NULL){
	 if( !strcmp(mapas->name_file, file) ){
             mapas->version = vers;
	     return;
	 }
         mapas = mapas->epomenos;
     }
 }



 int take_version_v(info_deikti2* linfo, char *file){

     typos_deikti2 mapas = (*linfo)->arxi;
     while(mapas != NULL){
	 if( !strcmp(mapas->name_file, file) ) return mapas->version;
         mapas = mapas->epomenos;
     }
     return 0;
 }

