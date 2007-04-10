// permitir offsets de 64bits (tamaños mayores de 4GB)
#define _FILE_OFFSET_BITS 64

#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <utime.h>
#include <stdlib.h>
#include <string.h>

#include "StringUtil.h"
#include "Ficheros.h"
#include "pkg-manager.h"



static char system_root[255];
static char pkg_basedir[255];
static char root_list_file[255];




// Estructuras para la lista de ficheros del sistema

Root_File_List *lista_sistema_cargar(){
	Root_File_List *lista_contenedor;
	Root_File_Ent *lista=NULL,*pi=NULL,*aux;
	FILE *fichero;
	char linea[2048];
	int origen;

	fichero=fopen(root_list_file,"r");
	if(fichero!=NULL){
		while(fgets(linea,2048,fichero)!=NULL){
			aux=(Root_File_Ent *)malloc(sizeof(Root_File_Ent));	
			origen=0;
			coger_entrecomillado(linea,&origen,aux->nombre);
			coger_entrecomillado(linea,&origen,aux->propietario);

			aux->next=NULL;
			if(pi==NULL){
				lista=aux;
				pi=lista;
			}else{
				pi->next=(void *)aux;
				pi=aux;
			}
		}
		fclose(fichero);
	}

	lista_contenedor=(Root_File_List *)malloc(sizeof(Root_File_List));
	lista_contenedor->lista=lista;
	lista_contenedor->tail=aux;

	return(lista_contenedor);
}

void lista_sistema_borrar(Root_File_List *lista){
	Root_File_Ent *next,*aux;
	next=lista->lista;
	while(next!=NULL){
		aux=(Root_File_Ent *)next->next;
		free(next);
		next=aux;
	}
	free(lista);
}

Root_File_Ent *lista_sistema_sacar_fichero(Root_File_List *lista,char *fichero){
	Root_File_Ent *aux,*prev=NULL,*pi;
	int enc=0;
	pi=lista->lista;
	
	while(pi!=NULL && !enc){
		if(!strcmp(pi->nombre,fichero)){
			aux=pi;
			enc=1;
		}else{
			prev=pi;
			pi=(Root_File_Ent *)pi->next;
		}
	}
	
	if(enc){
		if(aux==lista->tail){
			lista->tail=prev;
		}
		if(prev!=NULL){
			prev->next=aux->next;
		}else{
			lista->lista=aux->next;
		}
		return(aux);
	}else{
		return(NULL);
	}

}

void lista_sistema_apendizar_fichero(Root_File_List *lista,char *fichero,char *propietario){
	Root_File_Ent *aux;
	
	// anhadir fichero a la lista de ficheros(del sistema)
	aux=(Root_File_Ent *)malloc(sizeof(Root_File_Ent));
	strcpy(aux->nombre,fichero);
	strcpy(aux->propietario,propietario);
		
	aux->next=NULL;
	if(lista->tail==NULL){
		lista->lista=aux;
		lista->tail=aux;
	}else{
		lista->tail->next=(void *)aux;
		lista->tail=aux;
	}

}

void lista_sistema_guardar(Root_File_List *lista){
	Root_File_Ent *aux;
	FILE *fichero;
	aux=lista->lista;

	//fichero=fopen(ROOT_LIST_FILE,"w");
	fichero=fopen(root_list_file,"w");
	if(fichero!=NULL){
		while(aux!=NULL){
			fprintf(fichero,"\"%s\"\t\"%s\"\n",aux->nombre,aux->propietario);
			aux=(Root_File_Ent *)aux->next;
		}
		fclose(fichero);
	}
}




// Estructuras para la lista de ficheros del paquete

Pkg_File_List *pkg_lista_cargar(char *nombre){
	Pkg_File_List *lista_contenedor;
	Pkg_File_Ent *lista=NULL,*pi=NULL,*aux;
	FILE *fichero;
	char path_fichero[1024];
	char linea[1024];
	int a;

	//sprintf(path_fichero,PKG_BASEDIR"%s/lista\0",nombre);
	sprintf(path_fichero,"%s%s/lista\0",pkg_basedir,nombre);

	fichero=fopen(path_fichero,"r");
	if(fichero!=NULL){
		while(fgets(linea,1024,fichero)!=NULL){
			aux=(Pkg_File_Ent *)malloc(sizeof(Root_File_Ent));
			// quitar el salto de linea final
			for(a=strlen(linea)-3;a<strlen(linea);a++){
				if(linea[a]=='\n')
					linea[a]=0;
			}
			// copiar nombre de fichero
			strcpy(aux->nombre,linea);
			// avanzar el la lista
			aux->next=NULL;
			if(pi==NULL){
				lista=aux;
				pi=lista;
			}else{
				pi->next=(void *)aux;
				pi=aux;
			}
		}
		fclose(fichero);
	}

	lista_contenedor=(Pkg_File_List *)malloc(sizeof(Pkg_File_List));
	lista_contenedor->lista=lista;

	return(lista_contenedor);	
}


