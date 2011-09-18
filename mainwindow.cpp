/*************
**
** Project:      __PROJECT__
** File info:    $Id$
** Author:       Copyright (C) 2010 Kuzma Shapran <Kuzma.Shapran@gmail.com>
** License:      GPLv3
**
**  This file is part of __PROJECT__.
**
**  __PROJECT__ is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  __PROJECT__ is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with __PROJECT__.  If not, see <http://www.gnu.org/licenses/>.
**
*************/

#include <QtCore/QTimer>
#include <QtGui/QGraphicsScene>
#include <QtGui/QGraphicsPixmapItem>
#include <QtGui/QGraphicsLineItem>

#include <boost/bind.hpp>

#include <iostream>
#include <cmath>

#include "mainwindow.hpp"


MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, generator_timer(new QTimer(this))
	, scene_a(new QGraphicsScene(this))
	, scene_b(new QGraphicsScene(this))
	, scene_c(new QGraphicsScene(this))
	, colour_scene(new QGraphicsScene(this))
{
	setupUi(this);

	view_a->setScene(scene_a);
	view_b->setScene(scene_b);
	view_c->setScene(scene_c);
	colour_view->setScene(colour_scene);

	pixmap_a = scene_a->addPixmap(QPixmap(256,256));
	pixmap_b = scene_b->addPixmap(QPixmap(256,256));
	pixmap_c = scene_c->addPixmap(QPixmap(256,256));

	line_a_b = scene_a->addLine(  0,  0,  0,255,QPen(Qt::black,1));
	line_a_c = scene_a->addLine(  0,  0,255,  0,QPen(Qt::black,1));
	line_b_a = scene_b->addLine(255,  0,255,255,QPen(Qt::black,1));
	line_b_c = scene_b->addLine(  0,  0,255,  0,QPen(Qt::black,1));
	line_c_a = scene_c->addLine(  0,255,255,255,QPen(Qt::black,1));
	line_c_b = scene_c->addLine(  0,  0,  0,255,QPen(Qt::black,1));

	colour_scene->setSceneRect(0,0,colour_view->width(),colour_view->height());
	colour_sample = colour_scene->addRect(0,0,colour_view->width(),colour_view->height(),Qt::NoPen);


	__to_rgba = boost::bind(&MainWindow::rgb_to_rgba,this,_1,_2,_3,_4,_5,_6,_7);

	connect(generator_timer, SIGNAL(timeout()), this, SLOT(generate_images()));

	QTimer::singleShot(0,this,SLOT(start_generating()));
}

void MainWindow::start_generating(void)
{
	if (generator_timer->isActive())
		generator_timer->stop();
	step = 0;
	generator_timer->start(0);
}

void MainWindow::generate_images()
{
	int a = step / 3;
	int i = step % 3;

	if (a == 256)
	{
		generator_timer->stop();
		horizontalSlider_a->setValue(128);
		horizontalSlider_b->setValue(128);
		horizontalSlider_c->setValue(128);
		return;
	}

	++step;

	QImage img(256,256,QImage::Format_ARGB32);
	for (int b=0; b!=256; ++b)
		for (int c=0; c!=256; ++c)
		{
			QRgb rgb = 0xff000000;
			switch (i)
			{
			case 0:
				rgb = convert_colour(a,b,c);
				break;
			case 1:
				rgb = convert_colour(255-b,a,c);
				break;
			case 2:
				rgb = convert_colour(255-c,b,a);
				break;
			}

			img.setPixel(b,c,rgb);
		}
	image[i][a] = img;

	switch (i)
	{
	case 0:
		horizontalSlider_a->setValue(a);
		break;
	case 1:
		horizontalSlider_b->setValue(a);
		break;
	case 2:
		horizontalSlider_c->setValue(a);
		break;
	}
}

void MainWindow::update_a(int value)
{
	pixmap_a->setPixmap(QPixmap::fromImage(image[0][value]));
	line_b_a->setLine(255-value,0,255-value,255);
	line_c_a->setLine(0,255-value,255,255-value);
	colour_sample->setBrush(QBrush(QColor(convert_colour(horizontalSlider_a->value(),horizontalSlider_b->value(),horizontalSlider_c->value()))));
}

void MainWindow::update_b(int value)
{
	pixmap_b->setPixmap(QPixmap::fromImage(image[1][value]));
	line_a_b->setLine(value,0,value,255);
	line_c_b->setLine(value,0,value,255);
	colour_sample->setBrush(QBrush(QColor(convert_colour(horizontalSlider_a->value(),horizontalSlider_b->value(),horizontalSlider_c->value()))));
}

