/***************************************************************************
 * File Emblems (KF6)                                                      *
 * OverlayIconPluginKF6.hpp: KOverlayIconPlugin                            *
 *                                                                         *
 * Copyright (c) 2026 by David Korth.                                      *
 * SPDX-License-Identifier: GPL-2.0-or-later                               *
 ***************************************************************************/

#pragma once

#include <QtCore/qglobal.h>
#include <KOverlayIconPlugin>

class OverlayIconPlugin : public ::KOverlayIconPlugin
{
	Q_OBJECT
	//Q_INTERFACES(KOverlayIconPlugin)

	// NOTE: KF6 doesn't have a standard IID for KOverlayIconPlugin...
	Q_PLUGIN_METADATA(IID "com.gerbilsoft.file-emblems.KOverlayIconPlugin" FILE "OverlayIconPlugin.json")

public:
	explicit OverlayIconPlugin(QObject *parent = nullptr);

private:
	typedef KOverlayIconPlugin super;
	Q_DISABLE_COPY(OverlayIconPlugin);

public:
	QStringList getOverlays(const QUrl &item) final;
};
