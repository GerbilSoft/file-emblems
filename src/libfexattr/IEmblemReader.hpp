/***************************************************************************
 * File Emblems (libfexattr)                                               *
 * IEmblemReader.hpp: Emblem reader interface                              *
 *                                                                         *
 * Copyright (c) 2026 by David Korth.                                      *
 * SPDX-License-Identifier: GPL-2.0-or-later                               *
 ***************************************************************************/

#pragma once

// C++ STL classes
#include <string>
#include <vector>

namespace LibFeXAttr {

class IEmblemReader
{
protected:
	IEmblemReader()
		: m_writable(false)
	{}
	IEmblemReader(bool writable)
		: m_writable(writable)
	{}
public:
	~IEmblemReader() = default;

public:
	/**
	 * Get the filename.
	 * @return Filename, or nullptr on error.
	 */
	virtual const char *filename(void) const = 0;

	/**
	 * Is the file open?
	 * @return True if the file is open; false if not.
	 */
	virtual bool isOpen(void) const = 0;

	/**
	 * Close the file if it's open.
	 */
	virtual void close(void) = 0;

	/**
	 * Is the file writable?
	 * @return True if writable; false if not.
	 */
	bool isWritable(void) const
	{
		return m_writable;
	}

	/**
	 * Does this file have an emblems xattr?
	 * @return True if it does; false if it doesn't.
	 */
	virtual bool hasEmblems(void) = 0;

	/**
	 * Get emblems from the opened file.
	 * @return Vector of emblems.
	 */
	virtual std::vector<std::string> emblems(void) = 0;

protected:
	/**
	 * Check for a delimiter character.
	 * @param c Character
	 * @return True if whitespace; false if not.
	 */
	static constexpr bool is_whitespace(char c)
	{
		return (c == ' ' || c == '\n' || c == '\r' || c == '\t' || c == ',' || c == ':' || c == ';');
	}

protected:
	bool m_writable;
};

} // namespace LibFeXAttr
