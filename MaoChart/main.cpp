#include "MaoChart.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	QList<QPointF> list;
    MaoChart w;
	w.setAdaptivePoint(0);
	w.setAdaptiveLine(0);
	w.addPoint(QPointF(1, 1),"各种时间", QColor(68, 78, 228));
	w.addPoint(QPointF(2, 3), "各种时间", QColor(68, 78, 228));
	w.addPoint(QPointF(5, 4), "各种时间", QColor(68, 78, 228));
	w.addPoint(QPointF(7, 7), "各种时间", QColor(68, 78, 228));
	w.addPoint(QPointF(8, 9), "各种时间", QColor(68, 78, 228));


	list.append(QPointF(1, 2));
	list.append(QPointF(2, 4));
	list.append(QPointF(5, 5));
	list.append(QPointF(7, 8));
	list.append(QPointF(8, 10));

	w.addLine(list, QColor(168, 78, 228));

    w.show();
    return a.exec();
}
