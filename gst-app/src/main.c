/*
 * Copyright (C) <2005> GStreamer hacker X
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
