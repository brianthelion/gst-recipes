/*
 * GStreamer
 * Copyright (C) 2005 Thomas Vander Stichele <thomas@apestaart.org>
 * Copyright (C) 2005 Ronald S. Bultje <rbultje@ronald.bitfreak.net>
 * Copyright (C) 2012 Brian Rossa <<user@hostname.org>>
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
 * SECTION:element-collectpadsnoop
 *
 * FIXME:Describe collectpadsnoop here.
 *
 * <refsect2>
 * <title>Example launch line</title>
 * |[
 * gst-launch -v -m fakesrc ! collectpadsnoop ! fakesink silent=TRUE
 * ]|
 * </refsect2>
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gst/gst.h>

#include "gstcollectpadsnoop.h"

GST_DEBUG_CATEGORY_STATIC (gst_collect_pads_noop_debug);
#define GST_CAT_DEFAULT gst_collect_pads_noop_debug

/* Filter signals and args */
enum {
	/* FILL ME */
	LAST_SIGNAL
};

enum {
	PROP_0, PROP_SILENT
};

/* the capabilities of the inputs and outputs.
 *
 * describe the real formats here.
 */

//static GstStaticPadTemplate SINK_FACTORY_GLOBAL = GST_STATIC_PAD_TEMPLATE("sink", GST_PAD_SINK, GST_PAD_ALWAYS, GST_STATIC_CAPS("ANY"));
static GstStaticPadTemplate XSNK_FACTORY_GLOBAL = GST_STATIC_PAD_TEMPLATE("xsnk", GST_PAD_SINK, GST_PAD_ALWAYS, GST_STATIC_CAPS_ANY);
static GstStaticPadTemplate YSNK_FACTORY_GLOBAL = GST_STATIC_PAD_TEMPLATE("ysnk", GST_PAD_SINK, GST_PAD_ALWAYS, GST_STATIC_CAPS_ANY);

//static GstStaticPadTemplate SRC_FACTORY_GLOBAL = GST_STATIC_PAD_TEMPLATE("src", GST_PAD_SRC, GST_PAD_ALWAYS, GST_STATIC_CAPS("ANY"));
static GstStaticPadTemplate XSRC_FACTORY_GLOBAL = GST_STATIC_PAD_TEMPLATE("xsrc", GST_PAD_SRC, GST_PAD_ALWAYS, GST_STATIC_CAPS_ANY);
static GstStaticPadTemplate YSRC_FACTORY_GLOBAL = GST_STATIC_PAD_TEMPLATE("ysrc", GST_PAD_SRC, GST_PAD_ALWAYS, GST_STATIC_CAPS_ANY);

#define gst_collect_pads_noop_parent_class parent_class
G_DEFINE_TYPE(GstCollectPadsNoop, gst_collect_pads_noop, GST_TYPE_ELEMENT);

static void gst_collect_pads_noop_set_property(GObject * object, guint prop_id, const GValue * value, GParamSpec * pspec);
static void gst_collect_pads_noop_get_property(GObject * object, guint prop_id, GValue * value, GParamSpec * pspec);

//static gboolean gst_collect_pads_noop_sink_event(GstPad * pad, GstObject * parent, GstEvent * event);
static gboolean gst_collect_pads_noop_handle_event(GstCollectPads * pads, GstCollectData * data, GstEvent * event, gpointer user_data);
//static GstFlowReturn gst_collect_pads_noop_chain(GstPad * pad, GstObject * parent, GstBuffer * buf);
static GstFlowReturn gst_collect_pads_noop_collected(GstCollectPads * pads, GstCollectPadsNoop * filter);
static GstStateChangeReturn gst_text_overlay_change_state(GstElement * element, GstStateChange transition);

/* GObject vmethod implementations */

