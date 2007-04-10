#!/bin/sh

DEST=/mnt/dest/

#######################
## TIER ONE ###########
#######################

echo "#############"
echo "##  TIER 1 ##"
echo "#############"


## PREPARAR
export SYSTEM_ROOT=$DEST
./pkg-manager/pkg-manager prepare

# PREPARAR FHS
tar xvjfp FHS-lfs-20060531.tar.bz2 -C $DEST


# INSTALAR BASE (LFS)
cd lfs-pkgs
../pkg-manager/pkg-manager install \
linux-libc-headers-2.6.12.0.tar.bz2 \
man-pages-2.33.tar.bz2 \
glibc-2.3.6.tar.bz2 \
binutils-2.17.tar.bz2 \
gcc-4.0.3.tar.bz2 \
db-4.4.20.tar.bz2 \
coreutils-5.96.tar.bz2 \
iana-etc-2.10.tar.bz2 \
m4-1.4.4.tar.bz2 \
bison-2.2.tar.bz2 \
ncurses-5.5.tar.bz2 \
procps-3.2.6.tar.bz2 \
sed-4.1.5.tar.bz2 \
libtool-1.5.22.tar.bz2 \
perl-5.8.8.tar.bz2 \
readline-5.1.tar.bz2 \
zlib-1.2.3.tar.bz2 \
autoconf-2.59.tar.bz2 \
automake-1.9.6.tar.bz2 \
bash-3.1.tar.bz2 \
bzip2-1.0.3.tar.bz2 \
diffutils-2.8.1.tar.bz2 \
e2fsprogs-1.38.tar.bz2 \
file-4.17.tar.bz2 \
findutils-4.2.27.tar.bz2 \
flex-2.5.33.tar.bz2 \
grub-0.97.tar.bz2 \
gawk-3.1.5.tar.bz2 \
gettext-0.14.5.tar.bz2 \
grep-2.5.1a.tar.bz2 \
groff-1.18.1.1.tar.bz2 \
gzip-1.3.5.tar.bz2 \
inetutils-1.4.2.tar.bz2 \
iproute2-2.6.16-060323.tar.bz2 \
kbd-1.12.tar.bz2 \
less-394.tar.bz2 \
make-3.80.tar.bz2 \
man-db-2.4.3.tar.bz2 \
mktemp-1.5.tar.bz2 \
module-init-tools-3.2.2.tar.bz2 \
patch-2.5.4.tar.bz2 \
psmisc-22.2.tar.bz2 \
sysklogd-1.4.1.tar.bz2 \
sysvinit-2.86.tar.bz2 \
tar-1.15.1.tar.bz2 \
texinfo-4.8.tar.bz2 \
udev-092.tar.bz2 \
util-linux-2.12r.tar.bz2 \
vim-7.0.tar.bz2 
cd ..

# Configuracion BASE
./pkg-manager/pkg-manager install \
baseconfig-20061012.tar.bz2 \
bsd-bootscripts-20061012.tar.bz2

unset SYSTEM_ROOT

######################
## TIER TWO ##########
######################
## Para realizar acciones dentro del sistema
cp pkg-manager/pkg-manager ${DEST}sbin/pkg-manager
mount . ${DEST}mnt -o bind
chroot $DEST /mnt/install-tier2.sh
umount ${DEST}mnt




##########################################
## TODO:

# Establecer particiones
# Compilar Nucleo
# Establecer GRUB
