#include <stdio.h>
#include <string.h>


#include "StringUtil.h"



void basename(char *nombre,char *basename){
	int a,b,c;
	int len;
	int enc=0;
	len=strlen(nombre);
	a=len-1;
	while(a>-1 && !enc){
		if(nombre[a]!='/')
			a--;
		else
			enc=1;
	}
	if(enc){
		c=0;
		for(b=a+1;b<len;b++){
			basename[c]=nombre[b];
			c++;
		}
		basename[c]=0;
	}else{
		strcpy(basename,nombre);
	}
}



void coger_marcado(char *cadena,int inicio,int fin,char *destino){
	int a,b=0;
	for(a=inicio;a<=fin;a++){
		destino[b]=cadena[a];
		b++;
	}
	destino[b]=0;
}



void coger_entrecomillado(char *cadena,int *origen,char *dest){
	int pri=0,seg=0;
	int enc=0;
	int len=strlen(cadena);

	// buscar primera comilla
	pri=origen[0];
	while(pri<len && !enc){
		if(cadena[pri]=='"'){
			enc=1;
		}else{
			pri++;
		}
	}


	// buscar segunda comilla
	enc=0;
	seg=pri+1;
	while(seg<len && !enc){
		if(cadena[seg]=='"'){
			enc=1;
		}else{
			seg++;
		}
	}
	
	coger_marcado(cadena,pri+1,seg-1,dest);

	*origen=seg+1;
}
