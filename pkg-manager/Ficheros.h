#ifndef _FICHEROS_H_
#define _FICHEROS_H_

#include <stdio.h>

// Funcion para recorrer recursivamente un directorio
typedef void (*Recurse_Dir_func)(char *path,char *name,void *data);
void Recurse_Dir(char *dir_orig,Recurse_Dir_func func,void *data);

// funcion para recorrer un directorio
// (la recursividad la determina la funcion, si la necesitara)
typedef void (*Dir_ForEach_func)(char *path,char *name,void *data);
void Dir_ForEach(char *path,Dir_ForEach_func func,void *data);


int ExisteFichero(char *string);

int EsDirectorio(char *string);

void borrado_recursivo(char *path);
#endif
