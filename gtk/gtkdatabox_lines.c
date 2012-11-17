/* $Id: gtkdatabox_lines.c 4 2008-06-22 09:19:11Z rbock $ */
/* GtkDatabox - An extension to the gtk+ library
 * Copyright (C) 1998 - 2008  Dr. Roland Bock
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include <gtkdatabox_lines.h>

static void gtk_databox_lines_real_draw (GtkDataboxGraph * lines,
					 GtkDatabox* box);

struct _GtkDataboxLinesPrivate
{
   gint16 *xpixels;
   gint16 *ypixels;
   guint pixelsalloc;
};

static gpointer parent_class = NULL;

static void
lines_finalize (GObject * object)
{
   GtkDataboxLines *lines = GTK_DATABOX_LINES (object);

   g_free (lines->priv->xpixels);
   g_free (lines->priv->ypixels);
   g_free (lines->priv);

   /* Chain up to the parent class */
   G_OBJECT_CLASS (parent_class)->finalize (object);
}

static void
gtk_databox_lines_class_init (gpointer g_class /*, gpointer g_class_data */ )
{
   GObjectClass *gobject_class = G_OBJECT_CLASS (g_class);
   GtkDataboxGraphClass *graph_class = GTK_DATABOX_GRAPH_CLASS (g_class);
   GtkDataboxLinesClass *klass = GTK_DATABOX_LINES_CLASS (g_class);

   parent_class = g_type_class_peek_parent (klass);

   gobject_class->finalize = lines_finalize;

   graph_class->draw = gtk_databox_lines_real_draw;
}

static void
gtk_databox_lines_instance_init (GTypeInstance * instance	/*,
								   gpointer g_class */ )
{
   GtkDataboxLines *lines = GTK_DATABOX_LINES (instance);

   lines->priv = g_new0 (GtkDataboxLinesPrivate, 1);
   lines->priv->xpixels = NULL;
   lines->priv->ypixels = NULL;
   lines->priv->pixelsalloc = 0;
}

GType
gtk_databox_lines_get_type (void)
{
   static GType type = 0;

   if (type == 0)
   {
      static const GTypeInfo info = {
	 sizeof (GtkDataboxLinesClass),
	 NULL,			/* base_init */
	 NULL,			/* base_finalize */
	 (GClassInitFunc) gtk_databox_lines_class_init,	/* class_init */
	 NULL,			/* class_finalize */
	 NULL,			/* class_data */
	 sizeof (GtkDataboxLines),	/* instance_size */
	 0,			/* n_preallocs */
	 (GInstanceInitFunc) gtk_databox_lines_instance_init,	/* instance_init */
	 NULL,			/* value_table */
      };
      type = g_type_register_static (GTK_DATABOX_TYPE_XYC_GRAPH,
				     "GtkDataboxLines", &info, 0);
   }

   return type;
}

/**
 * gtk_databox_lines_new:
 * @len: length of @X and @Y
 * @X: array of horizontal position values of markers
 * @Y: array of vertical position values of markers
 * @color: color of the markers
 * @size: marker size or line width (depending on the @type)
 *
 * Creates a new #GtkDataboxLines object which can be added to a #GtkDatabox widget.
 *
 * Return value: A new #GtkDataboxLines object
 **/
GtkDataboxGraph *
gtk_databox_lines_new (guint len, gfloat * X, gfloat * Y,
		       GdkColor * color, guint size)
{
   GtkDataboxLines *lines;
   g_return_val_if_fail (X, NULL);
   g_return_val_if_fail (Y, NULL);
   g_return_val_if_fail ((len > 0), NULL);

   lines = g_object_new (GTK_DATABOX_TYPE_LINES,
			 "X-Values", X,
			 "Y-Values", Y,
			 "xstart", 0,
			 "ystart", 0,
			 "xstride", 1,
			 "ystride", 1,
			 "xtype", G_TYPE_FLOAT,
			 "ytype", G_TYPE_FLOAT,
			 "length", len,
			 "maxlen", len,
			 "color", color, "size", size, NULL);

   return GTK_DATABOX_GRAPH (lines);
}

