/*
 * GStreamer
 * Copyright (C) <1999> Erik Walthinsen <omega@cse.ogi.edu>
 * Copyright (C) <2003> David Schleef <ds@schleef.org>
 * Copyright (C) YEAR AUTHOR_NAME AUTHOR_EMAIL
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * Alternatively, the contents of this file may be used under the
 * GNU Lesser General Public License Version 2.1 (the "LGPL"), in
 * which case the following provisions apply instead of the ones
 * mentioned above:
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

/**
 * SECTION:element-plugin
 *
 * FIXME:Describe plugin here.
 *
 * <refsect2>
 * <title>Example launch line</title>
 * |[
 * gst-launch -v -m audiotestsrc ! plugin ! autoaudiosink
 * ]|
 * </refsect2>
 */
 
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gst/gst.h>
#include <gst/audio/audio.h>
#include <gst/audio/gstaudiofilter.h>
#include <string.h>

GST_DEBUG_CATEGORY_STATIC (gst_plugin_template_debug);
#define GST_CAT_DEFAULT gst_plugin_template_debug

typedef struct _GstPluginTemplate GstPluginTemplate;
typedef struct _GstPluginTemplateClass GstPluginTemplateClass;

#define GST_TYPE_PLUGIN_TEMPLATE \
  (gst_plugin_template_get_type())
#define GST_PLUGIN_TEMPLATE(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_PLUGIN_TEMPLATE,GstPluginTemplate))
#define GST_PLUGIN_TEMPLATE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_PLUGIN_TEMPLATE,GstPluginTemplateClass))
#define GST_IS_PLUGIN_TEMPLATE(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_PLUGIN_TEMPLATE))
#define GST_IS_PLUGIN_TEMPLATE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_PLUGIN_TEMPLATE))

struct _GstPluginTemplate
{
  GstAudioFilter audiofilter;
};

struct _GstPluginTemplateClass
{
  GstAudioFilterClass parent_class;
};


enum
{
  /* FILL ME */
  LAST_SIGNAL
};

enum
{
  ARG_0
  /* FILL ME */
};

/* debug category for fltering log messages
 *
 * FIXME:exchange the string 'Template plugin' with your description
 */
#define DEBUG_INIT(bla) \
  GST_DEBUG_CATEGORY_INIT (gst_plugin_template_debug, "plugin", 0, "Template plugin");

GST_BOILERPLATE_FULL (GstPluginTemplate, gst_plugin_template,
    GstAudioFilter, GST_TYPE_AUDIO_FILTER, DEBUG_INIT);

static void gst_plugin_template_set_property (GObject * object,
    guint prop_id, const GValue * value, GParamSpec * pspec);
static void gst_plugin_template_get_property (GObject * object,
    guint prop_id, GValue * value, GParamSpec * pspec);

static gboolean gst_plugin_template_setup (GstAudioFilter * filter,
    GstRingBufferSpec * spec);
static GstFlowReturn gst_plugin_template_filter (GstBaseTransform * bt,
    GstBuffer * outbuf, GstBuffer * inbuf);
static GstFlowReturn
gst_plugin_template_filter_inplace (GstBaseTransform * base_transform,
    GstBuffer * buf);

#define ALLOWED_CAPS_STRING \
    GST_AUDIO_INT_STANDARD_PAD_TEMPLATE_CAPS

/* GObject vmethod implementations */

static void
gst_plugin_template_base_init (gpointer klass)
{
  GstElementClass *element_class = GST_ELEMENT_CLASS (klass);
  GstAudioFilterClass *audiofilter_class = GST_AUDIO_FILTER_CLASS (klass);
  GstCaps *caps;
  
  gst_element_class_set_details_simple (element_class,
    "Plugin",
    "Filter/Effect/Audio",
    "FIXME:Filters audio",
    "AUTHOR_NAME AUTHOR_EMAIL");

  caps = gst_caps_from_string (ALLOWED_CAPS_STRING);
  gst_audio_filter_class_add_pad_templates (audiofilter_class, caps);
  gst_caps_unref (caps);
}

