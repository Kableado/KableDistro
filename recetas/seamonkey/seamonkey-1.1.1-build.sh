#!/bin/sh

## Untar
tar xvf seamonkey-1.1.1.source.tar.bz2
mkdir mozilla

## Config
cat > .mozconfig << "EOF"
mk_add_options MOZ_CO_PROJECT=suite
mk_add_options MOZ_OBJDIR=@TOPSRCDIR@/suite-opt
ac_add_options --enable-application=suite
ac_add_options --enable-debug
ac_add_options --disable-tests
ac_add_options --prefix=/usr
ac_add_options --with-system-zlib
ac_add_options --with-system-png
ac_add_options --with-system-jpeg
ac_add_options --disable-installer
ac_add_options --enable-svg
ac_add_options --disable-mailnews
ac_add_options --disable-ldap

ac_add_options --enable-default-toolkit=gtk2
ac_add_options --enable-xft

EOF

## BUILD
make -f client.mk build || exit -1

## INSTALL
mkdir dest
make DESTDIR=$(pwd)/dest -f client.mk install || exit -1
install -v -m755 -d dest/usr/include/seamonkey-1.1.1/nss

## PKG
cd dest
tar cjvfp ../../
cd cd ..

## CLEANUP
cd ..
rm -rf mozilla

