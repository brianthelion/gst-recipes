/* 
 * 
 * GStreamer
 * Copyright (C) 1999-2001 Erik Walthinsen <omega@cse.ogi.edu>
 * Copyright (C) 2002      David A. Schleef <ds@schleef.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include <gst/gst.h>
#include <plplot/plplot.h>
#include <plplot/plplotP.h>

/* include this header if you want to use dynamic parameters
#include <gst/control/control.h>
*/

#include "gstplplotsink.h"

static GstElementDetails plugin_details = {
  "PlplotSink",
  "sink/plplot sink",
  "Plplot-based plotter",
  VERSION,
  "David A. Schleef <ds@schleef.org>",
  "(C) 2002"
};

/* Filter signals and args */
enum {
  /* FILL ME */
  LAST_SIGNAL
};

enum {
  ARG_0,
  ARG_SILENT
};

GST_PAD_TEMPLATE_FACTORY (gst_plugin_template_sink_factory,
  "sink",
  GST_PAD_SINK,
  GST_PAD_ALWAYS,
  NULL		/* no caps */
);

static void	gst_plugin_template_class_init	(GstPluginTemplateClass *klass);
static void	gst_plugin_template_init		(GstPluginTemplate *filter);

static void	gst_plugin_template_set_property	(GObject *object, guint prop_id,
                                                 const GValue *value, 
					         GParamSpec *pspec);
static void	gst_plugin_template_get_property    (GObject *object, guint prop_id,
                                                 GValue *value, 
						 GParamSpec *pspec);

static void	gst_plugin_template_update_plugin   (const GValue *value, gpointer data);
static void	gst_plugin_template_update_mute     (const GValue *value, gpointer data);

static void	gst_plugin_template_chain		(GstPad *pad, GstBuffer *buf);

static GstElementClass *parent_class = NULL;

/* this function handles the connection with other plug-ins */
static GstPadConnectReturn
gst_plugin_template_connect (GstPad *pad, GstCaps *caps)
{
  GstPluginTemplate *filter;
  GstPad *otherpad;
  
  filter = GST_PLUGIN_TEMPLATE (gst_pad_get_parent (pad));
  g_return_val_if_fail (filter != NULL, GST_PAD_CONNECT_REFUSED);
  g_return_val_if_fail (GST_IS_PLUGIN_TEMPLATE (filter), 
                        GST_PAD_CONNECT_REFUSED);
  
  if (GST_CAPS_IS_FIXED (caps)) 
  {
    /* caps on other side were accepted, so we're ok */
    return GST_PAD_CONNECT_OK;
  }
  /* not enough information yet, delay negotation */ 
  return GST_PAD_CONNECT_DELAYED;
}

GType
gst_gst_plugin_template_get_type (void) 
{
  static GType plugin_type = 0;

  if (!plugin_type) 
  {
    static const GTypeInfo plugin_info = 
    {
      sizeof (GstPluginTemplateClass),
      NULL,
      NULL,
      (GClassInitFunc) gst_plugin_template_class_init,
      NULL,
      NULL,
      sizeof (GstPluginTemplate),
      0,
      (GInstanceInitFunc) gst_plugin_template_init,
    };
    plugin_type = g_type_register_static (GST_TYPE_ELEMENT, 
	                                  "GstPlplotSink", 
	                                  &plugin_info, 0);
  }
  return plugin_type;
}

/* initialize the plugin's class */
static void
gst_plugin_template_class_init (GstPluginTemplateClass *klass)
{
  GObjectClass *gobject_class;
  GstElementClass *gstelement_class;

  gobject_class = (GObjectClass*) klass;
  gstelement_class = (GstElementClass*) klass;

  parent_class = g_type_class_ref (GST_TYPE_ELEMENT);

  g_object_class_install_property (gobject_class, ARG_SILENT,
    g_param_spec_boolean ("silent", "Silent", "Produce verbose output ?",
                          FALSE, G_PARAM_READWRITE));
  
  gobject_class->set_property = gst_plugin_template_set_property;
  gobject_class->get_property = gst_plugin_template_get_property;
}

/* initialize the new element
 * instantiate pads and add them to element
 * set functions
 * initialize structure
 */
