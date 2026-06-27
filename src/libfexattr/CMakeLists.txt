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

# Sources
SET(${PROJECT_NAME}_SRCS
	XAttrReader.cpp
	)
# Headers
SET(${PROJECT_NAME}_H
	XAttrReader.hpp
	)

# Write the config.h files.
CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/config.lib${PROJECT_NAME}.h.in" "${CMAKE_CURRENT_BINARY_DIR}/config.lib${PROJECT_NAME}.h")

######################
# Build the library. #
######################

ADD_LIBRARY(${PROJECT_NAME} STATIC
	${${PROJECT_NAME}_SRCS}
	${${PROJECT_NAME}_H}
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
