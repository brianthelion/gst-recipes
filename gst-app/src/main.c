#include "gst-app.h"

int
main (int argc, char *argv[])
{
  gchar *filename;
  GstElement *pipeline;
  
  gst_init (&argc, &argv);

  if (argc <= 1)
    g_error ("Please supply an xml file describing a pipeline !\n");

  filename = g_strdup_printf ("%s", argv[1]);
  pipeline = gst_app_pipeline_load (filename);
  if (pipeline == NULL)
  {
    g_warning ("Could not get a usable pipeline from file '%s'\n", filename);
    return 1;
  }
  
  gst_element_set_state (pipeline, GST_STATE_PLAYING);
  while (gst_bin_iterate (GST_BIN (pipeline)))
    g_print ("+");

  g_print ("\n");

  return 0;
}
