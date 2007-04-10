// permitir offsets de 64bits (tamaños mayores de 4GB)
#define _FILE_OFFSET_BITS 64

#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <utime.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "StringUtil.h"
#include "Ficheros.h"
#include "pkg-manager.h"



void usage(){
	printf("Metodo de uso:\n");
	printf("pkg-manager install <pkg.tar.bz2> \n");
	printf("pkg-manager remove <pkg.tar.bz2> \n");
	printf("pkg-manager prepare \n");
	pkg_mostrar_entorno();
}


int main(int argc,char *argv[]){
	char fichero[1024];
	char nombre[1024];
	Pkg_File_List *pkg_lista;
	Root_File_List *lista;

	// iniciar el sistema de paquetes
	pkg_iniciar();


	if(argc>1){
		// Instalar uno
		if(!strcmp(argv[1],"install") && argc==3){

			// Comprobar que exista el paquete
			strcpy(fichero,argv[2]);
			if(!ExisteFichero(fichero)){
				printf("El paquete \"%s\" no existe.\n",fichero);
				exit(0);
			}
			
			// cargar lista de ficheros del sistema
			lista=lista_sistema_cargar();
			
			// determinar el nombre del paquete
			basename(fichero,nombre);
			
			// preparar para la extraccion del fichero
			pkg_preparar(nombre);
			
			// extraer el fichero
			printf("*** Extrayendo: \"%s\"\n",nombre);
			pkg_extraer(fichero,nombre);
			
			// combinar en el sistema
			printf("*** Combinando \"%s\"\n",nombre);
			pkg_merge(lista,nombre);
			
			// realizar proceso de postinstalacion
			printf("*** Configurando \"%s\"\n",nombre);
			pkg_postinstall(nombre);

			
			// guardar nueva lista
			lista_sistema_guardar(lista);
	
			// liberar lista
			lista_sistema_borrar(lista);

			// salir
			exit(0);
		}
		
		// Instalar varios
		if(!strcmp(argv[1],"install") && argc>3){
			int a;
			// cargar lista de ficheros del sistema
			lista=lista_sistema_cargar();

			// Por cada paquete
			for(a=2;a<argc;a++){
			
				// Comprobar que exista el paquete
				strcpy(fichero,argv[a]);
				if(!ExisteFichero(fichero))
					continue;
			
			
				// determinar el nombre del paquete
				basename(fichero,nombre);
				
				// preparar para la extraccion del fichero
				pkg_preparar(nombre);
				
				// extraer el fichero
				printf("*** Extrayendo: \"%s\"\n",nombre);
				pkg_extraer(fichero,nombre);
				
				// combinar en el sistema
				printf("*** Combinando \"%s\"\n",nombre);
				pkg_merge(lista,nombre);	

			}

		
			// guardar nueva lista
			lista_sistema_guardar(lista);
			
			// liberar lista del sistema
			lista_sistema_borrar(lista);


			// Postinstalacion
			for(a=2;a<argc;a++){
				// determinar el nombre del paquete
				basename(argv[a],nombre);
				if(!pkg_estado(nombre)){
					continue;
				}
				
				// realizar proceso de postinstalacion
				printf("*** Configurando \"%s\"\n",nombre);
				pkg_postinstall(nombre);
				
			}
			
			// salir
			exit(0);
		}
		
		// Desinstalar uno
		if(!strcmp(argv[1],"remove") && argc==3){
			if(pkg_estado(argv[2])){
				// cargar lista de ficheros del sistema
				lista=lista_sistema_cargar();
	
				// determinar el nombre del paquete
				strcpy(fichero,argv[2]);
				basename(fichero,nombre);
	
				// realizar proceso de predesinstalacion
				printf("*** Desconfigurando \"%s\"\n",nombre);
				pkg_preuninstall(nombre);
				// descombinar el paquete del sistema
				printf("*** Descombinando \"%s\"\n",nombre);
				pkg_unmerge(lista,nombre);
				// eliminar ficheros
				printf("*** Eliminando \"%s\"\n",nombre);
				pkg_eliminar(nombre);
		
				// guardar nueva lista
				lista_sistema_guardar(lista);		
	
				// liberar listas
				lista_sistema_borrar(lista);
			
				// salir
				exit(0);
			}else{
				// paquete no instalado
				printf("No esta instalado\n");
			}
		}
		
		
		// "Merge" uno
		if(!strcmp(argv[1],"merge") && argc==3){
			if(pkg_estado(argv[2])){
				// cargar lista de ficheros del sistema
				lista=lista_sistema_cargar();
				
				// determinar el nombre del paquete
				strcpy(fichero,argv[2]);
				basename(fichero,nombre);
				
				
				// combinar en el sistema
				printf("*** Combinando \"%s\"\n",nombre);
				pkg_merge(lista,nombre);

		
				// guardar nueva lista
				lista_sistema_guardar(lista);
			
				// liberar lista del sistema
				lista_sistema_borrar(lista);


				// salir
				exit(0);
			}else{
				// paquete no instalado
				printf("No esta instalado\n");
			}
		
		
		}
		
		
		
		// Preparar directorios para la BD
		if(!strcmp(argv[1],"prepare")){
			// Preparar el sistema destino
			pkg_preparar_dir_systema();	
			
			// salir
			exit(0);
		}
	}
	usage();

	return(0);
}
