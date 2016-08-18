#ifndef MYGANTT_PLAYER_H
#define MYGANTT_PLAYER_H

#include <QWidget>

namespace Ui {
class GanttPlayerControl;
}

class GanttPlayerControl : public QWidget
{
    Q_OBJECT

public:
    explicit GanttPlayerControl(QWidget *parent = 0);
    ~GanttPlayerControl();

private:
    Ui::GanttPlayerControl *ui;
};

#endif // MYGANTT_PLAYER_H
