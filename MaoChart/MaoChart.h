#pragma once

#include <QtWidgets/QWidget>
#include <vector>
#include <qlist.h>
#include "ui_MaoChart.h"
#include<qtimer.h>
#include<qlabel.h>
#include<QKeyEvent>
#include<qcolor.h>

#define PADDING 20.0;

#include<iostream>
using namespace std;

class MaoChart : public QWidget
{
    Q_OBJECT

public:
    MaoChart(QWidget *parent = Q_NULLPTR);
	~MaoChart();
	void setTitle(QString name); //设置标题
	void addLine(QList<QPointF> list);	//添加线段
	void addLine(QList<QPointF> list,QColor color);	//添加线段
	void setAdaptivePoint(double span);	//设置自适应边界
	void setAdaptiveLine(double span);
	void set_x_pointDensity(int x);	//设置坐标密度
	void set_y_pointDensity(int x);
	void set_gridOn(bool on);//设置显示坐标网格
	void set_padding(double padding);//设置padding
	void setStaticBorder(double x_min, double x_max, double y_min, double y_max); //设置固定边界范围
	void addPoint(QPointF point,string time);	//添加点点
	void addPoint(QPointF point, string time, QColor color);	//添加点点

private:
	void drawTitle();
	void drawPoints();
	void drawLines();
	double Interpolation(double x1, double y1, double x2, double y2, double value_x);//插值
	double X_axis_mapping(double x_value);	//x轴空间映射
	double Y_axis_mapping(double y_value);	//y轴空间映射
	void detectPointHovering();	//检测鼠标停留
    Ui::MaoChartClass ui;
	QString Title = "Untitled";

	QList<QPointF> *pointList;
	QList<QString> pointTimeList;
	QList<QPen*> pointPenList;

	QList<QList<QPointF>> LineList;
	QList<QPen*> LinePenList;

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
	double CursorPos_x;
	double CursorPos_y;
	bool gridOn = true;
protected:
	void paintEvent(QPaintEvent *);
	void mouseMoveEvent(QMouseEvent * e);
	void refreshUI();
	void drawCoordinate();
};