void pkg_lista_borrar(Pkg_File_List *lista){
	Pkg_File_Ent *next,*aux;
	next=lista->lista;
	while(next!=NULL){
		aux=(Pkg_File_Ent *)next->next;
		free(next);
		next=aux;
	}
	free(lista);
}


void pkg_lista_print(Pkg_File_List *lista){
	Pkg_File_Ent *pi;

	pi=lista->lista;
	while(pi!=NULL){
		printf("%s\n",pi->nombre);
		pi=(Pkg_File_Ent *)pi->next;
	}
	
}







// Funcion para preparar la extraccion de un paquete
void pkg_preparar(char *name){
	char path[1024];

	// crear los directorios
	//sprintf(path,PKG_BASEDIR"%s\0",name);
	sprintf(path,"%s%s\0",pkg_basedir,name);
	mkdir(path,0755);
	//sprintf(path,PKG_BASEDIR"%s/ficheros\0",name);
	sprintf(path,"%s%s/ficheros\0",pkg_basedir,name);
	mkdir(path,0755);
}

// Funcion para extraer un paquete
void pkg_extraer(char *fichero,char *nombre){
	char comando[1024];
	char destdir[1024];
	char listfile[1024];
	// preparar el comando
	sprintf(destdir,"%s%s/ficheros\0",pkg_basedir,nombre);
	sprintf(listfile,"%s%s/lista\0",pkg_basedir,nombre);
	sprintf(comando,"tar xjvfp %s -C %s --index-file %s &> /dev/null\0",fichero,destdir,listfile);


	// extarer el paquete utilizando tar+bzip2
	system(comando);



	// Recoger scripts de instalacion y desinstalacion
	char script_path[1024],script_dest_path[1024];
	sprintf(script_path,"%s%s/ficheros/install.sh\0",pkg_basedir,nombre);
	sprintf(script_dest_path,"%s%s/install.sh\0",pkg_basedir,nombre);
	if(ExisteFichero(script_path)){
		rename(script_path,script_dest_path);
	}

	sprintf(script_path,"%s%s/ficheros/uninstall.sh\0",pkg_basedir,nombre);
	sprintf(script_dest_path,"%s%s/uninstall.sh\0",pkg_basedir,nombre);
	if(ExisteFichero(script_path)){
		rename(script_path,script_dest_path);
	}

}





typedef struct {
	Root_File_List *lista_sistema;
	char pkg_nombre[1024];
	char path[1024];
	char vpath[1024];
} pkg_merge_parms ;


void pkg_merge_func(char *path,char *name,void *data){
	pkg_merge_parms *parms=(pkg_merge_parms *)data;
	char file_dest_path[1024];
	char file_vpath[1024];	
	struct stat stat_info;

	// preparar los paths
	sprintf(file_dest_path,"%s%s/%s\0",system_root,parms->vpath,name);
	strcpy(file_vpath,parms->vpath);
	if(strcmp(file_vpath,"")){
		strcat(file_vpath,"/");
	}
	strcat(file_vpath,name);

	// obtener stat del fichero
	lstat(path, &stat_info);


	if(EsDirectorio(path)){
		int existe=0;
		pkg_merge_parms new_parms;
		// es un directorio
		if(!ExisteFichero(file_dest_path)){
			// crear el directorio inexistente
			mkdir(file_dest_path,0755);
		}else{
			existe=1;
		}
		
		// preparar nuevos parametros
		strcpy(new_parms.path,path);
		strcpy(new_parms.vpath,file_vpath);
		strcpy(new_parms.pkg_nombre,parms->pkg_nombre);
		new_parms.lista_sistema=parms->lista_sistema;

		// recursivar
		Dir_ForEach(path,pkg_merge_func,&new_parms);

		// establecer atributos del directorio origen
		if(!existe){
			struct utimbuf tiempos;
			chmod(file_dest_path,stat_info.st_mode);
			chown(file_dest_path,stat_info.st_uid,stat_info.st_gid);
			tiempos.actime=stat_info.st_atime;
			tiempos.modtime=stat_info.st_mtime;
			utime(file_dest_path,&tiempos);
		}		
	}else{
		struct utimbuf tiempos;
		// es un fichero

		// comprobar si el fichero existia previamente
		if(ExisteFichero(file_dest_path)){
			// el fichero destino existe
			Root_File_Ent *nodo_fichero;
			nodo_fichero=lista_sistema_sacar_fichero(parms->lista_sistema,file_vpath);
			if(nodo_fichero!=NULL){
				if(strcmp(nodo_fichero->propietario,parms->pkg_nombre)){
					char path_respaldo[1024];
					// el fichero es de un paquete diferente
					sprintf(path_respaldo,"%s%s/ficheros/%s\0",
							pkg_basedir,
							nodo_fichero->propietario,
							file_vpath);
					rename(file_dest_path,path_respaldo);
				}
			free(nodo_fichero);
			}
		}
						
		// es un fichero (o enlace)
		rename(path,file_dest_path);
		lista_sistema_apendizar_fichero(parms->lista_sistema,file_vpath,parms->pkg_nombre);
	}
}

