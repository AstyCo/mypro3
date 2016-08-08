#ifndef GANTTHEADERVIEW_H
#define GANTTHEADERVIEW_H

#include <QHeaderView>

class GanttHeaderView : public QHeaderView
{
public:
    GanttHeaderView(Qt::Orientation orientation, QWidget * parent = 0);

    void update();

protected:
    void paintEvent(QPaintEvent *e);
    void paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const;
    bool event(QEvent *e);

};

#endif // GANTTHEADERVIEW_H
