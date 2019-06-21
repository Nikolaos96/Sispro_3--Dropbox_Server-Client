//File: lista.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista.h"



 typedef struct info_node{
 	int size;
	typos_deikti arxi;
	typos_deikti telos;
 }info_node;



 typedef struct typos_komvou{
 	tuple client_tuple;

	typos_deikti epomenos;
	typos_deikti proigoumenos;
 }typos_komvou;



 info_deikti LIST_dimiourgia(info_deikti* lista){

 	info_deikti linfo;
 	linfo = malloc(sizeof(info_node));
 	linfo->size = 0;
 	linfo->arxi = NULL;
	linfo->telos = NULL;
 	return linfo;
 }



 void eisagogi(info_deikti* linfo, char *ip, int portt){

 	typos_deikti prosorinos;
 	prosorinos = malloc(sizeof(typos_komvou));

 	strcpy(prosorinos->client_tuple.Ip, ip);
	prosorinos->client_tuple.port = portt;

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
 void emfanisi(info_deikti* linfo){

	typos_deikti mapas = (*linfo)->arxi;
	printf(" \n ");
 	while(mapas != NULL){
 		printf("%s  %d\n", mapas->client_tuple.Ip, mapas->client_tuple.port);
		mapas = mapas->epomenos;
 	}
	printf(" \n ");
 }



 void lista_diagrafi(info_deikti* linfo){

 	typos_deikti mapas = (*linfo)->arxi;
	typos_deikti mapas2;

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



 void diagrafi_komvou(info_deikti* linfo, int komvos){
	int k;

 	if(komvos > (*linfo)->size || komvos <= 0 || (*linfo)->size == 0){
		printf("Den yparxei aytos o komvos \n");
		return;
	}else{
		typos_deikti mapas = (*linfo)->arxi;
		typos_deikti mapas2;
		while(komvos-- > 0){
			mapas2 = mapas;
			mapas = mapas->epomenos;
		}
		if(mapas2 == (*linfo)->arxi && mapas2 == (*linfo)->telos){
			free(mapas2);
			(*linfo)->arxi = NULL;
			(*linfo)->telos = NULL;
		}else if(mapas2 == (*linfo)->arxi){
			(*linfo)->arxi = (*linfo)->arxi->epomenos;
			(*linfo)->arxi->proigoumenos = NULL;
			free(mapas2);
		}else if(mapas2 == (*linfo)->telos){
			(*linfo)->telos = (*linfo)->telos->proigoumenos;
			(*linfo)->telos->epomenos = NULL;
			free(mapas2);
		}else{
			mapas2->proigoumenos->epomenos = mapas2->epomenos;
			mapas2->epomenos->proigoumenos = mapas2->proigoumenos;
			free(mapas2);
		}
		(*linfo)->size--;
	}
 }



 int find_client(info_deikti* linfo, char *ip, int port){

     typos_deikti mapas = (*linfo)->arxi;
     int counter = 0;
     while(mapas != NULL){
         if( !strcmp(mapas->client_tuple.Ip, ip) && (mapas->client_tuple.port == port)) return (++counter);
	 counter++;
         mapas = mapas->epomenos;
     }
     return 0;
 }



 int s_list(info_deikti* linfo){
     return (*linfo)->size;
 }



 tuple  take_element(info_deikti* linfo, int elem){

     typos_deikti mapas = (*linfo)->arxi;
     int counter = elem-1;
     while(counter-- > 0){
         mapas = mapas->epomenos;
     }
     return mapas->client_tuple;
 }


