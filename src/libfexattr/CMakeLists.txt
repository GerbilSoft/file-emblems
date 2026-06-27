# rom-properties file handling library
PROJECT(fexattr LANGUAGES CXX)

# Check for an xattr header.
INCLUDE(CheckIncludeFile)
CHECK_INCLUDE_FILE("sys/xattr.h" HAVE_SYS_XATTR_H)
CHECK_INCLUDE_FILE("sys/extattr.h" HAVE_SYS_EXTATTR_H)

# Check for an xattr function.
INCLUDE(CheckSymbolExists)
IF(HAVE_SYS_XATTR_H)
	IF(APPLE)
		CHECK_SYMBOL_EXISTS(fsetxattr "sys/xattr.h" HAVE_FSETXATTR_MAC)
	ELSE(APPLE)
		CHECK_SYMBOL_EXISTS(fsetxattr "sys/xattr.h" HAVE_FSETXATTR_LINUX)
	ENDIF(APPLE)
ELSEIF(HAVE_SYS_EXTATTR_H)
	CHECK_SYMBOL_EXISTS(extattr_set_fd "sys/extattr.h" HAVE_EXTATTR_SET_FD)
ENDIF()

IF(NOT HAVE_FSETXATTR_MAC AND NOT HAVE_FSETXATTR_LINUX AND NOT HAVE_EXTATTR_SET_FD)
	MESSAGE(FATAL_ERROR "Unable to find a valid xattr function!")
ENDIF(NOT HAVE_FSETXATTR_MAC AND NOT HAVE_FSETXATTR_LINUX AND NOT HAVE_EXTATTR_SET_FD)

# Check for GLib.
IF(ENABLE_GVFS)
	# TODO: Determine the actual minimum GLib version.
	SET(GLIB_MIN_VERSION 2.34.0)
	FIND_PACKAGE(GLib2 ${REQUIRE_GVFS} ${GLIB_MIN_VERSION})
	FIND_PACKAGE(GObject2 ${REQUIRE_GVFS} ${GLIB_MIN_VERSION})
	FIND_PACKAGE(GIO ${REQUIRE_GVFS} ${GLIB_MIN_VERSION})
	FIND_PACKAGE(GIO-UNIX ${REQUIRE_GVFS} ${GLIB_MIN_VERSION})
	IF(GLib2_FOUND AND GObject2_FOUND AND GIO_FOUND AND GIO-UNIX_FOUND)
		# Ensure we don't use functions not available in glib-2.34.
		SET(GLIB_DEFINITIONS -DGLIB_VERSION_MIN_REQUIRED=GLIB_VERSION_2_34 -DGLIB_VERSION_MAX_ALLOWED=GLIB_VERSION_2_34)
	ELSE()
		# GLib not found; cannot enable GVfs integration.
		# TODO: Show an error message?
		SET(ENABLE_GVFS OFF CACHE INTERNAL "Enable GVfs integration to read emblems from Nautilus and Thunar." FORCE)
	ENDIF()
ENDIF(ENABLE_GVFS)

# Sources
SET(${PROJECT_NAME}_SRCS
	XAttrReader.cpp
	)
# Headers
SET(${PROJECT_NAME}_H
	IEmblemReader.hpp
	XAttrReader.hpp
	)

IF(ENABLE_GVFS)
	SET(${PROJECT_NAME}_GVfs_SRCS GVfsReader.cpp)
	SET(${PROJECT_NAME}_GVfs_H GVfsReader.hpp)
ENDIF(ENABLE_GVFS)

# Write the config.h files.
CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/config.lib${PROJECT_NAME}.h.in" "${CMAKE_CURRENT_BINARY_DIR}/config.lib${PROJECT_NAME}.h")

######################
# Build the library. #
######################

ADD_LIBRARY(${PROJECT_NAME} STATIC
	${${PROJECT_NAME}_SRCS} ${${PROJECT_NAME}_H}
	${${PROJECT_NAME}_GVfs_SRCS} ${${PROJECT_NAME}_GVfs_H}
	)
INCLUDE(SetMSVCDebugPath)
SET_MSVC_DEBUG_PATH(${PROJECT_NAME})
# Exclude from ALL builds; enable PIC.
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES
	EXCLUDE_FROM_ALL TRUE
	POSITION_INDEPENDENT_CODE TRUE
	)
# Make sure git_version.h is created before compiling this target.
IF(TARGET git_version)
	ADD_DEPENDENCIES(${PROJECT_NAME} git_version)
ENDIF(TARGET git_version)

IF(ENABLE_GVFS)
	# Need to link in the various GLib libraries.
	TARGET_LINK_LIBRARIES(${PROJECT_NAME} PUBLIC GLib2::gio-unix GLib2::gio GLib2::gobject GLib2::glib)
	IF(GLIB_DEFINITIONS)
		TARGET_COMPILE_DEFINITIONS(${PROJECT_NAME} PUBLIC ${GLIB_DEFINITIONS})
	ENDIF(GLIB_DEFINITIONS)
ENDIF(ENABLE_GVFS)

# Include paths:
# - Public: Current source and binary directories.
# - Private: Parent source and binary directories,
#            and top-level binary directory for git_version.h.
TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME}
	PUBLIC  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>		# librpfile
		$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>		# librpfile
	PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>	# src
		$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/..>	# src
		$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>			# build
	)
