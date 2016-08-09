#ifndef GANTTGRAPHICSVIEW_H
#define GANTTGRAPHICSVIEW_H

#include <QGraphicsView>

#include <QModelIndex>

class GanttWidget;
class GanttScene;

class GanttTreeView;

class GanttGraphicsView : public QGraphicsView
{
public:
    GanttGraphicsView(QWidget *parent = 0);
    GanttGraphicsView(QGraphicsScene * scene, QWidget * parent = 0);


    void setScene(GanttScene *scene);

    void setTreeView(GanttTreeView *treeView);

    void changeExpanding(const QModelIndex& index);

    void setHSliderHeight(int hSliderHeight);

protected:
    void resizeEvent(QResizeEvent *event);

    void scrollContentsBy(int dx, int dy);

    void mouseMoveEvent(QMouseEvent *event);
    void leaveEvent(QEvent *);

private:
    void initialize();

private:
    GanttScene *m_scene;
    GanttTreeView *m_treeView;

    int m_hSliderHeight;

    friend class GanttTreeView;

};

#endif // GANTTVIEW_H