/* initialize the collectpadsnoop's class */
static void gst_collect_pads_noop_class_init(GstCollectPadsNoopClass * klass) {
	GObjectClass *gobject_class;
	GstElementClass *gstelement_class;

	gobject_class = (GObjectClass *) klass;
	gstelement_class = (GstElementClass *) klass;

	gstelement_class->change_state = GST_DEBUG_FUNCPTR(gst_text_overlay_change_state);

	gobject_class->set_property = gst_collect_pads_noop_set_property;
	gobject_class->get_property = gst_collect_pads_noop_get_property;

	g_object_class_install_property(gobject_class, PROP_SILENT, g_param_spec_boolean("silent", "Silent", "Produce verbose output ?", FALSE, G_PARAM_READWRITE));

	gst_element_class_set_details_simple(gstelement_class, "CollectPadsNoop", "FIXME:Generic", "FIXME:Generic Template Element", "Brian Rossa <<user@hostname.org>>");

	gst_element_class_add_pad_template(gstelement_class, gst_static_pad_template_get(&XSRC_FACTORY_GLOBAL));
	gst_element_class_add_pad_template(gstelement_class, gst_static_pad_template_get(&XSNK_FACTORY_GLOBAL));
}

/* initialize the new element
 * instantiate pads and add them to element
 * set pad calback functions
 * initialize instance structure
 */
static void gst_collect_pads_noop_init(GstCollectPadsNoop * filter) {

	filter->xsnkpad = gst_pad_new_from_static_template(&XSNK_FACTORY_GLOBAL, "xsnk");
//	gst_pad_set_event_function(filter->xsnkpad, GST_DEBUG_FUNCPTR(gst_collect_pads_noop_sink_event));
//	gst_pad_set_chain_function(filter->xsnkpad, GST_DEBUG_FUNCPTR(gst_collect_pads_noop_chain));
	GST_PAD_SET_PROXY_CAPS(filter->xsnkpad);
	gst_element_add_pad(GST_ELEMENT(filter), filter->xsnkpad);

	filter->ysnkpad = gst_pad_new_from_static_template(&YSNK_FACTORY_GLOBAL, "ysnk");
//	gst_pad_set_event_function(filter->ysnkpad, GST_DEBUG_FUNCPTR(gst_collect_pads_noop_sink_event));
//	gst_pad_set_chain_function(filter->ysnkpad, GST_DEBUG_FUNCPTR(gst_collect_pads_noop_chain));
	GST_PAD_SET_PROXY_CAPS(filter->ysnkpad);
	gst_element_add_pad(GST_ELEMENT(filter), filter->ysnkpad);

	filter->xsrcpad = gst_pad_new_from_static_template(&XSRC_FACTORY_GLOBAL, "xsrc");
	GST_PAD_SET_PROXY_CAPS(filter->xsrcpad);
	gst_element_add_pad(GST_ELEMENT(filter), filter->xsrcpad);

	filter->ysrcpad = gst_pad_new_from_static_template(&YSRC_FACTORY_GLOBAL, "ysrc");
	GST_PAD_SET_PROXY_CAPS(filter->ysrcpad);
	gst_element_add_pad(GST_ELEMENT(filter), filter->ysrcpad);

	filter->silent = FALSE;

	filter->collect = gst_collect_pads_new();
	gst_collect_pads_set_function(filter->collect, (GstCollectPadsFunction)(GST_DEBUG_FUNCPTR(gst_collect_pads_noop_collected)), filter);
	gst_collect_pads_set_event_function(filter->collect, (GstCollectPadsEventFunction)(GST_DEBUG_FUNCPTR(gst_collect_pads_noop_handle_event)), filter);

	filter->y_collect_data = gst_collect_pads_add_pad(filter->collect, filter->xsnkpad, sizeof(GstCollectData), NULL, TRUE);
	filter->x_collect_data = gst_collect_pads_add_pad(filter->collect, filter->ysnkpad, sizeof(GstCollectData), NULL, TRUE);

}

static void gst_collect_pads_noop_set_property(GObject * object, guint prop_id, const GValue * value, GParamSpec * pspec) {
	GstCollectPadsNoop *filter = GST_COLLECTPADSNOOP (object);

	switch (prop_id) {
	case PROP_SILENT:
		filter->silent = g_value_get_boolean(value);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
		break;
	}
}

static void gst_collect_pads_noop_get_property(GObject * object, guint prop_id, GValue * value, GParamSpec * pspec) {
	GstCollectPadsNoop *filter = GST_COLLECTPADSNOOP (object);

	switch (prop_id) {
	case PROP_SILENT:
		g_value_set_boolean(value, filter->silent);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
		break;
	}
}

