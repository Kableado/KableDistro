# Makefile Generico Ajustable

##################
#  Definiciones  #
##################


CFLAGS    = -g 
LIBS      = 
CC        = gcc
CXX	  = g++
DEBUGER   = gdb
RM        = rm -f
FUENTES   = Ficheros.c StringUtil.c pkg-manager.c main.c
CABECERAS = Ficheros.h StringUtil.h pkg-manager.h
OBJS      = Ficheros.o StringUtil.o pkg-manager.o main.o
RESULTADO = pkg-manager

## definiciones para la instalacion
PREFIX=/usr/local
BINDIR     = $(PREFIX)/bin
BINFILES   = $(RESULTADO)
SHAREDIR   = $(PREFIX)/share/$(RESULTADO)
SHAREFILES =  
PIXDIR     = $(SHAREDIR)/pixmaps
PIXFILES   = 




###############
#  Objetivos  #
###############
all: $(RESULTADO)
	# ###########################
	# # Compilacion finalizada. #
	# ###########################

$(RESULTADO): $(OBJS)
	$(CC) -static $(OBJS) -o $(RESULTADO) $(LIBS)

clean: 
	$(RM) $(OBJS) $(RESULTADO)
	# #####################################
	# # limpieza terminada correctamente. #
	# #####################################

.cpp.o: $(CABECERAS)
	$(CXX) -c $(CFLAGS) $<

.c.o: $(CABECERAS)
	$(CC) -c $(CFLAGS) $<

debug: $(RESULTADO)
	$(DEBUGER) $(RESULTADO) 

run: $(RESULTADO)
	./$(RESULTADO)
	# #######################
	# # Ejecucion correcta. #
	# #######################

install: instala

instala: $(RESULTADO)
	mkdir -p $(DESTDIR)$(BINDIR) 
	cp $(BINFILES) $(DESTDIR)$(BINDIR) 
	if test "$(SHAREFILES)" != ""; then \
		mkdir -p $(DESTDIR)$(SHAREDIR); \
		cp $(SHAREFILES) $(DESTDIR)$(SHAREDIR); \
	fi
	if  test "$(PIXFILES)" != ""; then  \
		mkdir -p $(DESTDIR)$(PIXDIR) ; \
		cp $(PIXFILES) $(DESTDIR)$(PIXDIR) ; \
	fi 
	# #########################
	# # Instalacion Concluida #
	# #########################


help: 
	#
	# "make" para compilar, "make clean" para limpiar
	# y "make run" para ejecutar el resultado.
	# Aparte de "make debug" para ejecutar con el debuger.
	#



