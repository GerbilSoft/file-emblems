PROJECT(src)

# Set CMake hidden visibility options.
SET(CMAKE_C_VISIBILITY_PRESET "hidden")
SET(CMAKE_CXX_VISIBILITY_PRESET "hidden")
SET(CMAKE_VISIBILITY_INLINES_HIDDEN ON)

# Write the version number to config.version.h.
CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/config.version.h.in" "${CMAKE_CURRENT_BINARY_DIR}/config.version.h")

# Source Code subdirectories
ADD_SUBDIRECTORY(kde)
