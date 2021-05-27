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

	//!!!该控件在被其他窗口调用时 活动窗口不会是该控件
	if (QApplication::activeWindow() != NULL)
	{
		window_width = QApplication::activeWindow()->width();
		window_height = QApplication::activeWindow()->height();
	}

	//画标题
	drawTitle();
	//画坐标
	drawCoordinate();

	//画点
	drawPoints();

	//画线
	drawLines();

	//检测鼠标悬停在某一点上
	detectPointHovering();
}

void MaoChart::mouseMoveEvent(QMouseEvent *e)
{
	CursorPos_x = e->pos().x();
	CursorPos_y = e->pos().y();
}

void MaoChart::setStaticBorder(double x_min, double x_max, double y_min, double y_max)
{
	setAdaptiveLine(-1);
	setAdaptivePoint(-1);
	x_upperRange = x_max;
	x_lowerRange = x_min;
	y_upperRange = y_max;
	y_lowerRange = y_min;
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
	QPen * pen = new QPen;

	//默认风格
	pen->setColor(Qt::black);
	pen->setWidth(1);
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
		addPoint(list.at(i), "");
	}

	LineList.append(list);
	LinePenList.append(pen);
}

void MaoChart::addLine(QList<QPointF> list,QColor color)
{
	QPen * pen = new QPen;
	//默认风格
	pen->setColor(color);
	pen->setWidth(1);
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
		addPoint(list.at(i), "",color);
	}

	LineList.append(list);
	LinePenList.append(pen);
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

void MaoChart::set_gridOn(bool on)
{
	gridOn = on;
}

