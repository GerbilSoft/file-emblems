/***************************************************************************
 * File Emblems (libfexattr)                                               *
 * GVfsReader.cpp: Extended Attribute reader                              *
 *                                                                         *
 * Copyright (c) 2026 by David Korth.                                      *
 * SPDX-License-Identifier: GPL-2.0-or-later                               *
***************************************************************************/

/**
 * GVfs emblem format:
 * Emblems are stored in the "metadata::emblems" attribute as a vector of strings.
 */

#include "GVfsReader.hpp"

// C++ STL classes
using std::string;
using std::vector;

namespace LibFeXAttr {

/**
 * Create an GVfsReader.
 * @param filename Filename
 * @param writable If true, attempt to open for writing.
 */
GVfsReader::GVfsReader(const char *filename, bool writable)
	: m_filename(filename)
{
	// NOTE: GVfs metadata is always writable.
	m_writable = writable;

	// Open the file.
	m_file = g_file_new_for_path(filename);
}

GVfsReader::~GVfsReader()
{
	if (m_file) {
		g_object_unref(m_file);
	}
}

/**
 * Close the file if it's open.
 */
void GVfsReader::close(void)
{
	if (m_file) {
		// TODO: g_clear_object()?
		g_object_unref(m_file);
		m_file = nullptr;
	}
}

/**
 * Does this file have an emblems xattr?
 * @return True if it does; false if it doesn't.
 */
bool GVfsReader::hasEmblems(void)
{
	if (!isOpen()) {
		// File isn't open.
		return false;
	}


	GFileInfo *const fileInfo = g_file_query_info(m_file, "metadata::emblems", G_FILE_QUERY_INFO_NONE, nullptr, nullptr);
	if (!fileInfo) {
		return false;
	}

	// Did we actually get the "metadata::emblems" attribute?
	if (!g_file_info_has_attribute(fileInfo, "metadata::emblems")) {
		// No attribute.
		return false;
	}

	// Attribute type should be stringv.
	// (We'll accept string as well, though other programs might not...)
	GFileAttributeType type = g_file_info_get_attribute_type(fileInfo, "metadata::emblems");
	return (type == G_FILE_ATTRIBUTE_TYPE_STRINGV || type == G_FILE_ATTRIBUTE_TYPE_STRING);
}

/**
 * Get emblems from the opened file.
 * @return Vector of emblems.
 */
vector<string> GVfsReader::emblems(void)
{
	if (!isOpen()) {
		// File isn't open.
		return {};
	}

	GFileInfo *const fileInfo = g_file_query_info(m_file, "metadata::emblems", G_FILE_QUERY_INFO_NONE, nullptr, nullptr);
	if (!fileInfo) {
		return {};
	}

	// Did we actually get the "metadata::emblems" attribute?
	if (!g_file_info_has_attribute(fileInfo, "metadata::emblems")) {
		// No emblems.
		return {};
	}

	// Attribute type should be stringv.
	// (We'll accept string as well, though other programs might not...)
	GFileAttributeType type = g_file_info_get_attribute_type(fileInfo, "metadata::emblems");
	if (type != G_FILE_ATTRIBUTE_TYPE_STRINGV && type != G_FILE_ATTRIBUTE_TYPE_STRING) {
		// Not the right type.
		return {};
	}

	// Emblems
	vector<string> emblems;

	if (type == G_FILE_ATTRIBUTE_TYPE_STRING) {
		// Single string
		const char *str = g_file_info_get_attribute_string(fileInfo, "metadata::emblems");
		if (str) {
			emblems.push_back(str);
		}
		return emblems;
	}

	// Multiple strings
	const char *const *strv = g_file_info_get_attribute_stringv(fileInfo, "metadata::emblems");
	if (!strv) {
		// ...or no strings?
		return emblems;
	}

	// strv is owned by GFileInfo, so iterate, but don't free.
	for (; *strv; strv++) {
		emblems.push_back(*strv);
	}

	return emblems;
}

} // namespace LibFeXAttr
