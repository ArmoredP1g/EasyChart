# EasyChart
利用qt绘图事件实现的简易折线图控件

## 使用方式：先设置固定边界或设置成自适应，再添加点和线

## 函数说明

|   函数名   |   实现功能   |   备注   |
| ---- | ---- | ---- |
|  void setTitle(QString name)    |   设置标题   |      |
| void addLine(QList<QPointF> list)   |   添加线段，颜色为默认的黑色   |      |
|  void addLine(QList<QPointF> list,QColor color)    |   重载，增加一个自定义颜色的参数   |      |
  |  void addPoint(QPointF point,string time)  |   添加一个点   |      |
  |   void addPoint(QPointF point, string time, QColor color)  |   重载，增加一个自定义颜色的参数   |      |  
|   void setAdaptivePoint(double span)   |  设置针对点的自适应边界    |   要在添加点之前调用，否则会出错   |
|   void setAdaptiveLine(double span)   |   设置针对线的自适应边界   |   要在添加线之前调用，否则会出错   |
|   void set_x_pointDensity(int x)   |   设置x轴坐标密度   |      |
|   void set_y_pointDensity(int y)  |   设置y轴坐标密度   |      |
  |   void set_gridOn(bool on) |   是否显示坐标网格线   |   默认为true   |
  |   void set_padding(double padding)  |   设置padding   |      |
  |   void setStaticBorder(double x_min, double x_max, double y_min, double y_max) |   设置固定的边界范围   |   该函数会自动取消之前的自适应设置   |

