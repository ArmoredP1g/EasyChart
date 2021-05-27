#include "MaoChart.h"
#include <QPainter>
#include <QtCharts/QLineSeries>
#include <QtCharts/QScatterSeries>

MaoChart::MaoChart(QWidget *parent)
    : QWidget(parent)
{
	pointList = new QList<QPointF>;

	x_upperRange = -100000000;
	x_lowerRange = 1000000000;
	y_upperRange = -1000000;
	y_lowerRange = 10000000000;
	pointAdaptiveSpan = -1;
	lineAdaptiveSpan = -1;

	setMouseTracking(true);//无需点击即可触发鼠标移动事件

	//配置界面刷新为100hz
	timer = new QTimer(this);
	connect(timer, &QTimer::timeout, this, &MaoChart::refreshUI);
	timer->start(1000 / 30);

    ui.setupUi(this);
}

MaoChart::~MaoChart()
{
	delete pointList;
}

//drawing event
void MaoChart::paintEvent(QPaintEvent *)
{	
	if (QApplication::activeWindow() == this)
	{
		window_width = QApplication::activeWindow()->width();
		window_height = QApplication::activeWindow()->height();
	}

	//画坐标
	drawCoordinate();

	//画点
	drawPoints();
}

void MaoChart::mouseMoveEvent(QMouseEvent *e)
{

}

void MaoChart::set_padding(double padding)
{
	this->padding = padding;
}

void MaoChart::setTitle(QString name)
{
	this->Title = name;
}

void MaoChart::addLine(QList<QPointF> list)
{

	for (int i = 0; i < list.size(); i++)
	{
		if (lineAdaptiveSpan != -1)
		{
			if (list.at(i).x() >= x_upperRange)
			{
				x_upperRange = list.at(i).x() + lineAdaptiveSpan;
			}
			if (list.at(i).x() <= x_lowerRange)
			{
				x_lowerRange = list.at(i).x() + lineAdaptiveSpan;
			}
			if (list.at(i).y() >= y_upperRange)
			{
				y_upperRange = list.at(i).y() + lineAdaptiveSpan;
			}
			if (list.at(i).y() <= y_lowerRange)
			{
				y_lowerRange = list.at(i).y() + lineAdaptiveSpan;
			}
		}
	}

	LineList.append(list);
}

void MaoChart::setAdaptivePoint(double span)
{
	pointAdaptiveSpan = span;
}

void MaoChart::setAdaptiveLine(double span)
{
	lineAdaptiveSpan = span;
}

void MaoChart::set_x_pointDensity(int x)
{
	if (x >= 0)
	{
		x_pointDensity = x;
	}
}

void MaoChart::set_y_pointDensity(int x)
{
	if (x >= 0)
	{
		y_pointDensity = x;
	}
}

void MaoChart::addPoint(QPointF point)
{
	QLabel * lable = new QLabel(this);
	lable->setStyleSheet("QLabel{background-color:2px solid rgb(0, 200, 150);}");
	lable->setText("点");
	pointList->append(point);
	LableList_point.append(lable);
	if (pointAdaptiveSpan != -1)
	{
		if (point.x() >= x_upperRange)
		{
			x_upperRange = point.x() + pointAdaptiveSpan;
		}
		if (point.x() <= x_lowerRange)
		{
			x_lowerRange = point.x() - pointAdaptiveSpan;
		}
		if (point.y() >= y_upperRange)
		{
			y_upperRange = point.y() + pointAdaptiveSpan;
		}
		if (point.y() <= y_lowerRange)
		{
			y_lowerRange = point.y() - pointAdaptiveSpan;
		}
	}
}

