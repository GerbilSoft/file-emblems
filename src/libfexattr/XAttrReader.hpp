/***************************************************************************
 * File Emblems (libfexattr)                                               *
 * XAttrReader.hpp: Extended Attribute reader                              *
 *                                                                         *
 * Copyright (c) 2026 by David Korth.                                      *
 * SPDX-License-Identifier: GPL-2.0-or-later                               *
 ***************************************************************************/

#pragma once

// C++ STL classes
#include <string>
#include <vector>

namespace LibFeXAttr {

class XAttrReader
{
public:
	/**
	 * Create an XAttrReader.
	 * @param filename Filename
	 * @param writable If true, attempt to open for writing.
	 */
	XAttrReader(const char *filename, bool writable = false);

	~XAttrReader();

public:
	/**
	 * Get the filename.
	 * @return Filename, or nullptr on error.
	 */
	const char *filename(void) const
	{
		return (!m_filename.empty()) ? m_filename.c_str() : nullptr;
	}

	/**
	 * Is the file open?
	 * @return True if the file is open; false if not.
	 */
	bool isOpen(void) const
	{
		return (m_fd >= 0);
	}

	/**
	 * Is the file writable?
	 * @return True if writable; false if not.
	 */
	bool isWritable(void) const
	{
		return m_writable;
	}

	/**
	 * Get emblems from the opened file.
	 * @return Vector of emblems.
	 */
	std::vector<std::string> emblems(void);

private:
	std::string m_filename;
	int m_fd;
	bool m_writable;
};

} // namespace LibFeXAttr