void MainWindow::update_c(int value)
{
	pixmap_c->setPixmap(QPixmap::fromImage(image[2][value]));
	line_a_c->setLine(0,value,255,value);
	line_b_c->setLine(0,value,255,value);
	colour_sample->setBrush(QBrush(QColor(convert_colour(horizontalSlider_a->value(),horizontalSlider_b->value(),horizontalSlider_c->value()))));
}

QRgb MainWindow::convert_colour(int in0, int in1, int in2)
{
	double r_,g_,b_,a_;
	__to_rgba(static_cast<double>(in0)/255.,static_cast<double>(in1)/255.,static_cast<double>(in2)/255.,r_,g_,b_,a_);
	return qRgba(static_cast<int>(r_*255.),static_cast<int>(g_*255.),static_cast<int>(b_*255.),static_cast<int>(a_*255.));
}

void MainWindow::rgb_to_rgba(double _r,double _g,double _b,double &r_, double &g_, double &b_, double &a_)
{
	r_ = _r;
	g_ = _g;
	b_ = _b;
	a_ = 1.;
}

void MainWindow::hsv_to_rgba(double _h,double _s,double _v,double &r_, double &g_, double &b_, double &a_)
{
	QColor clr = QColor::fromHsvF(_h,_s,_v);
	r_ = clr.redF();
	g_ = clr.greenF();
	b_ = clr.blueF();
	a_ = 1.;
}

void MainWindow::hsl_to_rgba(double _h,double _s,double _l,double &r_, double &g_, double &b_, double &a_)
{
	QColor clr = QColor::fromHslF(_h,_s,_l);
	r_ = clr.redF();
	g_ = clr.greenF();
	b_ = clr.blueF();
	a_ = 1.;
}

void MainWindow::ycbcr_to_rgba(double _y,double _cb,double _cr,double &r_, double &g_, double &b_, double &a_)
{
	r_ = _y                                 + 0.701            *(_cr*2.-1.);
	g_ = _y - 0.886*0.114/0.587*(_cb*2.-1.) - 0.701*0.299/0.587*(_cr*2.-1.);
	b_ = _y + 0.886            *(_cb*2.-1.);
	a_ = ((r_<0.) || (r_>1.) || (g_<0.) || (g_>1.) || (b_<0.) || (b_>1.)) ? 0.125 : 1.0;
}

void MainWindow::rgb_to_hcy(double _r,double _g,double _b,double &h_, double &c_, double &y_)
{
	double r_weight = 0.299;
	double g_weight = 0.587;
	double b_weight = 0.114;

	double M = qMax(qMax(_r,_g),_b);
	double m = qMin(qMin(_r,_g),_b);

	y_ = (r_weight * _r + g_weight * _g + b_weight * _b);

	int H_sec = 0;
	double H_insec = 0.;
	if (M != m)
	{
		double X;
		if (M == _r)
		{
			if (m == _g)
			{
				H_sec = 5;
				c_ = _r - m;
				X  = _b - m;
				H_insec = 1. - X/c_;
			}
			else
			{
				H_sec = 0;
				c_ = _r - m;
				X  = _g - m;
				H_insec = X/c_;
			}
		}
		else if (M == _g)
		{
			if (m == _b)
			{
				H_sec = 1;
				c_ = _g - m;
				X  = _r - m;
				H_insec = 1. - X/c_;
			}
			else
			{
				H_sec = 2;
				c_ = _g - m;
				X  = _b - m;
				H_insec = X/c_;
			}
		}
		else
		{
			if (m == _r)
			{
				H_sec = 3;
				c_ = _b - m;
				X  = _g - m;
				H_insec = 1. - X/c_;
			}
			else
			{
				H_sec = 4;
				c_ = _b - m;
				X  = _r - m;
				H_insec = X/c_;
			}
		}
	}

	double Y_peak = 0.; // shut up the compiler
	switch (H_sec)
	{
	case 0:
		Y_peak =     r_weight  + H_insec * ((1.-b_weight) -     r_weight );
		break;
	case 1:
		Y_peak = (1.-b_weight) + H_insec * (    g_weight  - (1.-b_weight));
		break;
	case 2:
		Y_peak =     g_weight  + H_insec * ((1.-r_weight) -     g_weight );
		break;
	case 3:
		Y_peak = (1.-r_weight) + H_insec * (    b_weight  - (1.-r_weight));
		break;
	case 4:
		Y_peak =     b_weight  + H_insec * ((1.-g_weight) -     b_weight );
		break;
	case 5:
		Y_peak = (1.-g_weight) + H_insec * (    r_weight  - (1.-g_weight));
		break;
	}
	if ((y_ != 0.) && (y_ != 1.))
	{
		if (y_ < Y_peak)
			c_ /= y_/Y_peak;
		else
			c_ /=(1. - y_)/(1. - Y_peak);
	}


	h_ = (H_sec + H_insec) / 6.;
}

