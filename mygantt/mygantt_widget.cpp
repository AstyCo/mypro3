#include "mygantt_widget.h"
#include "ui_ganttwidget.h"

#include "mygantt_graphicsview.h"
#include "mygantt_scene.h"
#include "mygantt_treemodel.h"

#include "mygantt_infoleaf.h"
#include "mygantt_infonode.h"

#include "mygantt_item.h"

#include <QScrollBar>


GanttWidget::GanttWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GanttWidget)
{
    ui->setupUi(this);

    if(layout())
    {
        layout()->setMargin(2);
        layout()->setSpacing(0);
    }

    ui->treeView->setContentsMargins(0,0,0,0);
    ui->treeView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    ui->treeView->verticalScrollBar()->setStyleSheet("QScrollBar {width:0px;}");
//    ui->treeView->horizontalScrollBar()->setStyleSheet("QScrollBar {height:5px;}");

    ui->treeView->setGraphicsView(ui->ganttView);
    ui->ganttView->setTreeView(ui->treeView);

    connect(ui->ganttView->verticalScrollBar(),SIGNAL(valueChanged(int)),this,SLOT(onScrollGraphicsView(int)));

    m_scene = new GanttScene(this);
    m_model = new GanttTreeModel(NULL,this);

    ui->ganttView->setScene(m_scene);
    ui->treeView->setModel(m_model);

    connect(ui->treeView,SIGNAL(expanded(QModelIndex)), this,SLOT(expanded(QModelIndex)));
    connect(ui->treeView,SIGNAL(collapsed(QModelIndex)), this,SLOT(collapsed(QModelIndex)));
    connect(m_scene->slider(),SIGNAL(sliderPosChanged(qreal)),this,SLOT(repaintDtHeader()));

    GanttInfoNode *test1 = new GanttInfoNode
            ,*test3 = new GanttInfoNode;
    GanttInfoLeaf *item11 = new GanttInfoLeaf
            ,*item12 = new GanttInfoLeaf
            ,*item21 = new GanttInfoLeaf
            ,*item31 = new GanttInfoLeaf
            ,*item32 = new GanttInfoLeaf;

//    item11->setStart(UtcDateTime(QDate(2015,01,29)));
//    item11->setFinish(UtcDateTime(QDate(2015,01,29),QTime(5,0)));
//    item11->setColor(Qt::yellow);
//    item11->setTitle("leaf11");

//    item12->setStart(UtcDateTime(QDate(2015,01,29),QTime(10,0)));
//    item12->setFinish(UtcDateTime(QDate(2015,01,30),QTime(5,0)));
//    item12->setColor(Qt::yellow);
//    item12->setTitle("leaf12");

//    item21->setStart(UtcDateTime(QDate(2015,01,30),QTime(11,0)));
//    item21->setFinish(UtcDateTime(QDate(2015,01,30),QTime(14,0)));
//    item21->setColor(Qt::red);
//    item21->setTitle("leaf21");

//    item31->setStart(UtcDateTime(QDate(2015,01,31),QTime(10,0)));
//    item31->setFinish(UtcDateTime(QDate(2015,02,02),QTime(5,0)));
//    item31->setColor(Qt::green);
//    item31->setTitle("leaf31");

//    item32->setStart(UtcDateTime(QDate(2015,02,02),QTime(10,0)));
//    item32->setFinish(UtcDateTime(QDate(2015,02,02),QTime(20,0)));
//    item32->setColor(Qt::green);
//    item32->setTitle("leaf32");
    item11->setStart(UtcDateTime(QDate(2015,02,02)));
    item11->setFinish(UtcDateTime(QDate(2015,02,02),QTime(1,30)));
    item11->setColor(Qt::yellow);
    item11->setTitle("leaf11");

    item12->setStart(UtcDateTime(QDate(2015,02,02),QTime(1,30,30)));
    item12->setFinish(UtcDateTime(QDate(2015,02,02),QTime(1,32)));
    item12->setColor(Qt::yellow);
    item12->setTitle("leaf12");

    item21->setStart(UtcDateTime(QDate(2015,02,02),QTime(1,45)));
    item21->setFinish(UtcDateTime(QDate(2015,02,02),QTime(2,0)));
    item21->setColor(Qt::red);
    item21->setTitle("leaf21");

    item31->setStart(UtcDateTime(QDate(2015,02,02),QTime(3,0)));
    item31->setFinish(UtcDateTime(QDate(2015,02,02),QTime(4,0)));
    item31->setColor(Qt::green);
    item31->setTitle("leaf31");

    item32->setStart(UtcDateTime(QDate(2015,02,02),QTime(4,30)));
    item32->setFinish(UtcDateTime(QDate(2015,02,02),QTime(5,0)));
    item32->setColor(Qt::green);
    item32->setTitle("leaf32");

    test1->append(item11);
    test1->append(item12);
    test1->setTitle("node1");

//    test2->append(item21);
    test3->append(item31);
    test3->append(item32);
    test3->setTitle("node2");

    QList<GanttInfoItem*> testL;
    testL.append(test1);
    testL.append(item21);
    testL.append(test3);

    addItems(testL);

}

