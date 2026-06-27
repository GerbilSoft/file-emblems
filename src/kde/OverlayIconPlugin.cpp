/***************************************************************************
 * File Emblems (KDE)                                                      *
 * OverlayIconPlugin.cpp: KOverlayIconPlugin                               *
 *                                                                         *
 * Copyright (c) 2026 by David Korth.                                      *
 * SPDX-License-Identifier: GPL-2.0-or-later                               *
 ***************************************************************************/

#include <QtCore/qglobal.h>
#if QT_VERSION >= QT_VERSION_CHECK(7, 0, 0)
#  error Update for new Qt!
#elif QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#  include "OverlayIconPluginKF6.hpp"
#elif QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
#  include "OverlayIconPluginKF5.hpp"
#else
#  error Qt is too old!
#endif

// TODO: Simplify this by making a wrapper class that checks xattr and GVfs for us.
#include "config.libfexattr.h"

#include "libfexattr/XAttrReader.hpp"
using LibFeXAttr::XAttrReader;

#ifdef ENABLE_GVFS
#  include "libfexattr/GVfsReader.hpp"
using LibFeXAttr::GVfsReader;
#endif /* ENABLE_GVFS */

#include "FeQUrl.hpp"

// C++ STL classes
#include <string>
#include <vector>
using std::string;
using std::vector;
#include <stdio.h>

OverlayIconPlugin::OverlayIconPlugin(QObject *parent)
	: super(parent)
{}

static vector<string> getEmblems(const char *filename)
{
	// Check attributes.
	// TODO: Simplify this by making a wrapper class that checks both xattr and GVfs for us.
	// If the file has:
	// - no attributes at all: nothing.
	// - GVfs, but not XAttr: GVfs [and eventually, copy over to XAttr if possible]
	// - XAttr, but not GVfs: XAttr
	// - Both GVfs and XAttr: XAttr
	vector<string> emblems;

	// Do we have XAttr?
	XAttrReader xattrReader(filename);
	if (xattrReader.isOpen() && xattrReader.hasEmblems()) {
		// We have XAttr.
		emblems = xattrReader.emblems();
		if (!emblems.empty()) {
			// We actually *have* emblems in the XAttr.
			return emblems;
		}
		xattrReader.close();
	}

	// No XAttr. Check GVfs.
	GVfsReader gvfsReader(filename);
	if (gvfsReader.isOpen() && gvfsReader.hasEmblems()) {
		// We have GVfs.
		emblems = gvfsReader.emblems();
		if (!emblems.empty()) {
			// We actually *have* embelms in GVfs.
			return emblems;
		}
		gvfsReader.close();
	}

	// No emblems...
	return emblems;
}

QStringList OverlayIconPlugin::getOverlays(const QUrl &item)
{
	if (item.isEmpty()) {
		// Empty URL. Nothing to do here.
		return {};
	}

	// Need to localize the URL in order to open it with xattr functions.
	const QUrl localUrl = localizeQUrl(item);
	if (localUrl.isEmpty()) {
		// Unable to localize the URL.
		return {};
	}

	string s_local_filename;

	if (localUrl.scheme().isEmpty()) {
		s_local_filename = localUrl.path().toStdString();
	} else if (localUrl.isLocalFile()) {
		s_local_filename = localUrl.toLocalFile().toStdString();
	}

	
	// Get the emblems.
	vector<string> emblems = getEmblems(s_local_filename.c_str());

	// Convert from vector<string> to QStringList.
	QStringList sl;
	for (const string &str : emblems) {
		sl.append(QString::fromUtf8(str));
	}
	return sl;
}
