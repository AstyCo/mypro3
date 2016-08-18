#include "mygantt_playercontrol.h"
#include "ui_mygantt_playercontrol.h"

GanttPlayerControl::GanttPlayerControl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GanttPlayerControl)
{
    ui->setupUi(this);

//    ui->fragmentBegin->setStyleSheet(":hover {border-image: url(\":/Buttons 64x64/begin_on_64.png\");}");


    ui->stop->setActiveIcon(QIcon(":/Buttons 64x64/stop_off_64.png"));
    ui->stop->setCheckedIcon(QIcon(":/Buttons 64x64/stop_on_64.png"));

    ui->fragmentBegin->setActiveIcon(QIcon(":/Buttons 64x64/begin_off_64.png"));
    ui->fragmentBegin->setCheckedIcon(QIcon(":/Buttons 64x64/begin_on_64.png"));

    ui->prevEvent->setActiveIcon(QIcon(":/Buttons 64x64/prev_off_64.png"));
    ui->prevEvent->setCheckedIcon(QIcon(":/Buttons 64x64/prev_on_64.png"));

    ui->stepBackward->setActiveIcon(QIcon(":/Buttons 64x64/back_step_off_64.png"));
    ui->stepBackward->setCheckedIcon(QIcon(":/Buttons 64x64/back_step_on_64.png"));

    ui->backward->setActiveIcon(QIcon(":/Buttons 64x64/back_off_64.png"));
    ui->backward->setCheckedIcon(QIcon(":/Buttons 64x64/back_on_64.png"));

    ui->pause->setActiveIcon(QIcon(":/Buttons 64x64/pause_off_64.png"));
    ui->pause->setCheckedIcon(QIcon(":/Buttons 64x64/pause_on_64.png"));

    ui->forward->setActiveIcon(QIcon(":/Buttons 64x64/forward_off_64.png"));
    ui->forward->setCheckedIcon(QIcon(":/Buttons 64x64/forward_on_64.png"));

    ui->stepForward->setActiveIcon(QIcon(":/Buttons 64x64/forward_step_off_64.png"));
    ui->stepForward->setCheckedIcon(QIcon(":/Buttons 64x64/forward_step_on_64.png"));

    ui->nextEvent->setActiveIcon(QIcon(":/Buttons 64x64/next_off_64.png"));
    ui->nextEvent->setCheckedIcon(QIcon(":/Buttons 64x64/next_on_64.png"));

    ui->fragmentEnd->setActiveIcon(QIcon(":/Buttons 64x64/end_off_64.png"));
    ui->fragmentEnd->setCheckedIcon(QIcon(":/Buttons 64x64/end_on_64.png"));

    ui->record->setActiveIcon(QIcon(":/Buttons 64x64/rec_off_64.png"));
    ui->record->setCheckedIcon(QIcon(":/Buttons 64x64/rec_on_64.png"));


}

GanttPlayerControl::~GanttPlayerControl()
{
    delete ui;
}
