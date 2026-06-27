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
	// TODO: Read emblems.
	QStringList sl;
	sl += QLatin1String("edit-undo");
	return sl;
}
