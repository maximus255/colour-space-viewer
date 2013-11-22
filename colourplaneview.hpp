/*************
**
** Project:      Colour-space-viewer
** File info:    $Id$
** Author:       Copyright (C) 2012 Kuzma Shapran <Kuzma.Shapran@gmail.com>
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


#ifndef COLOURPLANEVIEW_HPP
#define COLOURPLANEVIEW_HPP

#include <QGraphicsView>

class ColourPlaneView : public QGraphicsView
{
	Q_OBJECT
public:
	explicit ColourPlaneView(QWidget *parent = NULL);

protected:
	virtual void mousePressEvent(QMouseEvent*);
	virtual void mouseMoveEvent(QMouseEvent*);

signals:
	void mouseDragged(const QPoint&);

public slots:

private:

};

#endif // COLOURPLANEVIEW_HPP
