# location where plug-in will be installed
plugindir= $(libdir)/gst

# change libgstplugin.la to something more suitable
plugin_LTLIBRARIES = libgstplugin.la

# sources used to compile this plug-in
libgstvolume_la_SOURCES = gstplugin.c

# flags used to compile this plugin
# we use the GST_LIBS flags because we might be using plug-in libs
libgstvolume_la_CFLAGS = $(GST_LIBS_CFLAGS)
libgstvolume_la_LDFLAGS = $(GST_LIBS_LDFLAGS)

# headers we need but don't want installed
noinst_HEADERS = gstplugin.h
