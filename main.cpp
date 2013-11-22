/*************
**
** Project:      Colour-space-viewer
** File info:    $Id$
** Author:       Copyright (C) 2011 Kuzma Shapran <Kuzma.Shapran@gmail.com>
** License:      GPLv3
**
**  This file is part of Colour-space-viewer.
**
**  Colour-space-viewer is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  Colour-space-viewer is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with Colour-space-viewer.  If not, see <http://www.gnu.org/licenses/>.
**
*************/

#include <QApplication>
#include "mainwindow.hpp"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;
	w.show();

	return a.exec();
}
