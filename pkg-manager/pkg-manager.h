#ifndef _PKG_MANAGER_H_
#define _PKG_MANAGER_H_





// Estructuras para la lista de ficheros del sistema

typedef struct {
	char nombre[1024];
	char propietario[255];
	void *next;
} Root_File_Ent;

typedef struct {
	Root_File_Ent *lista;
	Root_File_Ent *tail;
} Root_File_List ;

Root_File_List *lista_sistema_cargar();
void lista_sistema_borrar(Root_File_List *lista);
Root_File_Ent *lista_sistema_sacar_fichero(Root_File_List *lista,char *fichero);
void lista_sistema_apendizar_fichero(Root_File_List *lista,char *fichero,char *propietario);
void lista_sistema_guardar(Root_File_List *lista);


// Estructuras para la lista de ficheros del paquete

typedef struct {
	char nombre[1024];
	void *next;
} Pkg_File_Ent;

typedef struct {
	Pkg_File_Ent *lista;
} Pkg_File_List;


Pkg_File_List *pkg_lista_cargar(char *nombre);
void pkg_lista_borrar(Pkg_File_List *lista);
void pkg_lista_print(Pkg_File_List *lista);


// Funcion para preparar la extraccion de un paquete
void pkg_preparar(char *name);
// Funcion para extraer un paquete
void pkg_extraer(char *fichero,char *nombre);
// funcion para combinar el paquete con el sistema
void pkg_merge(Root_File_List *lista_sistema,char *nombre);
// funcion para descombinar el paquete del sistema
void pkg_unmerge(Root_File_List *lista_sistema,char *nombre);
// funcion para eliminar el paquete del sistema (solo el dir)
void pkg_eliminar(char *nombre);
// realizar las operaciones necesarias en la instalacion
void pkg_postinstall(char *nombre);
// realizar las operaciones necesarias en la desinstalacion
void pkg_preuninstall(char *nombre);
// funcion para comprobar el estado del paquete
int pkg_estado(char *nombre);

// Funcion para inicializar el sistema de paquetes
void pkg_iniciar();

// Funcion para preparar el sistema destino
void pkg_preparar_dir_systema();

// funcion para mostrar el entorno
void pkg_mostrar_entorno();

#endif