/**
 * gtk_databox_lines_new_full:
 * @maxlen: maximum length of @X and @Y
 * @len: actual number of @X and @Y values to plot
 * @X: array of horizontal position values of markers
 * @Y: array of vertical position values of markers
 * @xstart: the first element in the X array to plot (usually 0)
 * @ystart: the first element in the Y array to plot (usually 0)
 * @xstride: successive elements in the X array are separated by this much (1 if array, ncols if matrix)
 * @ystride: successive elements in the Y array are separated by this much (1 if array, ncols if matrix)
 * @xtype: the GType of the X array elements.  G_TYPE_FLOAT, G_TYPE_DOUBLE, etc.
 * @ytype: the GType of the Y array elements.  G_TYPE_FLOAT, G_TYPE_DOUBLE, etc.
 * @color: color of the markers
 * @size: marker size or line width (depending on the @type)
 *
 * Creates a new #GtkDataboxLines object which can be added to a #GtkDatabox widget.
 *
 * Return value: A new #GtkDataboxLines object
 **/
GtkDataboxGraph *
gtk_databox_lines_new_full (guint maxlen, guint len,
			void * X, guint xstart, guint xstride, GType xtype,
			void * Y, guint ystart, guint ystride, GType ytype,
		    GdkColor * color, guint size)
{
   GtkDataboxLines *lines;
   g_return_val_if_fail (X, NULL);
   g_return_val_if_fail (Y, NULL);
   g_return_val_if_fail ((len > 0), NULL);

   lines = g_object_new (GTK_DATABOX_TYPE_LINES,
			 "X-Values", X,
			 "Y-Values", Y,
			 "xstart", xstart,
			 "ystart", ystart,
			 "xstride", xstride,
			 "ystride", ystride,
			 "xtype", xtype,
			 "ytype", ytype,
			 "length", len,
			 "maxlen", maxlen,
			 "color", color, "size", size, NULL);

   return GTK_DATABOX_GRAPH (lines);
}

static void
gtk_databox_lines_real_draw (GtkDataboxGraph * graph,
			     GtkDatabox * box)
{
   GtkDataboxLines *lines = GTK_DATABOX_LINES (graph);
   guint i = 0;
   void *X;
   void *Y;
   guint len, maxlen;
   cairo_t *cr;
   gint16 *xpixels, *ypixels;
   guint xstart, xstride, ystart, ystride;
   GType xtype, ytype;
   float linewidth;

   g_return_if_fail (GTK_DATABOX_IS_LINES (lines));
   g_return_if_fail (GTK_IS_DATABOX (box));

   len = gtk_databox_xyc_graph_get_length (GTK_DATABOX_XYC_GRAPH (graph));
   maxlen = gtk_databox_xyc_graph_get_maxlen (GTK_DATABOX_XYC_GRAPH (graph));

   if (lines->priv->pixelsalloc < len)
   {
   	lines->priv->pixelsalloc = len;
	lines->priv->xpixels = (gint16 *)g_realloc(lines->priv->xpixels, len * sizeof(gint16));
	lines->priv->ypixels = (gint16 *)g_realloc(lines->priv->ypixels, len * sizeof(gint16));
   }

   xpixels = lines->priv->xpixels;
   ypixels = lines->priv->ypixels;

   X = gtk_databox_xyc_graph_get_X (GTK_DATABOX_XYC_GRAPH (graph));
   xstart = gtk_databox_xyc_graph_get_xstart (GTK_DATABOX_XYC_GRAPH (graph));
   xstride = gtk_databox_xyc_graph_get_xstride (GTK_DATABOX_XYC_GRAPH (graph));
   xtype = gtk_databox_xyc_graph_get_xtype (GTK_DATABOX_XYC_GRAPH (graph));
   gtk_databox_values_to_xpixels(box, xpixels, X, xtype, maxlen, xstart, xstride, len);

   Y = gtk_databox_xyc_graph_get_Y (GTK_DATABOX_XYC_GRAPH (graph));
   ystart = gtk_databox_xyc_graph_get_ystart (GTK_DATABOX_XYC_GRAPH (graph));
   ystride = gtk_databox_xyc_graph_get_ystride (GTK_DATABOX_XYC_GRAPH (graph));
   ytype = gtk_databox_xyc_graph_get_ytype (GTK_DATABOX_XYC_GRAPH (graph));
   gtk_databox_values_to_ypixels(box, ypixels, Y, ytype, maxlen, ystart, ystride, len);

   cr = gtk_databox_graph_create_gc (graph, box);

   linewidth = gtk_databox_graph_get_size (graph);
   cairo_set_line_width(cr, linewidth + 0.1);

   cairo_move_to(cr, xpixels[0] + 0.5, ypixels[0] + 0.5);
   for (i = 1; i < len; i++)
	  cairo_line_to(cr, xpixels[i] + 0.5, ypixels[i] + 0.5);

   cairo_stroke(cr);
   cairo_destroy(cr);

   return;
}

