#!/bin/sh
if test -z $AUTOCONF; then export AUTOCONF=autoconf-2.53; fi
if test -z $AUTOMAKE; then 
  export AUTOMAKE=automake-1.5 
  export ACLOCAL=aclocal-1.5
fi
set -x
$ACLOCAL && 
libtoolize --force && \
# autoheader
$AUTOCONF && \
$AUTOMAKE -a && \
./configure

