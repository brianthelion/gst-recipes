/* 
 * gstplugin.h: sample header file for plug-in
 */

#ifndef __GST_PLUGIN_H__
#define __GST_PLUGIN_H__

#include <gst/gst.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#define GST_TYPE_PLUGIN \
  (gst_plugin_get_type())
#define GST_PLUGIN(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_PLUGIN,GstPlugin))
#define GST_PLUGIN_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_ULAW,GstPlugin))
#define GST_IS_PLUGIN(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_PLUGIN))
#define GST_IS_PLUGIN_CLASS(obj) \
  (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_PLUGIN))

typedef struct _GstPlugin GstPlugin;
typedef struct _GstPluginClass GstPluginClass;

struct _GstPlugin {
  GstElement element;

  GstPad *sinkpad, *srcpad;

  gboolean silent;
};

struct _GstPluginClass {
  GstElementClass parent_class;
};

GType gst_plugin_get_type(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __GST_PLUGIN_H__ */
