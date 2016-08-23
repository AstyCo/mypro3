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

    long long step() const;
    void setStep(long long step);

    qreal playFrequency() const;
    void setPlayFrequency(const qreal &playFrequency);

signals:
    void makeStep(long long stepLengthInMicrosec);



private slots:
    void makeStep();

    void on_forward_toggled(bool checked);

    void on_backward_toggled(bool checked);

    void on_fragmentBegin_clicked();

    void on_fragmentEnd_clicked();

    void on_nextEvent_clicked();

    void on_pause_toggled(bool checked);

    void on_prevEvent_clicked();

    void on_record_toggled(bool checked);

    void on_stepBackward_clicked();

    void on_stepForward_clicked();

    void on_stop_toggled(bool checked);

private:
    int msec();

private:
    Ui::GanttPlayerControl *ui;

    long long m_step; ///< Шаг текущего времени в микросекундах
    qreal m_playFrequency; ///< Число шагов в секунду
};

#endif // MYGANTT_PLAYER_H
