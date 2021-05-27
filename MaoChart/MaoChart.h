#pragma once

#include <QtWidgets/QWidget>
#include <vector>
#include <qlist.h>
#include "ui_MaoChart.h"
#include<qtimer.h>
#include<qlabel.h>

#define PADDING 20.0;

class MaoChart : public QWidget
{
    Q_OBJECT

public:
    MaoChart(QWidget *parent = Q_NULLPTR);
	~MaoChart();
	void setTitle(QString name);
	void addLine(QList<QPointF> list);
	void setAdaptivePoint(double span);
	void setAdaptiveLine(double span);
	void set_x_pointDensity(int x);
	void set_y_pointDensity(int x);
	void set_padding(double padding);
	void addPoint(QPointF point);
	void drawPoints();
	double Interpolation(double x1, double y1, double x2, double y2, double value_x);//≤Â÷µ
	double X_axis_mapping(double x_value);	//x÷·ø’º‰”≥…‰
	double Y_axis_mapping(double y_value);	//y÷·ø’º‰”≥…‰
	void update();
private:
    Ui::MaoChartClass ui;
	QString Title;
	QList<QPointF> *pointList;
	QList<QPointF> *pointList;
	QList<QList<QPointF>> LineList;
	QList<QLabel*> LableList_point;
	QList<QLabel> LableList_line;
	QTimer *timer;
	double window_width = 0;
	double window_height = 0;
	double padding = 55;
	double x_upperRange;
	double x_lowerRange;
	double y_upperRange;
	double y_lowerRange;
	double pointAdaptiveSpan;
	double lineAdaptiveSpan;
	unsigned int x_pointDensity = 10;
	unsigned int y_pointDensity = 10;
protected:
	void paintEvent(QPaintEvent *);
	void mouseMoveEvent(QMouseEvent * e);
	void refreshUI();
	void drawCoordinate();
};
