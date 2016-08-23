#include "ganttplayersettings.h"
#include "ui_ganttplayersettings.h"

#include <QMenu>

GanttPlayerSettings::GanttPlayerSettings(QWidget *parent) :
    QToolBar(parent),
    ui(new Ui::GanttPlayerSettings)
{
    ui->setupUi(this);

    setToolButtonStyle(Qt::ToolButtonIconOnly);
    initActions();
}

GanttPlayerSettings::~GanttPlayerSettings()
{
    delete ui;
}

void GanttPlayerSettings::initActions()
{
    QAction *p_action;

    p_action = new QAction(QIcon(":/Buttons 64x64/reg_speed_on_64.png"),"speed",this);
//    connect(m_updateAction,SIGNAL(triggered()),this,SLOT(on_update()));
    m_actions.append(p_action);

    QMenu *p_menu = new QMenu(this);

    p_menu->addAction("x1");
    p_menu->addAction("x3");
    p_menu->addAction("x5");

    p_action->setMenu(p_menu);


    p_action = new QAction(QIcon(":/Buttons 64x64/reg_step_on_64.png"),"step",this);
//    connect(m_updateAction,SIGNAL(triggered()),this,SLOT(on_update()));
    m_actions.append(p_action);

    addActions(m_actions);
}
