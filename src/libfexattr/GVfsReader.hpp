/***************************************************************************
 * File Emblems (libfexattr)                                               *
 * GVfsReader.hpp: GVfs metadata reader                                    *
 *                                                                         *
 * Copyright (c) 2026 by David Korth.                                      *
 * SPDX-License-Identifier: GPL-2.0-or-later                               *
 ***************************************************************************/

#pragma once

#include "IEmblemReader.hpp"

#include <glib.h>
#include <gio/gio.h>

namespace LibFeXAttr {

class GVfsReader : public IEmblemReader
{
public:
	/**
	 * Create an GVfsReader.
	 * @param filename Filename
	 * @param writable If true, attempt to open for writing.
	 */
	GVfsReader(const char *filename, bool writable = false);
public:
	~GVfsReader();

public:
	/**
	 * Get the filename.
	 * @return Filename, or nullptr on error.
	 */
	const char *filename(void) const final
	{
		return (!m_filename.empty()) ? m_filename.c_str() : nullptr;
	}

	/**
	 * Is the file open?
	 * @return True if the file is open; false if not.
	 */
	bool isOpen(void) const final
	{
		return (m_file != nullptr);
	}

	/**
	 * Close the file if it's open.
	 */
	void close(void) final;

	/**
	 * Does this file have an emblems xattr?
	 * @return True if it does; false if it doesn't.
	 */
	bool hasEmblems(void) final;

	/**
	 * Get emblems from the opened file.
	 * @return Vector of emblems.
	 */
	std::vector<std::string> emblems(void) final;

private:
	GFile *m_file;
	std::string m_filename;
};

} // namespace LibFeXAttr
