# Find GIO libraries and headers.
# If found, the following variables will be defined:
# - GIO_FOUND: System has GLib2.
# - GIO_INCLUDE_DIRS: GLib2 include directories.
# - GIO_LIBRARIES: GLib2 libraries.
# - GIO_DEFINITIONS: Compiler switches required for using GLib2.
#
# In addition, a target GLib2::gio will be created with all of
# these definitions.
#
# References:
# - https://cmake.org/Wiki/CMake:How_To_Find_Libraries
# - http://francesco-cek.com/cmake-and-gtk-3-the-easy-way/
#

INCLUDE(FindLibraryPkgConfig)
FIND_LIBRARY_PKG_CONFIG(GIO
	gio-2.0	# pkgconfig
	gio/gio.h	# header
	gio-2.0		# library
	GLib2::gio	# imported target
	)
