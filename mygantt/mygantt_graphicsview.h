#ifndef GANTTGRAPHICSVIEW_H
#define GANTTGRAPHICSVIEW_H

#include <QGraphicsView>

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

protected:
    void resizeEvent(QResizeEvent *event);

    void scrollContentsBy(int dx, int dy);

private:
    void initialize();

private:
    GanttScene *m_scene;
    GanttTreeView *m_treeView;

    friend class GanttTreeView;

};

#endif // GANTTVIEW_H