void MainWindow::hcy_to_rgba(double _h, double _c, double _y,double &r_,double &g_,double &b_, double &a_)
{
	double r_weight = 0.299;
	double g_weight = 0.587;
	double b_weight = 0.114;

	if (_h >= 1.0)
		_h -= static_cast<int>(_h);

	_h *= 6.;
	int H_sec = static_cast<int>(_h);
	int H1 = H_sec/2;
	H1*=2;
	double H2 = _h - static_cast<double>(H1);


	double Y_peak=0.; // shut up the compiler
	double H_insec = _h - H_sec;
	switch (H_sec)
	{
	case 0:
		Y_peak =     r_weight  + H_insec * ((1.-b_weight) -     r_weight );
		break;
	case 1:
		Y_peak = (1.-b_weight) + H_insec * (    g_weight  - (1.-b_weight));
		break;
	case 2:
		Y_peak =     g_weight  + H_insec * ((1.-r_weight) -     g_weight );
		break;
	case 3:
		Y_peak = (1.-r_weight) + H_insec * (    b_weight  - (1.-r_weight));
		break;
	case 4:
		Y_peak =     b_weight  + H_insec * ((1.-g_weight) -     b_weight );
		break;
	case 5:
		Y_peak = (1.-g_weight) + H_insec * (    r_weight  - (1.-g_weight));
		break;
	}
	if (_y < Y_peak)
		_c *= _y/Y_peak;
	else
		_c *=(1. - _y)/(1. - Y_peak);


	double X = _c * (1. - fabs(H2 - 1.));

	r_ = 0.;
	g_ = 0.;
	b_ = 0.;
	switch (H_sec)
	{
	case 0:
		r_ = _c; g_ = X;  /*    */
		break;
	case 1:
		r_ = X;  g_ = _c; /*    */
		break;
	case 2:
		/*    */ g_ = _c; b_ = X;
		break;
	case 3:
		/*    */ g_ = X;  b_ = _c;
		break;
	case 4:
		r_ = X;  /*    */ b_ = _c;
		break;
	case 5:
		r_ = _c; /*    */ b_ = X;
		break;
	}

	double m = _y - (r_weight * r_ + g_weight * g_ + b_weight * b_);

	r_ += m;
	g_ += m;
	b_ += m;
	a_ = 1.;
}

//void MainWindow::hcy_to_rgba2(double _h, double _c, double _y,double &r_,double &g_,double &b_, double &a_)
//{
//	hcy_to_rgba(_h,_c,_y,r_,g_,b_,a_);
//	rgb_to_hcy(r_,g_,b_,_h,_c,_y);
//	hcy_to_rgba(_h,_c,_y,r_,g_,b_,a_);
//}

void MainWindow::set_colourspace_rgb(void)
{
	if (radioButton_rgb->isChecked())
	{
		__to_rgba = boost::bind(&MainWindow::rgb_to_rgba,this,_1,_2,_3,_4,_5,_6,_7);
		label_title_a->setText("R");
		label_title_b->setText("G");
		label_title_c->setText("B");
		start_generating();
	}
}

void MainWindow::set_colourspace_hsv(void)
{
	if (radioButton_hsv->isChecked())
	{
		__to_rgba = boost::bind(&MainWindow::hsv_to_rgba,this,_1,_2,_3,_4,_5,_6,_7);
		label_title_a->setText("H");
		label_title_b->setText("S");
		label_title_c->setText("V");
		start_generating();
	}
}

void MainWindow::set_colourspace_hsl(void)
{
	if (radioButton_hsl->isChecked())
	{
		__to_rgba = boost::bind(&MainWindow::hsl_to_rgba,this,_1,_2,_3,_4,_5,_6,_7);
		label_title_a->setText("H");
		label_title_b->setText("S");
		label_title_c->setText("L");
		start_generating();
	}
}

void MainWindow::set_colourspace_ycbcr(void)
{
	if (radioButton_ycbcr->isChecked())
	{
		__to_rgba = boost::bind(&MainWindow::ycbcr_to_rgba,this,_1,_2,_3,_4,_5,_6,_7);
		label_title_a->setText("Y");
		label_title_b->setText("Cb");
		label_title_c->setText("Cr");
		start_generating();
	}
}

void MainWindow::set_colourspace_hcy(void)
{
	if (radioButton_hcy->isChecked())
	{
		__to_rgba = boost::bind(&MainWindow::hcy_to_rgba,this,_1,_2,_3,_4,_5,_6,_7);
		label_title_a->setText("H");
		label_title_b->setText("C");
		label_title_c->setText("Y");
		start_generating();
	}
}
