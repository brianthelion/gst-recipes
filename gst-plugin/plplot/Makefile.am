# location where plug-in will be installed
plugindir= $(libdir)/gst

plugin_LTLIBRARIES = libgstplplotsink.la

# sources used to compile this plug-in
libgstplplotsink_la_SOURCES = gstplplotsink.c

# flags used to compile this plugin
# we use the GST_LIBS flags because we might be using plug-in libs
libgstplplotsink_la_CFLAGS = $(GST_LIBS_CFLAGS)
libgstplplotsink_la_LDFLAGS = $(GST_LIBS_LIBS) -lplplotd

# headers we need but don't want installed
noinst_HEADERS = gstplplotsink.h