/* GstElement vmethod implementations */

/* this function handles sink events */

//static gboolean gst_collect_pads_noop_sink_event(GstPad * pad, GstObject * parent, GstEvent * event) {
static gboolean gst_collect_pads_noop_handle_event(GstCollectPads * pads, GstCollectData * data, GstEvent * event, gpointer user_data) {
	gboolean ret;
//	GstCollectPadsNoop *filter = GST_COLLECTPADSNOOP (user_data);
//	GstPad *pad = GST_COLLECTPADSNOOP(data->pad);

	switch (GST_EVENT_TYPE(event)) {
	case GST_EVENT_CAPS: {
		GstCaps * caps;

		gst_event_parse_caps(event, &caps);
		/* do something with the caps */

		/* and forward */
		ret = gst_collect_pads_event_default(pads, data, event, FALSE);
		break;
	}
	default:
		ret = gst_collect_pads_event_default(pads, data, event, FALSE);
		break;
	}
	return ret;
}

/* chain function
 * this function does the actual processing
 */
//static GstFlowReturn gst_collect_pads_noop_chain(GstPad * pad, GstObject * parent, GstBuffer * buf) {
//	GstCollectPadsNoop *filter;
//
//	filter = GST_COLLECTPADSNOOP (parent);
//
//	if (filter->silent == FALSE)
//		g_print("I'm plugged, therefore I'm in.\n");
//
//	/* just push out the incoming buffer without touching it */
//	return gst_pad_push(filter->srcpad, buf);
//}
static GstFlowReturn gst_collect_pads_noop_collected(GstCollectPads * pads, GstCollectPadsNoop * filter) {

//	GstFlowReturn ret = GST_FLOW_OK;
//	GstClockTime now, video_end, audio_end;
	GstBuffer *video_buffer = NULL;
	GstBuffer *audio_buffer = NULL;
//	gchar *text;
//	gint text_len;

	GST_DEBUG_OBJECT(filter,"Starting to collect");

	video_buffer = gst_collect_pads_peek(filter->collect, filter->y_collect_data);
	audio_buffer = gst_collect_pads_peek(filter->collect, filter->x_collect_data);

	if ((video_buffer == NULL) & (audio_buffer == NULL)) {
		GST_DEBUG_OBJECT(filter,"Both streams at EOS");
		return GST_FLOW_EOS;
	} else if (video_buffer == NULL) {
		GST_DEBUG_OBJECT(filter,"Video at EOS; pushing audio.");
		return gst_pad_push(filter->xsrcpad, audio_buffer);
	} else if (audio_buffer == NULL) {
		GST_DEBUG_OBJECT(filter,"Audio at EOS; pushing video;");
		return gst_pad_push(filter->ysrcpad, video_buffer);
	} else {
		GstFlowReturn aret,vret;
		GST_DEBUG_OBJECT(filter,"All good");
		vret = gst_pad_push(filter->ysrcpad, video_buffer);
		aret = gst_pad_push(filter->xsrcpad, audio_buffer);
//		FIXME: Write the logic that checks the return values
		return GST_FLOW_OK;
	}
}
//
//
//	if (video_frame == NULL) {
//		GST_DEBUG("Video stream at EOS");
//	} else {
//		if (GST_BUFFER_TIMESTAMP(video_frame) == GST_CLOCK_TIME_NONE) g_warning("%s: video frame has invalid timestamp", G_STRLOC);
////		GST_DEBUG ("Got video frame: %" GST_TIME_FORMAT " - %" GST_TIME_FORMAT, GST_TIME_ARGS (now), GST_TIME_ARGS (frame_end));
//		ret = gst_pad_push(filter->ysrcpad, video_frame);
//		return ret;
//	}
//
//	if (audio_buffer == NULL) {
//		GST_DEBUG("Audio buffer at EOS");
//	} else {
//		if (GST_BUFFER_TIMESTAMP(audio_buffer) == GST_CLOCK_TIME_NONE) g_warning("%s: audio buffer has invalid timestamp", G_STRLOC);
////		txt_end = GST_BUFFER_TIMESTAMP(audio_buffer) + GST_BUFFER_DURATION(audio_buffer);
////		GST_DEBUG ("Got text buffer: %" GST_TIME_FORMAT " - %" GST_TIME_FORMAT, GST_TIME_ARGS (GST_BUFFER_TIMESTAMP (text_buf)), GST_TIME_ARGS (txt_end));
//		ret = gst_pad_push(filter->xsrcpad, audio_buffer);
//		return ret;
//	}
//
//	if ((video_frame == NULL) | (audio_buffer == NULL)) {
//		gst_pad_push_event(filter->xsrcpad, gst_event_new_eos());
//		gst_pad_push_event(filter->ysrcpad, gst_event_new_eos());
//		ret = GST_FLOW_EOS;
//		goto done;
//	}
//
////	/* send EOS if video stream EOSed regardless of text stream */
////	if (video_frame == NULL) {
////		GST_DEBUG("Video stream at EOS");
////		if (filter->x_collect_data) {
////			audio_buffer = gst_collect_pads_pop(filter->collect, filter->x_collect_data);
////		}
////		gst_pad_push_event(filter->ysrcpad, gst_event_new_eos());
////		ret = GST_FLOW_EOS;
////		goto done;
////	}
//
////	if (GST_BUFFER_TIMESTAMP(video_frame) == GST_CLOCK_TIME_NONE) {
////		g_warning("%s: video frame has invalid timestamp", G_STRLOC);
////	}
//
////	now = GST_BUFFER_TIMESTAMP(video_frame);
////
////	if (GST_BUFFER_DURATION(video_frame) != GST_CLOCK_TIME_NONE) {
////		frame_end = now + GST_BUFFER_DURATION(video_frame);
////	} else if (filter->fps_n > 0) {
////		frame_end = now + gst_util_uint64_scale_int(GST_SECOND, filter->fps_d, filter->fps_n);
////	} else {
////		/* magic value, does not really matter since texts
////		 * tend to span quite a few frames in practice anyway */
////		frame_end = now + GST_SECOND / 25;
////	}
////
////	GST_DEBUG ("Got video frame: %" GST_TIME_FORMAT " - %" GST_TIME_FORMAT, GST_TIME_ARGS (now), GST_TIME_ARGS (frame_end));
////
////	/* text pad not linked? */
////	if (filter->x_collect_data == NULL) {
////		GST_DEBUG("Text pad not linked, rendering default text: '%s'", GST_STR_NULL(filter->default_text));
////		if (filter->default_text && *filter->default_text != '\0') {
////			gst_text_overlay_render_text(filter, filter->default_text, -1);
////			ret = gst_text_overlay_push_frame(filter, video_frame);
////		} else {
////			ret = gst_pad_push(filter->srcpad, video_frame);
////		}
////		gst_text_overlay_pop_video(filter);
////		video_frame = NULL;
////		goto done;
////	}
//
////	audio_buffer = gst_collect_pads_peek(filter->collect, filter->x_collect_data);
//
//	/* just push the video frame if the text stream has EOSed */
////	if (audio_buffer == NULL) {
////		GST_DEBUG("Text pad EOSed, just pushing video frame as is");
////		ret = gst_pad_push(filter->srcpad, video_frame);
////		gst_text_overlay_pop_video(filter);
////		video_frame = NULL;
////		goto done;
////	}
//	/* if the text buffer isn't stamped right, pop it off the
//	 *  queue and display it for the current video frame only */
////	if (GST_BUFFER_TIMESTAMP(audio_buffer) == GST_CLOCK_TIME_NONE || GST_BUFFER_DURATION(audio_buffer) == GST_CLOCK_TIME_NONE) {
////		GST_WARNING("Got text buffer with invalid time stamp or duration");
////		gst_text_overlay_pop_text(filter);
////		GST_BUFFER_TIMESTAMP (text_buf) = now;
////		GST_BUFFER_DURATION (text_buf) = frame_end - now;
////	}
////	txt_end = GST_BUFFER_TIMESTAMP(audio_buffer) + GST_BUFFER_DURATION(audio_buffer);
////
////	GST_DEBUG ("Got text buffer: %" GST_TIME_FORMAT " - %" GST_TIME_FORMAT, GST_TIME_ARGS (GST_BUFFER_TIMESTAMP (text_buf)), GST_TIME_ARGS (txt_end));
//	/* if the text buffer is too old, pop it off the
//	 * queue and return so we get a new one next time */
////	if (txt_end < now) {
////		GST_DEBUG("Text buffer too old, popping off the queue");
////		gst_text_overlay_pop_text(filter);
////		ret = GST_FLOW_OK;
////		goto done;
////	}
//	/* if the video frame ends before the text even starts,
//	 * just push it out as is and pop it off the queue */
////	if (frame_end < GST_BUFFER_TIMESTAMP(audio_buffer)) {
////		GST_DEBUG("Video buffer before text, pushing out and popping off queue");
////		ret = gst_pad_push(filter->srcpad, video_frame);
////		gst_text_overlay_pop_video(filter);
////		video_frame = NULL;
////		goto done;
////	}
//	/* text duration overlaps video frame duration */
////	text = g_strndup((gchar *) GST_BUFFER_DATA(audio_buffer), GST_BUFFER_SIZE(audio_buffer));
////	g_strdelimit(text, "\n\r\t", ' ');
////	text_len = strlen(text);
////	if (text_len > 0) {
////		GST_DEBUG("Rendering text '%*s'", text_len, text);
////		;
////		gst_text_overlay_render_text(filter, text, text_len);
////	} else {
////		GST_DEBUG("No text to render (empty buffer)");
////		gst_text_overlay_render_text(filter, " ", 1);
////	}
////
////	g_free(text);
////
////	gst_text_overlay_pop_video(filter);
////	ret = gst_text_overlay_push_frame(filter, video_frame);
////	video_frame = NULL;
////	goto done;
//	done: {
//		if (audio_buffer)
//			gst_buffer_unref(audio_buffer);
//
//		if (video_frame)
//			gst_buffer_unref(video_frame);
//
//		return ret;
//	}
//}

