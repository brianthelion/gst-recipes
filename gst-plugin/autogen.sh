#!/bin/sh
# you can either set the environment variables AUTOCONF and AUTOMAKE
# to the right versions, or leave them unset and get the RedHat 7.3 defaults
if test -z $AUTOMAKE; then 
  export AUTOMAKE=automake 
  export ACLOCAL=aclocal
fi

# if you would want to be running autoheader as well, you will have to do
# something similar as above for it
if test -z $AUTOCONF; then export AUTOCONF=autoconf; fi
echo "#!/bin/sh" > autoregen.sh
echo "./autogen.sh $@ \$@" >> autoregen.sh
chmod +x autoregen.sh

set -x

# if any of these steps fails, the others will not execute, which is good
# we want to treat errors as soon as possible
$ACLOCAL -I m4 || exit 1
libtoolize --force || exit 1
autoheader || exit 1
$AUTOMAKE -a || exit 1
$AUTOCONF || exit 1
./configure --enable-maintainer-mode $*