void MaoChart::addPoint(QPointF point, string time)
{
	QPen * pen = new QPen;

	//默认风格
	pen->setColor(Qt::black);
	pen->setWidth(1);

	pointList->append(point);
	pointPenList.append(pen);
	pointTimeList.append(QString::fromLocal8Bit(time.data()));


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

//自定义颜色的重载
void MaoChart::addPoint(QPointF point, string time,QColor color)
{
	QPen * pen = new QPen;

	pen->setColor(color);
	pen->setWidth(1);

	pointList->append(point);
	pointPenList.append(pen);
	pointTimeList.append(QString::fromLocal8Bit(time.data()));


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

void MaoChart::drawTitle()
{
	QPainter painter(this);
	QRectF rect;
	QFont font("黑体", 20, QFont::Bold, false);
	rect = QRectF(window_width/2 - 250, 0, 500, 50);
	painter.setFont(font);
	painter.drawText(rect, Qt::AlignHCenter, Title);

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
	for (int i = 0; i < x_pointDensity +2; i++)
	{
		QString num = QString::number(x_lowerRange+i*(x_upperRange-x_lowerRange)/(x_pointDensity+1), 10, 2);

		//坐标点线段的起点和终点
		QPointF s = QPointF(padding +i*x_PixelSpacing, window_height - padding);
		QPointF e = QPointF(padding + i * x_PixelSpacing, window_height - padding + 5);

		//纵向网格线的终点 起点和坐标点线段一致
		QPointF e_grid = QPointF(padding + i * x_PixelSpacing, padding);
		
		QRectF rect = QRectF(QPointF((padding + i * x_PixelSpacing) - 22.5, window_height - padding + 7), QPointF((padding + i * x_PixelSpacing) + 22.5, window_height - padding + 7 + 22.5));

		painter.drawLine(s, e);

		//绘制网格
		if (gridOn)
		{
			pen.setColor(QColor(128, 128, 128));
			pen.setWidth(1);
			pen.setStyle(Qt::DashLine);
			painter.setPen(pen);
			painter.setRenderHints(false);

			painter.drawLine(s, e_grid);
			
			pen.setColor(QColor(0, 0, 0));
			pen.setWidth(2);
			painter.setPen(pen);
			painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

		}
		//painter.drawRect(rect);
		painter.drawText(rect, Qt::AlignHCenter, num);
	}


	//绘制y轴线数点
	for (int i = 0; i < y_pointDensity + 2; i++)
	{
		QString num = QString::number(y_lowerRange + i * (y_upperRange - y_lowerRange) / (y_pointDensity+1), 10, 2);

		//坐标点线段的起点和终点
		QPointF s = QPointF(padding, window_height - i * y_PixelSpacing - padding);
		QPointF e = QPointF(padding - 5, window_height - i * y_PixelSpacing - padding);
		//横向网格线的终点 起点和坐标点线段一致
		QPointF e_grid = QPointF(window_width - padding, window_height - i * y_PixelSpacing - padding);
		
		QRectF rect = QRectF(QPointF(padding - 7 - 45, window_height - i * y_PixelSpacing - padding - 22.5/3), QPointF(padding - 7, window_height - i * y_PixelSpacing - padding + 22.5/3));
		
		painter.drawLine(s, e);

		//绘制网格
		if (gridOn)
		{
			pen.setColor(QColor(128, 128, 128));
			pen.setWidth(1);
			pen.setStyle(Qt::DashLine);
			painter.setPen(pen);
			painter.setRenderHints(false);

			painter.drawLine(s, e_grid);

			pen.setColor(QColor(0, 0, 0));
			pen.setWidth(2);
			painter.setPen(pen);
			painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

		}

		//painter.drawRect(rect);
		painter.drawText(rect, Qt::AlignHCenter, num);
	}
}

void MaoChart::drawPoints()
{
	QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
	for (int i = 0; i < pointList->size(); i++)
	{
		double pixel_x = X_axis_mapping(pointList->at(i).x());
		double pixel_y = Y_axis_mapping(pointList->at(i).y());
		

		painter.setPen(*pointPenList.at(i));
		painter.setBrush(QBrush(pointPenList.at(i)->color()));
		painter.drawEllipse(pixel_x - 3, pixel_y - 3, 3 *2 , 3 * 2);

	}

}

void MaoChart::drawLines()
{
	QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
	for (int i = 0; i < LineList.size(); i++)
	{
		painter.setPen(*LinePenList.at(i));
		for (int j = 0; j < LineList.at(i).size()-1; j++)
		{
			double pixel_x1 = X_axis_mapping(LineList.at(i).at(j).x());
			double pixel_y1 = Y_axis_mapping(LineList.at(i).at(j).y());
			double pixel_x2 = X_axis_mapping(LineList.at(i).at(j + 1).x());
			double pixel_y2 = Y_axis_mapping(LineList.at(i).at(j + 1).y());

			painter.drawLine(pixel_x1, pixel_y1, pixel_x2, pixel_y2);
		}
	}

}


void MaoChart::detectPointHovering()
{
	double pixel_x;
	double pixel_y;

	for (int i = 0; i < pointList->size(); i++)
	{
		pixel_x = X_axis_mapping(pointList->at(i).x());
		pixel_y = Y_axis_mapping(pointList->at(i).y());
		//若光标在该点2.5个像素以内, 则显示该点详细信息
		
		if (sqrt(pow(CursorPos_x - pixel_x, 2) + pow(CursorPos_y - pixel_y, 2)) < 3.5)
		{
			QString str ="x:" + QString::number(pointList->at(i).x(), 10, 3) + " \ny:" + QString::number(pointList->at(i).y(), 10, 3) + QStringLiteral("\n时间:") + pointTimeList.at(i);

			QPainter painter(this);
			QRectF rect;
			rect = QRectF(CursorPos_x-60, CursorPos_y-40, 120, 40);
			painter.drawRect(rect);
			painter.setPen(QColor(214,20,103));

			painter.drawText(rect, Qt::AlignHCenter, str);

			painter.setPen(QColor(0, 255 ,100));
			painter.setBrush(QBrush(QColor( 0,255, 100)));
			painter.drawEllipse(pixel_x - 4, pixel_y - 4, 4 * 2, 4 * 2);

			break;
		}
	}
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