/* entry point to initialize the plug-in
 * initialize the plug-in itself
 * register the element factories and other features
 */
static gboolean collectpadsnoop_init(GstPlugin * collectpadsnoop) {
	/* debug category for fltering log messages
	 *
	 * exchange the string 'Template collectpadsnoop' with your description
	 */
	GST_DEBUG_CATEGORY_INIT(gst_collect_pads_noop_debug, "collectpadsnoop", 0, "Template collectpadsnoop");
	return gst_element_register(collectpadsnoop, "collectpadsnoop", GST_RANK_NONE, GST_TYPE_COLLECTPADSNOOP);
}

static GstStateChangeReturn gst_text_overlay_change_state(GstElement * element, GstStateChange transition) {

	GstStateChangeReturn ret = GST_STATE_CHANGE_SUCCESS;
	GstCollectPadsNoop *filter = GST_COLLECTPADSNOOP (element);

	switch (transition) {
	case GST_STATE_CHANGE_READY_TO_PAUSED:
		gst_collect_pads_start(filter->collect);
		break;
	case GST_STATE_CHANGE_PAUSED_TO_READY:
		/* need to unblock the collectpads before calling the
		 * parent change_state so that streaming can finish */
		gst_collect_pads_stop(filter->collect);
		break;
	default:
		break;
	}

	ret = GST_ELEMENT_CLASS(parent_class)->change_state(element, transition);
	if (ret == GST_STATE_CHANGE_FAILURE) return ret;

	switch (transition) {
	default:
		break;
	}

	return ret;
}

/* PACKAGE: this is usually set by autotools depending on some _INIT macro
 * in configure.ac and then written into and defined in config.h, but we can
 * just set it ourselves here in case someone doesn't use autotools to
 * compile this code. GST_PLUGIN_DEFINE needs PACKAGE to be defined.
 */
#ifndef PACKAGE
#define PACKAGE "collectpadsnoop"
#endif

/* gstreamer looks for this structure to register collectpadsnoops
 *
 * exchange the string 'Template collectpadsnoop' with your collectpadsnoop description
 */
GST_PLUGIN_DEFINE (
		GST_VERSION_MAJOR,
		GST_VERSION_MINOR,
		collectpadsnoop,
		"Template collectpadsnoop",
		collectpadsnoop_init,
		VERSION,
		"LGPL",
		"GStreamer",
		"http://gstreamer.net/"
)
