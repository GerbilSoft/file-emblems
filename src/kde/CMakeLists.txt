# File Emblems for KDE
PROJECT(file-emblems-kde LANGUAGES CXX)
SET(CMAKE_AUTOMOC ON)

# Common files for all KDE versions.

# Sources and headers
SET(${PROJECT_NAME}_SRCS
	OverlayIconPlugin.cpp
	)
SET(${PROJECT_NAME}_H
	)

# Qt options:
# - Fast QString concatenation. (Qt 4.6+, plus 4.8-specific version)
# - Disable implicit QString ASCII casts.
# - Use strict iterators.
# - Disable implicit cast from QString to QUrl.
# - Use lean headers. (Qt6)
ADD_DEFINITIONS(-DQT_USE_FAST_CONCATENATION
	-DQT_USE_FAST_OPERATOR_PLUS
	-DQT_USE_QSTRINGBUILDER
	-DQT_NO_CAST_FROM_ASCII
	-DQT_NO_CAST_TO_ASCII
	-DQT_STRICT_ITERATORS
	-DQT_NO_URL_CAST_FROM_STRING
	-DQT_NO_CAST_FROM_BYTEARRAY
	-DQT_NO_FOREACH
	-DQT_NO_NARROWING_CONVERSIONS_IN_CONNECT
	-DQT_EXPLICIT_QFILE_CONSTRUCTION_FROM_PATH
	-DQT_NO_NARROWING_CONVERSIONS_IN_CONNECT

	# Qt6
	-DQT_LEAN_HEADERS=1
	-DQT_NO_CONTEXTLESS_CONNECT
	)

# Define -DQT_NO_DEBUG in release builds.
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DQT_NO_DEBUG")

# Only KF6 is supported right now.
# TODO: KF5 build maybe?
ADD_SUBDIRECTORY(kf6)