void MaoChart::drawCoordinate()
{
	QPainter painter(this);
	QPen pen; //画笔
	pen.setColor(QColor(0, 0, 0));
	pen.setWidth(2);
	painter.setPen(pen);
	painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
	//绘制x轴
	QPointF x_startpoint = QPointF(padding, window_height - padding);
	QPointF x_endpoint = QPointF(window_width- padding, window_height - padding);
	//绘制y轴
	QPointF y_startpoint = QPointF(padding, window_height - padding);
	QPointF y_endpoint = QPointF(padding, padding);
	painter.drawLine(x_startpoint, x_endpoint);
	painter.drawLine(y_startpoint, y_endpoint);

	//绘制x轴线数点
	double x_PixelSpacing = (window_width - 2*padding) / (x_pointDensity + 1);
	double y_PixelSpacing = (window_height - 2 * padding) / (y_pointDensity + 1);
	for (int i = 1; i < x_pointDensity +1; i++)
	{
		QString num = QString::number(2309.1443, 10, 0);
		QPointF s = QPointF(padding +i*x_PixelSpacing, window_height - padding);
		QPointF e = QPointF(padding + i * x_PixelSpacing, window_height - padding + 5);
		
		QRectF rect = QRectF(QPointF((padding + i * x_PixelSpacing) - 22.5, window_height - padding + 7), QPointF((padding + i * x_PixelSpacing) + 22.5, window_height - padding + 7 + 22.5));

		painter.drawLine(s, e);
		//painter.drawRect(rect);
		painter.drawText(rect, Qt::AlignHCenter, num);
	}


	//绘制y轴线数点
	for (int i = 1; i < y_pointDensity + 1; i++)
	{
		QString num = QString::number(2309.1443, 10, 0);
		QPointF s = QPointF(padding, window_height - i * y_PixelSpacing - padding);
		QPointF e = QPointF(padding - 5, window_height - i * y_PixelSpacing - padding);
		
		QRectF rect = QRectF(QPointF(padding - 7 - 45, window_height - i * y_PixelSpacing - padding - 22.5/3), QPointF(padding - 7, window_height - i * y_PixelSpacing - padding + 22.5/3));
		
		painter.drawLine(s, e);
		//painter.drawRect(rect);
		painter.drawText(rect, Qt::AlignHCenter, num);
	}
}

void MaoChart::drawPoints()
{
	for (int i = 0; i < pointList->size(); i++)
	{
		double pixel_x = X_axis_mapping(pointList->at(i).x());
		double pixel_y = Y_axis_mapping(pointList->at(i).y());
		
		LableList_point.at(i)->setVisible(true);
		LableList_point.at(i)->setGeometry(pixel_x - 2, pixel_y - 2, 4, 4);
	}

}

void MaoChart::update()
{
	//huaxian
	/*for (int i = 0; i < LineList.size(); i++)
	{
		QLineSeries *series = new QLineSeries(chart);
		for (int j = 0; j < LineList.at(i).size(); j++)
		{
			if (lineAdaptiveSpan != -1)
			{
				if (LineList.at(i).at(j).x() >= x_upperRange)
				{
					x_upperRange = LineList.at(i).at(j).x() + lineAdaptiveSpan;
				}
				if (LineList.at(i).at(j).x() <= x_lowerRange)
				{
					x_lowerRange = LineList.at(i).at(j).x() + lineAdaptiveSpan;
				}
				if (LineList.at(i).at(j).y() >= y_upperRange)
				{
					y_upperRange = LineList.at(i).at(j).y() + lineAdaptiveSpan;
				}
				if (LineList.at(i).at(j).y() <= y_lowerRange)
				{
					y_lowerRange = LineList.at(i).at(j).y() + lineAdaptiveSpan;
				}
			}
			series->append(LineList.at(i).at(j));
		}
		chart->addSeries(series);
	}*/

	//draw points
	/*QScatterSeries * series2 = new QScatterSeries(chart);
	for (int i = 0; i < pointList->size(); i++)
	{
		series2->append(pointList->at(i));
	}
	series2->setMarkerSize(4);
	series2->setBorderColor(QColor(0, 0, 0, 0));
	chart->addSeries(series2);

	chart->createDefaultAxes();
	chart->axisX()->setRange(x_lowerRange, x_upperRange);
	chart->axisY()->setRange(y_lowerRange, y_upperRange);
	chart->legend()->setVisible(false);

	chart->setTitle(Title);
	ui.Chart->setChart(chart);



	this->update();*/
}

double MaoChart::Interpolation(double x1, double y1, double x2, double y2, double value_x)
{
	return (y1 - y2)*((value_x - x2) / (x1 - x2)) + y2;
}


//x轴空间映射
double MaoChart::X_axis_mapping(double x_value)
{
	return Interpolation(x_lowerRange, padding, x_upperRange, window_width - padding, x_value);
}

//y轴空间映射
double MaoChart::Y_axis_mapping(double y_value)
{
	return Interpolation(y_lowerRange, window_height - padding, y_upperRange, padding, y_value);
}

void MaoChart::refreshUI()
{
	this->update();
}