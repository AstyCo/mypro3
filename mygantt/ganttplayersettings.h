#ifndef GANTTPLAYERSETTINGS_H
#define GANTTPLAYERSETTINGS_H

#include <QToolBar>

namespace Ui {
class GanttPlayerSettings;
}

class GanttPlayerSettings : public QToolBar
{
    Q_OBJECT

public:
    explicit GanttPlayerSettings(QWidget *parent = 0);
    ~GanttPlayerSettings();

private:
    void initActions();

private:
    Ui::GanttPlayerSettings *ui;

    QList<QAction*> m_actions;

};

#endif // GANTTPLAYERSETTINGS_H
