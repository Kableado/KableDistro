#!/bin/sh

source /etc/profile
cd /mnt/

echo "#############"
echo "##  TIER 2 ##"
echo "#############"

#instalar SHADOW
pkg-manager install lfs-pkgs/shadow-4.0.15.tar.bz2

##### Instalar todo BLFS
pkg-manager install blfs-pkgs/*

##### Instalar todo extra
pkg-manager install extra-pkgs/*


# establecer password
echo "######"
echo "PASSWORD de ROOT"
passwd



##########################################
## TODO:

# Establecer particiones
# Compilar Nucleo
# Establecer GRUB
