// permitir offsets de 64bits (tamaños mayores de 4GB)
#define _FILE_OFFSET_BITS 64

#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#include "Ficheros.h"


// Funcion para recorrer recursivamente un directorio
void Recurse_Dir(char *dir_orig,Recurse_Dir_func func,void *data){
	DIR *directorio;
	struct dirent *entidad_dir;
	char dir[1024];
	char path[1024];
	char file[255];
	struct stat stat_info;

	// poner la bara final del directorio
	strcpy(dir,dir_orig);
	if(dir_orig[strlen(dir_orig)-1]!='/'){
		strcat(dir,"/");	
	}
	directorio=opendir(dir);
	if(directorio!=NULL){
		do{
			entidad_dir=readdir(directorio);
			if(entidad_dir!=NULL){ 
				if(strcmp(entidad_dir->d_name,".") && 
				   strcmp(entidad_dir->d_name,"..")){
					// costruir el path y fichero
					strcpy(path,dir);
					strcat(path,entidad_dir->d_name);
					strcpy(file,entidad_dir->d_name);
					// llamar a la funcion
					func(path,file,data);
					// comprobar si es un directorio
					lstat(path, &stat_info);
					if(S_ISDIR(stat_info.st_mode) &&
					   !(stat_info.st_mode&S_IFLNK)){
						// es un directorio recursivar
						Recurse_Dir(path,func,data);
					}
				}
			}
		}while(entidad_dir!=NULL);
		closedir(directorio);
	}
}

// funcion para recorrer un directorio
// (la recursividad la determina la funcion, si la necesitara)
void Dir_ForEach(char *path,Dir_ForEach_func func,void *data){
	DIR *directorio;
	struct dirent *entidad_dir;
	char f_path[1024];

	directorio=opendir(path);
	if(directorio!=NULL){
		do{
			entidad_dir=readdir(directorio);
			if(entidad_dir!=NULL){ 
				if(strcmp(entidad_dir->d_name,".") &&
				   strcmp(entidad_dir->d_name,"..")){
					// Apartir de aqui hay un fichero(o directorio)
					strcpy(f_path,path);
					strcat(f_path,"/");
					strcat(f_path,entidad_dir->d_name);
					func(f_path,entidad_dir->d_name,data);
				}
			}
		}while(entidad_dir!=NULL);
		closedir(directorio);
	}
}



int ExisteFichero(char *string){
	int existe=0;
	FILE *fil;
	fil=fopen(string,"r");
	if(fil!=NULL){
		fclose(fil);
		existe=1;
	}
	return(existe);
}

int EsDirectorio(char *string){
	struct stat stat_info;	
	lstat(string, &stat_info);
	if(S_ISDIR(stat_info.st_mode) &&
	   !(stat_info.st_mode&S_IFLNK)){
		return(1);
	}else{
		return(0);
	}
}


void borrado_recursivo_func(char *path,char *name,void *data){
	if(EsDirectorio(path))
		Dir_ForEach(path,borrado_recursivo_func,NULL);
	remove(path);
}

void borrado_recursivo(char *path){
	Dir_ForEach(path,borrado_recursivo_func,NULL);
	remove(path);
}



