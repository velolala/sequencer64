#******************************************************************************
# Makefile.am (seq_rtmidi/src)
#------------------------------------------------------------------------------
##
# \file       	Makefile.am
# \library    	seq_rtmidi library
# \author     	Chris Ahlstrom
# \date       	2016-11-19
# \update      2017-01-04
# \version    	$Revision$
# \license    	$XPC_SUITE_GPL_LICENSE$
#
# 		This module provides an Automake makefile for the seq_rtmidi C/C++
# 		library.
#
#------------------------------------------------------------------------------

#*****************************************************************************
# Packing/cleaning targets
#-----------------------------------------------------------------------------

AUTOMAKE_OPTIONS = foreign dist-zip dist-bzip2
MAINTAINERCLEANFILES = Makefile.in Makefile $(AUX_DIST)

#******************************************************************************
# CLEANFILES
#------------------------------------------------------------------------------

CLEANFILES = *.gc*
MOSTLYCLEANFILES = *~

#******************************************************************************
#  EXTRA_DIST
#------------------------------------------------------------------------------

EXTRA_DIST =

#******************************************************************************
# Items from configure.ac
#-------------------------------------------------------------------------------

PACKAGE = @PACKAGE@
VERSION = @VERSION@
GIT_VERSION = @GIT_VERSION@

SEQ64_API_MAJOR = @SEQ64_API_MAJOR@
SEQ64_API_MINOR = @SEQ64_API_MINOR@
SEQ64_API_PATCH = @SEQ64_API_PATCH@
SEQ64_API_VERSION = @SEQ64_API_VERSION@

SEQ64_LT_CURRENT = @SEQ64_LT_CURRENT@
SEQ64_LT_REVISION = @SEQ64_LT_REVISION@
SEQ64_LT_AGE = @SEQ64_LT_AGE@

#******************************************************************************
# Install directories
#------------------------------------------------------------------------------

prefix = @prefix@
includedir = @sequencer64includedir@
libdir = @sequencer64libdir@
datadir = @datadir@
datarootdir = @datarootdir@
sequencer64includedir = @sequencer64includedir@
sequencer64libdir = @sequencer64libdir@

#******************************************************************************
# localedir
#------------------------------------------------------------------------------
#
# 	'localedir' is the normal system directory for installed localization
#  files.
#
#------------------------------------------------------------------------------

localedir = $(datadir)/locale
DEFS = -DLOCALEDIR=\"$(localedir)\" @DEFS@

#******************************************************************************
# Local project directories
#------------------------------------------------------------------------------

top_srcdir = @top_srcdir@
builddir = @abs_top_builddir@

#*****************************************************************************
# libtool
#-----------------------------------------------------------------------------

version = $(SEQ64_API_MAJOR):$(SEQ64_API_MINOR):$(SEQ64_API_PATCH)

#*****************************************************************************
# git_version
#-----------------------------------------------------------------------------
# git_version = $(shell git describe --abbrev=7 --always --tags)
#-----------------------------------------------------------------------------

git_version = $(shell git describe --tags --long)
git_branch =$(shell git branch | grep -e ^*)
git_info = "$(git_version) $(git_branch)"

#******************************************************************************
# Compiler and linker flags
#
#  $(GTKMM_CFLAGS)
#
#------------------------------------------------------------------------------

AM_CXXFLAGS = \
 -I../include \
 -I$(top_srcdir)/include \
 -I$(top_srcdir)/libseq64/include \
 -I$(top_srcdir)/seq_rtmidi/include \
 $(ALSA_CFLAGS) \
 $(JACK_CFLAGS) \
 -DSEQ64_GIT_VERSION=\"$(git_info)\"

#******************************************************************************
# The library to build, a libtool-based library
#------------------------------------------------------------------------------

lib_LTLIBRARIES = libseq_rtmidi.la

#******************************************************************************
# Source files
#
#  We include only the JACK and ALSA support here.
#
# midi_jack.cpp
#
#----------------------------------------------------------------------------

libseq_rtmidi_la_SOURCES = \
   mastermidibus.cpp \
   midibus.cpp \
	midi_alsa.cpp \
	midi_alsa_info.cpp \
	midi_api.cpp \
	midi_info.cpp \
	midi_jack.cpp \
	midi_jack_info.cpp \
	midi_probe.cpp \
	rtmidi.cpp \
	rtmidi_info.cpp \
	rtmidi_types.cpp

libseq_rtmidi_la_LDFLAGS = -version-info $(version)
libseq_rtmidi_la_LIBADD = $(ALSA_LIBS) $(JACK_LIBS)

#******************************************************************************
# uninstall-hook
#------------------------------------------------------------------------------
#
#     We'd like to remove /usr/local/include/sequencer64-1.0 if it is
#     empty.  However, we don't have a good way to do it yet.
#
#------------------------------------------------------------------------------

uninstall-hook:
	@echo "Note:  you may want to remove $(libdir) manually"

#******************************************************************************
# Makefile.am (seq_rtmidi/src)
#------------------------------------------------------------------------------
# 	vim: ts=3 sw=3 ft=automake
#------------------------------------------------------------------------------
