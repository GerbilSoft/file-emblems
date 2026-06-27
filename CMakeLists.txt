# File Emblems
CMAKE_MINIMUM_REQUIRED(VERSION 4.0)

# Read the project version.
CONFIGURE_FILE(version.txt version.txt.tmp)
FILE(STRINGS version.txt FE_VERSION_FILE)
IF(NOT FE_VERSION_FILE)
	MESSAGE(FATAL_ERROR "version.txt not found; cannot determine project version.")
ENDIF(NOT FE_VERSION_FILE)
LIST(GET FE_VERSION_FILE 0 FE_VERSION)
UNSET(FE_VERSION_FILE)
PROJECT(file-emblems VERSION ${FE_VERSION})
IF(CMAKE_PROJECT_VERSION_PATCH)
	SET(VERSION_STRING "${CMAKE_PROJECT_VERSION_MAJOR}.${CMAKE_PROJECT_VERSION_MINOR}.${CMAKE_PROJECT_VERSION_PATCH}")
ELSE(CMAKE_PROJECT_VERSION_PATCH)
	SET(VERSION_STRING "${CMAKE_PROJECT_VERSION_MAJOR}.${CMAKE_PROJECT_VERSION_MINOR}")
ENDIF(CMAKE_PROJECT_VERSION_PATCH)
IF(CMAKE_PROJECT_VERSION_TWEAK)
	SET(VERSION_STRING "${VERSION_STRING}+")
ENDIF(CMAKE_PROJECT_VERSION_TWEAK)
SET(VERSION_STRING_WIN32 "${CMAKE_PROJECT_VERSION_MAJOR},${CMAKE_PROJECT_VERSION_MINOR},${CMAKE_PROJECT_VERSION_PATCH},${CMAKE_PROJECT_VERSION_TWEAK}")

LIST(APPEND CMAKE_MODULE_PATH
	"${CMAKE_SOURCE_DIR}/cmake/macros"
	"${CMAKE_SOURCE_DIR}/cmake/libs"
	)

# Put all the binaries and libraries into a single directory.
# NOTE: CACHE INTERNAL is required in order to get this to work
# for KF5 for some reason. (and maybe that's why KDE4 did this
# layout by default?)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin" CACHE INTERNAL "Put all binaries in a single directory.")
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib" CACHE INTERNAL "Put all libraries in a single directory.")
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib" CACHE INTERNAL "Put all archives in a single directory.")

# Don't re-link executables if a shared library's implementation has
# changed but its headers haven't changed.
SET(CMAKE_LINK_DEPENDS_NO_SHARED ON)

# Set the standards versions. (Minimum C11, C++14)
# C17 was added in CMake 3.21; for older versions, use C11.
# C++17 was added in CMake 3.8; for older versions, use C++14.
# C++20 was added in CMake 3.12; for older versions, use C++17.
# - C++20 is needed for std::span<>.
# NOTE: These aren't set as hard requirements, though if the compiler
# doesn't support them, code will either be less optimal or will fail
# to compile.
SET(CMAKE_C_STANDARD_REQUIRED OFF)
SET(CMAKE_C_EXTENSIONS OFF)
SET(CMAKE_CXX_STANDARD_REQUIRED OFF)
SET(CMAKE_CXX_EXTENSIONS OFF)
SET(CMAKE_C_STANDARD 17)
SET(CMAKE_CXX_STANDARD 20)

# Set default build options.
INCLUDE(cmake/options.cmake)
# Check for platform-specific functionality.
#INCLUDE(cmake/platform.cmake NO_POLICY_SCOPE)

# Program information.
SET(DESCRIPTION "File Emblems")
SET(PACKAGE_NAME "file-emblems")
SET(AUTHOR "David Korth")

# Split Debug function.
# Also sets the image version for Windows builds.
# TODO: Move to SplitDebugInformation.cmake?
FUNCTION(DO_SPLIT_DEBUG _target)
	GET_TARGET_PROPERTY(_target_type ${_target} TYPE)
	IF(TARGET ${_target} AND NOT _target_type STREQUAL "STATIC_LIBRARY")
		# Split debug information.
		INCLUDE(SetMSVCDebugPath)
		SET_MSVC_DEBUG_PATH(${_target})
		IF(SPLIT_DEBUG)
			INCLUDE(SplitDebugInformation)
			SPLIT_DEBUG_INFORMATION(${_target})
		ENDIF(SPLIT_DEBUG)
	ENDIF(TARGET ${_target} AND NOT _target_type STREQUAL "STATIC_LIBRARY")
ENDFUNCTION(DO_SPLIT_DEBUG)

