/*
 * GStreamer gst-app template
 * load.c : load a pipeline from disk
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

