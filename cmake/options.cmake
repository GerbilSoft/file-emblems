# Build options.

OPTION(ENABLE_WERROR "Treat all compile warnings as errors. (Enable for development!)" OFF)

# Platform options.
# NOTE: If a platform is specified but it isn't found,
# that plugin will not be built. There doesn't seem to
# be a way to have "yes, no, auto" like in autotools.

# NOTE: OPTION() only supports BOOL values.
# Reference: https://cmake.org/pipermail/cmake/2016-October/064342.html
MACRO(OPTION_UI _pkg _desc)
	SET(BUILD_${_pkg} AUTO CACHE STRING "${_desc}")
	SET_PROPERTY(CACHE BUILD_${_pkg} PROPERTY STRINGS AUTO ON OFF)

	IF(BUILD_${_pkg} STREQUAL "AUTO")
		SET(REQUIRE_${_pkg} "")
	ELSEIF(BUILD_${_pkg})
		SET(REQUIRE_${_pkg} "REQUIRED")
	ELSE()
		SET(REQUIRE_${_pkg} "")
	ENDIF()
ENDMACRO(OPTION_UI)

# OPTION_UI but for ENABLE_
MACRO(OPTION_UI_ENABLE _pkg _desc)
	SET(ENABLE_${_pkg} AUTO CACHE STRING "${_desc}")
	SET_PROPERTY(CACHE ENABLE_${_pkg} PROPERTY STRINGS AUTO ON OFF)

	IF(ENABLE_${_pkg} STREQUAL "AUTO")
		SET(REQUIRE_${_pkg} "")
	ELSEIF(ENABLE_${_pkg})
		SET(REQUIRE_${_pkg} "REQUIRED")
	ELSE()
		SET(REQUIRE_${_pkg} "")
	ENDIF()
ENDMACRO(OPTION_UI_ENABLE)

IF(UNIX AND NOT APPLE)
	# NOTE: OPTION() only supports BOOL values.
	# Reference: https://cmake.org/pipermail/cmake/2016-October/064342.html
	#OPTION_UI(KF5 "Build the KDE Frameworks 5 plugin.")
	OPTION_UI(KF6 "Build the KDE Frameworks 6 plugin.")

	# QT_SELECT must be unset before compiling.
	UNSET(ENV{QT_SELECT})
ELSE()
	MESSAGE(FATAL_ERROR "File Emblems currently only supports Linux platforms.")
ENDIF()

# GVfs integration (requires glib-2.0)
OPTION_UI_ENABLE(GVFS "Enable GVfs integration to read emblems from Nautilus and Thunar.")

# Link-time optimization
# FIXME: Not working in clang builds and Ubuntu's gcc...
IF(MSVC)
	SET(LTO_DEFAULT ON)
ELSE()
	SET(LTO_DEFAULT OFF)
ENDIF()
OPTION(ENABLE_LTO "Enable link-time optimization in release builds." ${LTO_DEFAULT})

# Split debug information into a separate file
# FIXME: macOS `strip` shows an error:
# error: symbols referenced by indirect symbol table entries that can't be stripped in: [library]
# NOTE: Disabled on Emscripten because it's JavaScript/WebAssembly.
IF(NOT EMSCRIPTEN)
IF(APPLE)
	OPTION(SPLIT_DEBUG "Split debug information into a separate file." OFF)
ELSE(APPLE)
	OPTION(SPLIT_DEBUG "Split debug information into a separate file." ON)
ENDIF(APPLE)

# Install the split debug file
OPTION(INSTALL_DEBUG "Install the split debug files." ON)
IF(INSTALL_DEBUG AND NOT SPLIT_DEBUG)
	# Cannot install debug files if we're not splitting them.
	SET(INSTALL_DEBUG OFF CACHE INTERNAL "Install the split debug files." FORCE)
ENDIF(INSTALL_DEBUG AND NOT SPLIT_DEBUG)
ENDIF(NOT EMSCRIPTEN)

# Enable NLS (internationalization)
#STRING(TOLOWER "${_MSVC_C_ARCHITECTURE_FAMILY}" _msvc_c_arch_lower)
#IF(NOT WIN32 OR NOT MSVC)
#	OPTION(ENABLE_NLS "Enable NLS using gettext for localized messages." ON)
#ELSEIF(MSVC AND _msvc_c_arch_lower MATCHES "^(i?[x3]86)|x64|arm.*|aarch.*$")
#	OPTION(ENABLE_NLS "Enable NLS using gettext for localized messages." ON)
#ELSE()
#	SET(ENABLE_NLS OFF CACHE INTERNAL "Enable NLS using gettext for localized messages." FORCE)
#ENDIF()
#UNSET(_msvc_c_arch_lower)

# Install documentation
OPTION(INSTALL_DOC "Install documentation." ON)
