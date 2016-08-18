#include "ganttplayersettings.h"
#include "ui_ganttplayersettings.h"

GanttPlayerSettings::GanttPlayerSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GanttPlayerSettings)
{
    ui->setupUi(this);
}

GanttPlayerSettings::~GanttPlayerSettings()
{
    delete ui;
}
