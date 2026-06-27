/***************************************************************************
 * File Emblems (libfexattr)                                               *
 * XAttrReader.cpp: Extended Attribute reader                              *
 *                                                                         *
 * Copyright (c) 2026 by David Korth.                                      *
 * SPDX-License-Identifier: GPL-2.0-or-later                               *
***************************************************************************/

/**
 * Extended Attribute format:
 * Emblems are stored in the "user.emblem" xattr.
 *
 * The original kdelibs 3.5 hack only supported a single emblem.
 * This implementation supports multiple emblems:
 * - When writing: Emblems are delimited by spaces.
 * - When reading: Any whitespace, comma, colon, or semicolon are considered delimiters.
 */

#include "config.libfexattr.h"
#include "XAttrReader.hpp"

#if defined(HAVE_SYS_XATTR_H)
// NOTE: Mac fsetxattr() is the same as Linux but with an extra options parameter.
#  include <sys/xattr.h>
#elif defined(HAVE_SYS_EXTATTR_H)
#  include <sys/types.h>
#  include <sys/extattr.h>
#endif

// C includes
#include <fcntl.h>
#include <unistd.h>

// C++ STL classes
using std::string;
using std::vector;

// Uninitialized vector class
#include "uvector.h"

namespace LibFeXAttr {

/**
 * Create an XAttrReader.
 * @param filename Filename
 * @param writable If true, attempt to open for writing.
 */
XAttrReader::XAttrReader(const char *filename, bool writable)
	: m_filename(filename)
	, m_writable(false)
{
	if (writable) {
		// Try opening for writing.
		// If it fails, we'll open for reading only.
		m_fd = open(filename, O_RDWR | O_SYNC);
		if (m_fd >= 0) {
			m_writable = true;
			return;
		}
	}

	// Open the file for reading.
	m_fd = open(filename, O_RDONLY | O_SYNC);
}

XAttrReader::~XAttrReader()
{
	if (m_fd >= 0) {
		::close(m_fd);
	}
}

/**
 * Close the file if it's open.
 */
void XAttrReader::close(void)
{
	if (m_fd >= 0) {
		::close(m_fd);
		m_fd = -1;
	}
}

/**
 * Check for a delimiter character..
 * @param c Character
 * @return True if whitespace; false if not.
 */
static constexpr bool is_whitespace(char c)
{
	return (c == ' ' || c == '\n' || c == '\r' || c == '\t' || c == ',' || c == ':' || c == ';');
}

/**
 * Get emblems from the opened file.
 * @return Vector of emblems.
 */
vector<string> XAttrReader::emblems(void)
{
	if (!isOpen()) {
		// File isn't open.
		return {};
	}

	// Value buffer
	fe::uvector<char> value;
	value.reserve(128);

	// Attempt to load the "user.emblem" xattr.
	ssize_t valuelen = 0;
	do {
		// Get the value for this attribute.
		// NOTE: valuelen does *not* include a NULL-terminator.
		value.resize(valuelen);
#if defined(HAVE_SYS_XATTR_H) && !defined(__stub_getxattr) && !defined(__APPLE__)
		valuelen = fgetxattr(m_fd, "user.emblem", value.data(), valuelen);
#elif defined(__APPLE__)
		valuelen = fgetxattr(fd, "user.emblem", value.data(), valuelen, 0, 0);
#elif defined(HAVE_SYS_EXTATTR_H)
		valuelen = extattr_get_fd(fd, EXTATTR_NAMESPACE_USER, "emblem", value.data(), valuelen);
#endif

		if (valuelen > 0 && value.size() == 0) {
			continue;
		} else if (valuelen > 0 && value.size() > 0) {
			break;
		} else if (valuelen == -1) {
			// Error reading the attribute.
			return {};
		} else if (valuelen == 0) {
			// attr value is an empty string
			break;
		}
	} while (true);

	if (value.empty()) {
		// No emblems.
		return {};
	}

	// Emblems
	vector<string> emblems;

	// Split the xattr value on whitespace characters.
	const char *p = value.data();
	const char *p_end = value.data() + value.size();
	while (p < p_end) {
		// Find a whitespace character.
		const char *pn = p;
		for (; pn < p_end; pn++) {
			if (is_whitespace(*pn)) {
				break;
			} else if (*pn == '\0') {
				// NULL terminator.
				// TODO: Handle as whitespace?
				p_end = pn;
				break;
			}
		}

		if (p == pn) {
			// Still at the start of the string?
			p++;
			continue;
		} else if (p + 1 == pn) {
			// Delimiter is directly after the current character.
			// Empty string; ignore it.
			p += 2;
			continue;
		} else if (pn >= p_end) {
			// Past the end of the xattr.
			// Take the rest of the xattr as the last string.
			emblems.emplace_back(p, p_end - p);
			break;
		}

		// [p, pn) is the next string.
		emblems.emplace_back(p, pn - p);
		p = pn + 1;
	}

	return emblems;
}

} // namespace LibFeXAttr
