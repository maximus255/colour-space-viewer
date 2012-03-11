/*************
**
** Project:      Colour-space-viewer
** File info:    $Id$
** Author:       Copyright (C) 2011-2012 Kuzma Shapran <Kuzma.Shapran@gmail.com>
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

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "ui_mainwindow.h"

#include <boost/function.hpp>

class QTimer;
class QGraphicsScene;
class QGraphicsPixmapItem;
class QGraphicsLineItem;
class QGraphicsRectItem;

class MainWindow : public QMainWindow, private Ui::MainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = NULL);

public slots:
	void first_frame(void);
	void start_generating(void);
	void generate_images(void);

	void set_colourspace_rgb(void);
	void set_colourspace_hsv(void);
	void set_colourspace_hsl(void);
	void set_colourspace_ycbcr(void);
	void set_colourspace_hcy(void);

	void update_a(int);
	void update_b(int);
	void update_c(int);

	void update_ab(const QPoint&);
	void update_bc(const QPoint&);
	void update_ac(const QPoint&);

private:
	QImage image[3][256];

	int step;
	QTimer *generator_timer;

	QGraphicsScene *scene_a;
	QGraphicsPixmapItem *pixmap_a;
	QGraphicsLineItem *line_a_b;
	QGraphicsLineItem *line_a_c;

	QGraphicsScene *scene_b;
	QGraphicsPixmapItem *pixmap_b;
	QGraphicsLineItem *line_b_a;
	QGraphicsLineItem *line_b_c;

	QGraphicsScene *scene_c;
	QGraphicsPixmapItem *pixmap_c;
	QGraphicsLineItem *line_c_a;
	QGraphicsLineItem *line_c_b;

	QGraphicsScene *colour_scene;
	QGraphicsRectItem *colour_sample;

	QRgb convert_colour(int in0, int in1, int in2);

	boost::function<void (double in0, double in1, double in2,double &r_, double &g_, double &b_, double &a_)> __to_rgba;
	boost::function<void (double _r,double _g,double _b,double &out0, double &out1, double &out2)> __from_rgb;

	void rgb_to_rgba(double _r,double _g,double _b,double &r_, double &g_, double &b_, double &a_);

	void hsv_to_rgba(double _h,double _s,double _v,double &r_, double &g_, double &b_, double &a_);

	void hsl_to_rgba(double _h,double _s,double _l,double &r_, double &g_, double &b_, double &a_);

	void ycbcr_to_rgba(double _y,double _cb,double _cr,double &r_, double &g_, double &b_, double &a_);

	void rgb_to_hcy(double _r,double _g,double _b,double &h_, double &c_, double &y_);
	void hcy_to_rgba(double _h, double _c, double _y,double &r_,double &g_,double &b_,double &a_);
//	void hcy_to_rgba2(double _h, double _c, double _y,double &r_,double &g_,double &b_,double &a_);
};

#endif // MAINWINDOW_HPP