# Git version information.
FIND_PROGRAM(POSIX_SH sh)
IF(POSIX_SH)
	# sh is available.
	# Run the git version script.
	IF(WIN32)
		SET(ENV{SHELLOPTS} igncr)
	ENDIF(WIN32)
	ADD_CUSTOM_TARGET(git_version ALL
		${POSIX_SH} "${CMAKE_SOURCE_DIR}/git_version.sh"
		-s "${CMAKE_SOURCE_DIR}"
		-o "${CMAKE_BINARY_DIR}/git_version.h"
		VERBATIM
		)
ELSE(POSIX_SH)
	# sh isn't available.
	# Create a blank git_version.h.
	FILE(WRITE "${CMAKE_BINARY_DIR}/git_version.h"
		"/* dummy file; POSIX sh is not available */\n")
	# Dummy target for dependencies.
	ADD_CUSTOM_TARGET(git_version)
ENDIF(POSIX_SH)

# Make sure the file is deleted on `make clean`.
SET_PROPERTY(DIRECTORY APPEND
	PROPERTY ADDITIONAL_MAKE_CLEAN_FILES "${CMAKE_BINARY_DIR}/git_version.h")

# Reference: https://cmake.org/Wiki/RecipeAddUninstallTarget
########### Add uninstall target ###############
CONFIGURE_FILE(
	"${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
	"${CMAKE_CURRENT_BINARY_DIR}/cmake/cmake_uninstall.cmake"
	IMMEDIATE @ONLY)
ADD_CUSTOM_TARGET(uninstall
	"${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake/cmake_uninstall.cmake")

### Subdirectories ###

# Project subdirectories
ADD_SUBDIRECTORY(src)
IF(INSTALL_DOC)
#	ADD_SUBDIRECTORY(doc)
ENDIF(INSTALL_DOC)

### CPack ###

# CPack settings
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${DESCRIPTION}")
SET(CPACK_PACKAGE_NAME "${PACKAGE_NAME}")
SET(CPACK_PACKAGE_VENDOR "${AUTHOR}")
SET(CPACK_PACKAGE_CONTACT "David Korth <gerbilsoft@gerbilsoft.com>")
SET(CPACK_PACKAGE_VERSION_MAJOR ${CMAKE_PROJECT_VERSION_MAJOR})
SET(CPACK_PACKAGE_VERSION_MINOR ${CMAKE_PROJECT_VERSION_MINOR})
SET(CPACK_PACKAGE_VERSION_PATCH ${CMAKE_PROJECT_VERSION_PATCH})
SET(CPACK_PACKAGE_VERSION_TWEAK ${CMAKE_PROJECT_VERSION_TWEAK})
SET(CPACK_PACKAGE_VERSION ${VERSION_STRING})

# TODO: DESCRIPTION and WELCOME files.
##SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/doc/DESCRIPTION.txt")
#SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
#SET(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README.md")
##SET(CPACK_RESOURCE_FILE_WELCOME "${CMAKE_SOURCE_DIR}/doc/WELCOME.txt")

IF(INSTALL_DOC)
#	# Additional document files for the root directory.
#	INCLUDE(DirInstallPaths)
#	INSTALL(FILES NETWORK.md
#		DESTINATION "${DIR_INSTALL_DOC_ROOT}"
#		COMPONENT "doc")
ENDIF(INSTALL_DOC)

# CPack: Source package settings.
# NOTE: Double-escape is required because the unescaped
# string # is written to CPackSourceConfig.cmake, which
# is then unescaped.
SET(CPACK_SOURCE_GENERATOR "TGZ")
SET(CPACK_SOURCE_IGNORE_FILES
	"build.*/"
	"build.*\\\\.sh"
	"\\\\.git/"
	"\\\\.gitignore"
	"*\\\\.kate-swp"
	)

IF(CMAKE_SYSTEM_PROCESSOR MATCHES "^(i.|x)86\$")
	SET(CPACK_PACKAGE_SYSTEM_PROCESSOR "i386")
ELSEIF(CMAKE_SYSTEM_PROCESSOR MATCHES "^x86_64\$")
	SET(CPACK_PACKAGE_SYSTEM_PROCESSOR "amd64")
ELSE()
	SET(CPACK_PACKAGE_SYSTEM_PROCESSOR ${CMAKE_SYSTEM_PROCESSOR})
ENDIF()

# Components
#SET(CPACK_COMPONENTS_ALL dll program debug doc i18n)
#SET(CPACK_COMPONENT_DLL_DISPLAY_NAME "DLLs")
#SET(CPACK_COMPONENT_PROGRAM_DISPLAY_NAME "Programs")
#SET(CPACK_COMPONENT_DEBUG_DISPLAY_NAME "Debugging Symbols")
#SET(CPACK_COMPONENT_DOC_DISPLAY_NAME "Documents")
#SET(CPACK_COMPONENT_I18N_DISPLAY_NAME "Internationalization")

# Initialize CPack.
INCLUDE(CPack)
