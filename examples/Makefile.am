# $Id: Makefile.am 4 2008-06-22 09:19:11Z rbock $

noinst_PROGRAMS         = \
                          basics\
                          basics2\
                          full\
                          logarithmic\
                          rulers\
                          keycontrol\
                          colors\
                          markers\
                          grid\
                          grid_array\
                          signals\
                          lissajous\
			              basics_libglade \
                          enable_disable\
                          addremove


basics_SOURCES          = basics.c

basics2_SOURCES         = basics2.c

full_SOURCES         	= full.c

logarithmic_SOURCES     = logarithmic.c

rulers_SOURCES     = rulers.c

keycontrol_SOURCES      = keycontrol.c

colors_SOURCES          = colors.c

markers_SOURCES          = markers.c

grid_SOURCES            = grid.c

grid_array_SOURCES      = grid_array.c

signals_SOURCES         = signals.c

lissajous_SOURCES       = lissajous.c

enable_disable_SOURCES  = enable_disable.c

addremove_SOURCES       = addremove.c

basics_libglade_SOURCES = basics_libglade.c
EXTRA_DIST              = basics_libglade.glade

INCLUDES		= \
			-I$(top_srcdir)/gtk

AM_CFLAGS			=\
			-O2\
			-W -Wall -pedantic\
			-DG_DISABLE_DEPRECATED\
			-DGDK_DISABLE_DEPRECATED\
			-DGDK_PIXBUF_DISABLE_DEPRECATED\
			-DGTK_DISABLE_DEPRECATED\
			-DGSEAL_ENABLED\
			-DGTK_DISABLE_SINGLE_INCLUDES\
			@LIBGLADE_CFLAGS@ \
			@GTK_CFLAGS@


LDADD 			= $(top_builddir)/gtk/libgtkdatabox.la\
		 	@GTK_LIBS@ \
			-lm


if BUILD_LIBGLADE
basics_libglade_LDADD	= $(top_builddir)/gtk/libgtkdatabox.la \
			$(top_builddir)/glade/libdatabox.la \
		 	@GTK_LIBS@ \
			@LIBGLADE_LIBS@
else
basics_libglade_LDADD = $(LDADD)
endif