GanttWidget::~GanttWidget()
{
    delete ui;
}

void GanttWidget::addItems(GanttInfoItem* item)
{

    m_model->addItems(item);
    m_scene->addItems(item);

}

void GanttWidget::addItems(const QList<GanttInfoItem *> &items)
{
    m_model->addItems(items);
    m_scene->addItems(items);
}



UtcDateTime GanttWidget::slidersDt() const
{
    if(!m_scene)
        return UtcDateTime();
    return m_scene->slidersDt();
}

void GanttWidget::on_comboBox_mode_currentIndexChanged(int index)
{
    GanttHeader::GanttPrecisionMode newMode;

    switch(index)
    {
    case 0:
        newMode = GanttHeader::months1;
        break;
    case 1:
        newMode = GanttHeader::days1;
        break;
    case 2:
        newMode = GanttHeader::hours1;
        break;
    case 3:
        newMode = GanttHeader::minutes1;
        break;
    case 4:
        newMode = GanttHeader::seconds1;
        break;
    default:
        return;
    }

    m_scene->setMode(newMode);


}

void GanttWidget::expanded(const QModelIndex &index)
{
    GanttInfoNode * node = m_model->nodeForIndex(index);
    node->setIsExpanded(true);

    return updatePos(node);
}

void GanttWidget::collapsed(const QModelIndex &index)
{
    GanttInfoNode * node = m_model->nodeForIndex(index);
    node->setIsExpanded(false);

    return updatePos(node);
}

void GanttWidget::onScrollGraphicsView(int value)
{
    m_scene->updateHeaderPos(value);
    m_scene->updateSliderRect();
}

void GanttWidget::updatePos(GanttInfoNode *from)
{
    m_curSceneMax = 0;

    GanttInfoNode* p_parent;

    while((p_parent=from->parent())!=m_model->m_root)
        from = from->parent();

    for(int i = m_model->m_root->indexOf(from); i < m_model->m_root->size(); ++i )
    {
        updatePosHelper(m_model->m_root->m_items[i]);
    }

    m_scene->updateHeight(m_curSceneMax);

    m_scene->update();
}

void GanttWidget::updatePosHelper(GanttInfoItem *item)
{
    GanttInfoLeaf *leaf = dynamic_cast<GanttInfoLeaf*>(item);
    if(leaf)
    {
        GanttItem *graphicsItem = m_scene->itemByInfo(leaf);

        graphicsItem->setPos(graphicsItem->scenePos().x(),2*DEFAULT_ITEM_WIDTH + leaf->pos());
        if(graphicsItem->rect().bottom()>m_curSceneMax)
            m_curSceneMax = graphicsItem->rect().bottom();
    }
    else
    {
        GanttInfoNode *node = dynamic_cast<GanttInfoNode*>(item);
        if(node)
        {
            foreach(GanttInfoItem* item, node->m_items)
                updatePosHelper(item);
        }
    }
}






void GanttWidget::on_pushButton_slider_clicked()
{
//    m_scene->slider()->setVisible(!(m_scene->slider()->isVisible()));

    if(UtcDateTime(m_scene->m_header->m_minDt).addSecs(13*SECONDS_IN_MINUTE) > UtcDateTime(m_scene->m_header->m_maxDt).addSecs(-13*SECONDS_IN_MINUTE))
        m_scene->setRange( UtcDateTime(m_scene->m_header->m_minDt).addSecs(SECONDS_IN_MINUTE/2)
                           ,UtcDateTime(m_scene->m_header->m_maxDt).addSecs(-SECONDS_IN_MINUTE/2) );
    else
        m_scene->setRange( UtcDateTime(m_scene->m_header->m_minDt).addSecs(13*SECONDS_IN_MINUTE)
                           ,UtcDateTime(m_scene->m_header->m_maxDt).addSecs(-13*SECONDS_IN_MINUTE) );
}

void GanttWidget::on_pushButton_header_clicked()
{
    if(m_scene->headerMode() == GanttHeader::TimelineMode)
        m_scene->setHeaderMode(GanttHeader::GanttDiagramMode);
    else
        m_scene->setHeaderMode(GanttHeader::TimelineMode);
}

void GanttWidget::repaintDtHeader()
{
    ui->treeView->repaintHeader();
}
