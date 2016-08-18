#ifndef GANTTPLAYERSETTINGS_H
#define GANTTPLAYERSETTINGS_H

#include <QWidget>

namespace Ui {
class GanttPlayerSettings;
}

class GanttPlayerSettings : public QWidget
{
    Q_OBJECT

public:
    explicit GanttPlayerSettings(QWidget *parent = 0);
    ~GanttPlayerSettings();

private:
    Ui::GanttPlayerSettings *ui;
};

#endif // GANTTPLAYERSETTINGS_H
