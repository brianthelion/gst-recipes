/*
 * GStreamer gst-app template
 * load.c : load a pipeline from disk
 *
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

#include <gst/gst.h>

/*
 * load an xml pipeline description and parse it
 * Returns: the top-level element, or NULL if unable to parse
 */

GstElement *
gst_app_pipeline_load (const gchar *filename)
{
  GstXML *xml;
  GList *l;

  xml = gst_xml_new ();
  if (gst_xml_parse_file (xml, filename, NULL) != TRUE)
  {
    g_warning ("Error loading pipeline from file '%s'\n", filename);
    return NULL;
  }

  l = gst_xml_get_topelements (xml);
  if (l == NULL)
  {
    g_warning ("No toplevel element in file '%s'\n", filename);
    return NULL;
  }
  if (l->next != NULL)
  {
    g_warning ("More than one toplevel element in file '%s'\n", filename);
    return NULL;
  }

  return GST_ELEMENT (l->data);
}

