#include "ganttintervalsliderzoom.h"
#include "ui_ganttintervalsliderzoom.h"

GanttIntervalSliderZoom::GanttIntervalSliderZoom(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GanttIntervalSliderZoom)
{
    ui->setupUi(this);

    connect(ui->increaseButton,SIGNAL(clicked(bool)),this,SIGNAL(increaseButtonClicked(bool)));
    connect(ui->restoreButton,SIGNAL(clicked(bool)),this,SIGNAL(restoreButtonClicked(bool)));

}

GanttIntervalSliderZoom::~GanttIntervalSliderZoom()
{
    delete ui;
}
