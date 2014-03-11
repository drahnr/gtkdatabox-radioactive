#! /usr/bin/env python
# encoding: utf-8

VERSION = '0.0.1'
LIBNAME = 'gtkdatabox'

top = '.'
out = 'build'

import os
from waflib import Logs as logs
from waflib import Utils as utils

def options(ctx):
	ctx.load('compiler_c gnu_dirs glib2')


def configure(ctx):
	ctx.load('compiler_c gnu_dirs glib2')

	ctx.env.appname = LIBNAME
	ctx.env.version = VERSION

	ctx.define('VERSION', VERSION)
	ctx.define('GETTEXT_PACKAGE', LIBNAME)


	#things the applications needs to know about, for easier re-use in subdir wscript(s)
	ctx.env.path_ui = utils.subst_vars('${DATADIR}/gtkdatabox/ui/', ctx.env)
	ctx.env.path_examples =  utils.subst_vars('${DATADIR}/gtkdatabox/examples/', ctx.env)

	#define the above paths so the application does know about files locations
	ctx.define('LIBGTKDATABOX_UIDIR', ctx.env.path_ui)
	ctx.define('LIBGTKDATABOX_EXAMPLEDIR', ctx.env.path_examples)




	ctx.check_cc(lib='m', uselib_store='M', mandatory=True)

	ctx.check_cfg(atleast_pkgconfig_version='0.26')
	ctx.check_cfg(package='glib-2.0', uselib_store='GLIB', args=['glib-2.0 >= 2.24', '--cflags', '--libs'], mandatory=True)
	ctx.check_cfg(package='gobject-2.0', uselib_store='GOBJECT', args=['--cflags', '--libs'], mandatory=True)
	ctx.check_cfg(package='gtk+-3.0', uselib_store='GTK3', args=['--cflags', '--libs'], mandatory=True)

	ctx.check_large_file(mandatory=False)
	ctx.check_endianness(mandatory=False)
	ctx.check_inline(mandatory=False)

	# -ggdb vs -g -- http://stackoverflow.com/questions/668962
	ctx.setenv('debug', env=ctx.env.derive())
	ctx.env.CFLAGS = ['-ggdb', '-Wall']
	ctx.define('DEBUG',1)

	ctx.setenv('release', env=ctx.env.derive())
	ctx.env.CFLAGS = ['-O2', '-Wall']
	ctx.define('RELEASE',1)



def dist(ctx):
	ctx.base_name = LIBNAME+'-'+VERSION
	ctx.algo = 'tar.xz'
	ctx.excl = ['.*', '*~','./build','*.'+ctx.algo],
	ctx.files = ctx.path.ant_glob('**/wscript')



def pre(ctx):
	if ctx.cmd != 'install':
		logs.info ('Variant: \'' + ctx.variant + '\'')




def build(bld):
	bld.add_pre_fun(pre)

	if bld.cmd != 'install' and bld.cmd != 'uninstall':
		if not bld.variant:
			bld.variant = 'debug'
			logs.warn ('Defaulting to \'debug\' build variant')
			logs.warn ('Do "waf debug" or "waf release" to avoid this warning')
		if os.geteuid()==0:
			logs.fatal ('Do not run "' + ctx.cmd + '" as root, just don\'t!. Aborting.')
	else:
		if not os.geteuid()==0:
			logs.warn ('You most likely need root privileges to install or uninstall '+LIBNAME+' properly.')



	logs.info ("::: "+str(os.getcwd())+":::")

	lib = bld.shlib (
		['c','glib2'],
		source = bld.path.ant_glob(['src/*.c'], excl='*/main.c'),
		target = LIBNAME,
		includes = ['src/'],
		export_includes = ['src/'],
		uselib = 'M GOBJECT GLIB GTK3',
		install_path = "${LIBDIR}"
	)

	for item in lib.includes:
		logs.debug(item)

	nodes = bld.path.ant_glob(['examples/*.c'], excl='*/Gtk+IOStream.C'),
	for node in nodes[0]:
		print (node)
		filename = os.path.basename (node.abspath())
		test = bld.program(
			features = ['c', 'glib2'],
			target = os.path.splitext(filename)[0]+"-test",
			source = "examples/"+filename,
			use = LIBNAME,
			uselib = 'M GOBJECT GLIB GTK3'
		)


from waflib.Build import BuildContext


def gdb(ctx):
	os.system ("G_DEBUG=resident-modules,fatal-warnings gdb --args ./build/debug/oregano --debug-boundingboxes --debug-wires")

class release(BuildContext):
	"""compile release binary"""
	cmd = 'release'
	variant = 'release'

class debug(BuildContext):
	"""compile debug binary"""
	cmd = 'debug'
	variant = 'debug'