static void
gst_plugin_template_init (GstPluginTemplate *filter)
{
  filter->sinkpad = gst_pad_new_from_template (gst_plugin_template_sink_factory (), 
                                               "sink");
  gst_pad_set_connect_function (filter->sinkpad, gst_plugin_template_connect);
  
  gst_element_add_pad (GST_ELEMENT (filter), filter->sinkpad);
  gst_pad_set_chain_function (filter->sinkpad, gst_plugin_template_chain);
  filter->silent = FALSE;

}

/* chain function
 * this function does the actual processing
 */

static void
gst_plugin_template_chain (GstPad *pad, GstBuffer *buf)
{
  GstPluginTemplate *filter;
  GstBuffer *out_buf;
  guint16 *data;
  double *data_x, *data_y;
  gint n;
  int i;

  g_return_if_fail (GST_IS_PAD (pad));
  g_return_if_fail (buf != NULL);
  
  filter = GST_PLUGIN_TEMPLATE (GST_OBJECT_PARENT (pad));
  g_return_if_fail (GST_IS_PLUGIN_TEMPLATE (filter));

  {
	  static int done = 0;

	  if(!done){
		  done = 1;
		  plsstrm(0);
		  plspage(0.0,0.0,640,480,0,0);
		  plsdev("xwin");
		  plinit();
		  plspause(0);
		  plssub(1,1);
	  }
  }

  data = (guint16 *) GST_BUFFER_DATA(buf);
  n = GST_BUFFER_SIZE(buf)/sizeof(float);

  plsstrm(0);
  plbop();

  plcol(15);
  pladv(1);
  plvsta();
  plwind(0, n, -10.0, 10.0);
  plbox("bcnst", 0.0, 0, "bcnstv", 0.0, 0);
  pllab("time (s)", "voltage (V)", "ack");

  data_x = malloc(sizeof(double)*n);
  for(i=0;i<n;i++){
	  data_x[i] = i;
  }

  data_y = malloc(sizeof(double)*n);
  for(i=0;i<n;i++){
	  data_y[i] = -10.0 + (20.0/65536)*data[i];
  }

  plcol(1);
  plline(n, data_x, data_y);
  
  pleop();

  free(data_x);
  free(data_y);

  gst_buffer_unref(buf);
}

static void
gst_plugin_template_set_property (GObject *object, guint prop_id, 
                              const GValue *value, GParamSpec *pspec)
{
  GstPluginTemplate *filter;

  g_return_if_fail (GST_IS_PLUGIN_TEMPLATE (object));
  filter = GST_PLUGIN_TEMPLATE (object);

  switch (prop_id) 
  {
  case ARG_SILENT:
    filter->silent = g_value_get_boolean (value);
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    break;
  }
}

static void
gst_plugin_template_get_property (GObject *object, guint prop_id, 
                              GValue *value, GParamSpec *pspec)
{
  GstPluginTemplate *filter;

  g_return_if_fail (GST_IS_PLUGIN_TEMPLATE (object));
  filter = GST_PLUGIN_TEMPLATE (object);
  
  switch (prop_id) {
  case ARG_SILENT:
    g_value_set_boolean (value, filter->silent);
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    break;
  }
}

/* entry point to initialize the plug-in
 * initialize the plug-in itself
 * register the element factories and pad templates
 * register the features 
 */
static gboolean
plugin_init (GModule *module, GstPlugin *plugin)
{
  GstElementFactory *factory;

  factory = gst_element_factory_new ("plplotsink", GST_TYPE_PLUGIN_TEMPLATE,
                                     &plugin_details);
  g_return_val_if_fail (factory != NULL, FALSE);
  
  gst_element_factory_add_pad_template (factory, 
                                        gst_plugin_template_sink_factory ());

  gst_plugin_add_feature (plugin, GST_PLUGIN_FEATURE (factory));

  /* plugin initialisation succeeded */
  return TRUE;
}

/* this is the structure that gst-register looks for
 * so keep the name plugin_desc, or you cannot get your plug-in registered */
GstPluginDesc plugin_desc = {
  GST_VERSION_MAJOR,
  GST_VERSION_MINOR,
  "plplotsink",
  plugin_init
};
