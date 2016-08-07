#ifndef GANTTWIDGET_H
#define GANTTWIDGET_H

#include "mygantt_infoleaf.h"

#include <QModelIndex>

#include <QList>

#include <QWidget>


namespace Ui {
class GanttWidget;
}

class GanttScene;
class GanttTreeModel;

class GanttWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GanttWidget(QWidget *parent = 0);

    ~GanttWidget();

    void addItems(GanttInfoItem* item);
    void addItems(const QList<GanttInfoItem*>& items);

private slots:
    void on_comboBox_mode_currentIndexChanged(int index);

    void expanded(const QModelIndex& index);
    void collapsed(const QModelIndex& index);

    void onScrollGraphicsView(int value);


    void on_pushButton_slider_clicked();

    void on_pushButton_header_clicked();

private:
    void updatePos(GanttInfoNode* from);
    void updatePosHelper(GanttInfoItem* item);



    void onResize();

private:
    Ui::GanttWidget *ui;

    GanttScene *m_scene;
    GanttTreeModel *m_model;

    qreal m_curSceneMax;

    friend class GanttScene;
    friend class MyGanttView;
};

#endif // GANTTWIDGET_H
