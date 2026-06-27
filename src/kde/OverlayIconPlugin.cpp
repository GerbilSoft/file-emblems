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

#include "libfexattr/XAttrReader.hpp"
using LibFeXAttr::XAttrReader;

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

	// Open the file and get the emblem list.
	XAttrReader xattrReader(s_local_filename.c_str());
	if (!xattrReader.isOpen()) {
		// Unable to open the file...
		return {};
	}

	// Get the emblems.
	vector<string> emblems = xattrReader.emblems();

	// Convert from vector<string> to QStringList.
	QStringList sl;
	for (const string &str : emblems) {
		sl.append(QString::fromUtf8(str));
	}
	return sl;
}
