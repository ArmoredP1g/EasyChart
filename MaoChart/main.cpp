#include "MaoChart.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MaoChart w;
	w.setAdaptivePoint(5);
	w.addPoint(QPointF(1, 1));
	w.addPoint(QPointF(2, 3));
	w.addPoint(QPointF(5, 4));
	w.addPoint(QPointF(7, 7));
	w.addPoint(QPointF(8, 9));
    w.show();
    return a.exec();
}
