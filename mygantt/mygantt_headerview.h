#ifndef GANTTHEADERVIEW_H
#define GANTTHEADERVIEW_H

#include <QHeaderView>

class GanttTreeView;

class GanttHeaderView : public QHeaderView
{
public:
    GanttHeaderView(Qt::Orientation orientation, QWidget * parent = 0);

protected:
    void paintEvent(QPaintEvent *e);

private:
    GanttTreeView *m_view;

};

#endif // GANTTHEADERVIEW_H