// funcion para combinar el paquete con el sistema
void pkg_merge(Root_File_List *lista_sistema,char *nombre){
	char path[1024];
	pkg_merge_parms parms;

	// preparar los parametros
	sprintf(path,"%s%s/ficheros\0",pkg_basedir,nombre);
	strcpy(parms.vpath,"");
	strcpy(parms.path,path);
	strcpy(parms.pkg_nombre,nombre);
	parms.lista_sistema=lista_sistema;

	// combinar el directorio con el sistema
	Dir_ForEach(path,pkg_merge_func,&parms);
}


// funcion para descombinar el paquete del sistema
void pkg_unmerge(Root_File_List *lista_sistema,char *nombre){
	Root_File_Ent *pi,*prev;
	
	prev=NULL;
	pi=lista_sistema->lista;
	
	while(pi!=NULL){
		if(!strcmp(pi->propietario,nombre)){
			char orig_path[1024],dest_path[1024];
			
			// preparar paths
			sprintf(orig_path,"%s%s\0",system_root,pi->nombre);
			sprintf(dest_path,"%s%s/ficheros/%s\0",pkg_basedir,nombre,pi->nombre);
			
			// mover el fichero
			rename(orig_path,dest_path);
			
			// avanzar
			if(prev==NULL){
				lista_sistema->lista=(Root_File_Ent *)pi->next;
			}else{
				prev->next=(void *)pi->next;
			}
			pi=(Root_File_Ent *)pi->next;
		}else{
			// siguente
			prev=pi;
			pi=(Root_File_Ent *)pi->next;
		}
	}
}


// funcion para eliminar el paquete del sistema (solo el dir)
void pkg_eliminar(char *nombre){
	char path[1024];

	sprintf(path,"%s%s\0",pkg_basedir,nombre);
	borrado_recursivo(path);
}

// realizar las operaciones necesarias en la instalacion
void pkg_postinstall(char *nombre){
	char path[1024],comando[1024];

	sprintf(path,"%s%s/install.sh\0",pkg_basedir,nombre);
	if(ExisteFichero(path)){
		sprintf(comando,"sh %s\0",path);
		system(comando);
	}
}

// realizar las operaciones necesarias en la desinstalacion
void pkg_preuninstall(char *nombre){
	char path[1024],comando[1024];

	sprintf(path,"%s%s/uninstall.sh\0",pkg_basedir,nombre);
	if(ExisteFichero(path)){
		sprintf(comando,"sh %s\0",path);
		system(comando);
	}
}

// funcion para comprobar el estado del paquete
int pkg_estado(char *nombre){
	char path[1024];

	sprintf(path,"%s%s\0",pkg_basedir,nombre);
	if(ExisteFichero(path))
		if(EsDirectorio(path))
			return(1);
	return(0);
}



#define SYSTEM_ROOT "/"
#define PKG_BASEDIR "var/db/pkg/"
#define ROOT_LIST_FILE "lista_ficheros"

// Funcion para inicializar el sistema de paquetes
void pkg_iniciar(){
	char *env_val;

	/////////////////////////
	// obtener configuracion
	/////////////////////////


	env_val=getenv("SYSTEM_ROOT");
	if(env_val!=NULL){
		// valor en el entorno
		strcpy(system_root,env_val);
	}else{
		// valor por defecto
		strcpy(system_root,SYSTEM_ROOT);
	}

	env_val=getenv("PKG_BASEDIR");
	if(env_val!=NULL){
		// valor de entorno
		sprintf(pkg_basedir,"%s%s\0",system_root,env_val);
	}else{
		// valor por defecto
		sprintf(pkg_basedir,"%s%s\0",system_root,PKG_BASEDIR);
	}

	
	env_val=getenv("ROOT_LIST_FILE");
	if(env_val!=NULL){
		// valor de entorno
		sprintf(root_list_file,"%s%s\0",pkg_basedir,env_val);
	}else{
		// valor por defecto
		sprintf(root_list_file,"%s%s\0",pkg_basedir,ROOT_LIST_FILE);
	}


	//////////////////////////////////////////////////
	// comprobar directorio del sistema de paquetes
	/////////////////////////////////////////////////

}

void pkg_preparar_dir_systema(){
	char comando[255];
	
	// Crear los directorios
	sprintf(comando,"mkdir -pv %s",pkg_basedir);
	system(comando);

	// Crear el fichero de la lista del sistema
	sprintf(comando,"touch %s",root_list_file);
	system(comando);
	
	
}


// funcion para mostrar el entorno
void pkg_mostrar_entorno(){
	printf("Entorno:\n");
	printf("SYSTEM_ROOT    =\"%s\"\n",system_root);
	printf("PKG_BASEDIR    =\"%s\"\n",pkg_basedir);
	printf("ROOT_LIST_FILE =\"%s\"\n",root_list_file);

}
