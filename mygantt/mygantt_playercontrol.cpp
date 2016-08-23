#include "mygantt_playercontrol.h"
#include "ui_mygantt_playercontrol.h"

#include "mygantt_globalvalues.h"

#include <QTimer>

GanttPlayerControl::GanttPlayerControl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GanttPlayerControl)
{
    ui->setupUi(this);

    setStep(GANTTPLAYER_DEFAULT_STEP);
    setPlayFrequency(GANTTPLAYER_DEFAULT_FREQUENCY);


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
long long GanttPlayerControl::step() const
{
    return m_step;
}
void GanttPlayerControl::setStep(long long step)
{
    m_step = step;
}

void GanttPlayerControl::makeStep()
{
    emit makeStep(m_step);
}


void GanttPlayerControl::on_forward_toggled(bool checked)
{
    static QTimer *p_timer = NULL;
    if(checked)
    {
        p_timer = new QTimer();
        connect(p_timer, SIGNAL(timeout()), this, SLOT(makeStep()));
        p_timer->start(msec());
    }
    else
    {
        if(p_timer)
        {
            disconnect(p_timer,SIGNAL(timeout()),this,0);
            delete p_timer;
            p_timer = NULL;
        }
    }
}

int GanttPlayerControl::msec()
{
    return _MILISECONDS_IN_SECOND / m_playFrequency;
}
qreal GanttPlayerControl::playFrequency() const
{
    return m_playFrequency;
}

void GanttPlayerControl::setPlayFrequency(const qreal &playFrequency)
{
    m_playFrequency = playFrequency;
}


void GanttPlayerControl::on_backward_toggled(bool checked)
{

}

void GanttPlayerControl::on_fragmentBegin_clicked()
{

}

void GanttPlayerControl::on_fragmentEnd_clicked()
{

}

void GanttPlayerControl::on_nextEvent_clicked()
{

}

void GanttPlayerControl::on_pause_toggled(bool checked)
{

}

void GanttPlayerControl::on_prevEvent_clicked()
{

}

void GanttPlayerControl::on_record_toggled(bool checked)
{

}

void GanttPlayerControl::on_stepBackward_clicked()
{

}

void GanttPlayerControl::on_stepForward_clicked()
{

}

void GanttPlayerControl::on_stop_toggled(bool checked)
{

}
