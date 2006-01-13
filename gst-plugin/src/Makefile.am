# plugindir is set in configure

# change libgstplugin.la to something more suitable
plugin_LTLIBRARIES = libgstplugin.la

# for the next set of variables, rename the prefix if you renamed the .la

# sources used to compile this plug-in
libgstplugin_la_SOURCES = gstplugin.c

# flags used to compile this plugin
# add other _CFLAGS and _LIBS as needed
libgstplugin_la_CFLAGS = $(GST_CFLAGS)
libgstplugin_la_LIBADD = $(GST_LIBS)
libgstplugin_la_LDFLAGS = $(GST_PLUGIN_LDFLAGS)

# headers we need but don't want installed
noinst_HEADERS = gstplugin.h