static void
gst_plugin_template_class_init (GstPluginTemplateClass * klass)
{
  GObjectClass *gobject_class;
  GstBaseTransformClass *btrans_class;
  GstAudioFilterClass *audio_filter_class;

  gobject_class = (GObjectClass *) klass;
  btrans_class = (GstBaseTransformClass *) klass;
  audio_filter_class = (GstAudioFilterClass *) klass;

#if 0
  g_object_class_install_property (gobject_class, ARG_METHOD,
      g_param_spec_enum ("method", "method", "method",
          GST_TYPE_AUDIOTEMPLATE_METHOD, GST_AUDIOTEMPLATE_METHOD_1,
          G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));
#endif

  gobject_class->set_property = gst_plugin_template_set_property;
  gobject_class->get_property = gst_plugin_template_get_property;

  /* this function will be called whenever the format changes */
  audio_filter_class->setup = gst_plugin_template_setup;

  /* here you set up functions to process data (either in place, or from
   * one input buffer to another output buffer); only one is required */
  btrans_class->transform = gst_plugin_template_filter;
  btrans_class->transform_ip = gst_plugin_template_filter_inplace;
}

static void
gst_plugin_template_init (GstPluginTemplate * plugin_template,
    GstPluginTemplateClass * g_class)
{
  GST_DEBUG ("init");

  /* do stuff if you need to */
}

static void
gst_plugin_template_set_property (GObject * object, guint prop_id,
    const GValue * value, GParamSpec * pspec)
{
  GstPluginTemplate *filter = GST_PLUGIN_TEMPLATE (object);

  GST_OBJECT_LOCK (filter);
  switch (prop_id) {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
  GST_OBJECT_UNLOCK (filter);
}

static void
gst_plugin_template_get_property (GObject * object, guint prop_id,
    GValue * value, GParamSpec * pspec)
{
  GstPluginTemplate *filter = GST_PLUGIN_TEMPLATE (object);

  GST_OBJECT_LOCK (filter);
  switch (prop_id) {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
  GST_OBJECT_UNLOCK (filter);
}

static gboolean
gst_plugin_template_setup (GstAudioFilter * filter,
    GstRingBufferSpec * spec)
{
  GstPluginTemplate *plugin_template;

  plugin_template = GST_PLUGIN_TEMPLATE (filter);

  /* if any setup needs to be done, do it here */

  return TRUE;                  /* it's all good */
}

/* You may choose to implement either a copying filter or an
 * in-place filter (or both).  Implementing only one will give
 * full functionality, however, implementing both will cause
 * audiofilter to use the optimal function in every situation,
 * with a minimum of memory copies. */

static GstFlowReturn
gst_plugin_template_filter (GstBaseTransform * base_transform,
    GstBuffer * inbuf, GstBuffer * outbuf)
{
  GstPluginTemplate *plugin_template;
  GstAudioFilter *audiofilter;

  audiofilter = GST_AUDIO_FILTER (base_transform);
  plugin_template = GST_PLUGIN_TEMPLATE (base_transform);

  /* FIXME: do something interesting here.  This simply copies the source
   * to the destination. */

  memcpy (GST_BUFFER_DATA (outbuf), GST_BUFFER_DATA (inbuf),
      GST_BUFFER_SIZE (inbuf));

  return GST_FLOW_OK;
}

static GstFlowReturn
gst_plugin_template_filter_inplace (GstBaseTransform * base_transform,
    GstBuffer * buf)
{
  GstPluginTemplate *plugin_template;
  GstAudioFilter *audiofilter;

  audiofilter = GST_AUDIO_FILTER (base_transform);
  plugin_template = GST_PLUGIN_TEMPLATE (base_transform);

  /* FIXME: do something interesting here.  This simply copies the source
   * to the destination. */

  return GST_FLOW_OK;
}

static gboolean
plugin_init (GstPlugin * plugin)
{
  return gst_element_register (plugin, "plugin", GST_RANK_NONE,
      GST_TYPE_PLUGIN_TEMPLATE);
}

/* gstreamer looks for this structure to register plugins
 *
 * FIXME:exchange the string 'Template plugin' with you plugin description
 */
GST_PLUGIN_DEFINE (
    GST_VERSION_MAJOR,
    GST_VERSION_MINOR,
    "plugin",
    "Template plugin",
    plugin_init,
    VERSION, "LGPL",
    "GStreamer",
    "http://gstreamer.net/"
);
