AC_INIT

dnl Fill in your package name and version here
PACKAGE=gst-app
VERSION=0.1.1

dnl these AC_DEFINE_UNQUOTED's are necessary for make dist to work
AC_DEFINE_UNQUOTED(PACKAGE, "$PACKAGE")
AC_DEFINE_UNQUOTED(VERSION, "$VERSION")
AC_SUBST(PACKAGE)
AC_SUBST(VERSION)

AM_INIT_AUTOMAKE($PACKAGE, $VERSION)

AM_CONFIG_HEADER(config.h)

AC_PROG_CC
AC_PROG_LIBTOOL

dnl Check for pkgconfig first
AC_CHECK_PROG(HAVE_PKGCONFIG, pkg-config, yes, no)

dnl Give error and exit if we don't have pkgconfig
if test "x$HAVE_PKGCONFIG" = "xno"; then
  AC_MSG_ERROR(you need to have pkgconfig installed !)
fi

dnl Now we're ready to ask for gstreamer libs and cflags
dnl And we can also ask for the right version of gstreamer

GST_MAJORMINOR=0.10

GSTCORE_REQUIRED=0.10.0
GSTBASE_REQUIRED=0.10.0

dnl =======================================================================
dnl We are checking for a lot of things from the GStreamer core that
dnl an app might possibly be interested in here. Your app might not need
dnl all of these, but it can't hurt to check for them either (and they
dnl should all be installed on development systems anyway)
dnl =======================================================================

PKG_CHECK_MODULES(GST, [
    gstreamer-$GST_MAJORMINOR              >= $GSTCORE_REQUIRED
    gstreamer-base-$GST_MAJORMINOR         >= $GSTCORE_REQUIRED
    gstreamer-controller-$GST_MAJORMINOR   >= $GSTCORE_REQUIRED
  ], [
    HAVE_GST=yes
  ], [
    HAVE_GST=no
  ])
                                                                                
dnl =======================================================================
dnl Give error and exit if couldn't find gstreamer development files
dnl =======================================================================
if test "x$HAVE_GST" = "xno";
then
  AC_MSG_ERROR([
    Can't find the following GStreamer development packages:

      gstreamer-$GST_MAJORMINOR              >= $GSTCORE_REQUIRED
      gstreamer-base-$GST_MAJORMINOR         >= $GSTCORE_REQUIRED
      gstreamer-controller-$GST_MAJORMINOR   >= $GSTCORE_REQUIRED

    Please make sure you have the necessary GStreamer-$GST_MAJORMINOR
    development headers installed.

    On debian/Ubuntu systems you will probably need to install the
    the 'libgstreamer$GST_MAJORMINOR-dev' package.

    On RPM-based systems you will probably need to install the
    'gstreamer-devel-$GST_MAJORMINOR' package.
  ])
fi

dnl =======================================================================
dnl Make GST_CFLAGS and GST_LIBS available for use in Makefile.am files
dnl =======================================================================

AC_SUBST(GST_CFLAGS)
AC_SUBST(GST_LIBS)





dnl =======================================================================
dnl OPTIONAL:
dnl Also check for extra stuff that is in gst-plugins-base, like additional
dnl tags for example. You might not need this.
dnl =======================================================================

PKG_CHECK_MODULES(GST_PLUGINS_BASE, [
    gstreamer-plugins-base-$GST_MAJORMINOR >= $GSTBASE_REQUIRED
  ], [
    HAVE_GST_PLUGINS_BASE=yes
  ], [
    HAVE_GST_PLUGINS_BASE=no
  ])
                                                                                
dnl =======================================================================
dnl Give a warning only if we don't find the gstreamer plugins base stuff
dnl =======================================================================

if test "x$HAVE_GST_PLUGINS_BASE" = "xno"
then
  AC_MSG_NOTICE([
    Can't find the following GStreamer development packages:

      gstreamer-plugins-base-$GST_MAJORMINOR >= $GSTBASE_REQUIRED

    Please make sure you have the necessary GStreamer-$GST_MAJORMINOR
    development headers installed.

    On debian/Ubuntu systems you will probably need to install the
    the 'libgstreamer-plugins-base$GST_MAJORMINOR-dev' package.

    On RPM-based systems you will probably need to install the
    'gstreamer-plugins-base-devel-$GST_MAJORMINOR' package.
  ])
  sleep 3
fi

dnl =======================================================================
dnl Make GST_CFLAGS and GST_LIBS available for use in Makefile.am files
dnl =======================================================================

AC_SUBST(GST_PLUGINS_BASE_CFLAGS)
AC_SUBST(GST_PLUGINS_BASE_LIBS)


dnl =======================================================================
dnl Finally, create Makefiles in all directories
dnl =======================================================================

AC_OUTPUT([
Makefile
src/Makefile
])

