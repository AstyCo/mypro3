#include "mygantt_headerview.h"

#include "mygantt_widget.h"


#include <QPaintEvent>
#include <QPainter>

#include <QFontMetrics>

#include <QDebug>


#define HEADERVIEW_MARGIN 5


GanttHeaderView::GanttHeaderView(Qt::Orientation orientation, QWidget * parent) :
    QHeaderView(orientation,parent)
{

}

void GanttHeaderView::paintEvent(QPaintEvent *e)
{
    qDebug() << "GanttHeaderView::paintEvent";



//    if(e->rect() != rect())
//    {
//        // Don't draw columns

//        qDebug() << e->rect();

//        e->accept();
//        QPaintEvent ev(rect());
//        paintEvent(&ev);
//        return;
//    }
//    QHeaderView::paintEvent(e);

    static GanttWidget* p_parentWidget = NULL;
    if(!p_parentWidget)
    {
        qDebug() << parentWidget()
                 << parentWidget()->parentWidget();

        if(parentWidget()
                && parentWidget()->parentWidget())
            if(!(p_parentWidget = dynamic_cast<GanttWidget*>(parentWidget()
                                                             ->parentWidget()
                                                             ->parentWidget())))
                return;
    }


    QPainter painter;

    if(!painter.begin(viewport()) && !painter.begin(this))
    {
        qDebug() << "GanttHeaderView::paintEvent painter initializing failed";
        return;
    }

    qDebug() <<"viewport "<< viewport();

    QFontMetrics fmetrics(painter.font(),painter.device());


    QRect utcRect = fmetrics.tightBoundingRect("UTC").adjusted(-3,-3,3,3);

    qDebug() << "utcRect" << utcRect;

    QRect dtRect = rect().adjusted(0,0, - utcRect.width() - HEADERVIEW_MARGIN , 0);

    utcRect.moveTop((dtRect.bottom() - utcRect.height())/2);
    utcRect.moveLeft(dtRect.right() - HEADERVIEW_MARGIN);

    qDebug() << "dtRect" << dtRect
             << "utcRect" << utcRect;

    painter.fillRect(rect(),QBrush(Qt::cyan));

    painter.drawText(dtRect,p_parentWidget->slidersDt().toString(),QTextOption(Qt::AlignCenter));

    painter.drawRect(utcRect);
    painter.drawText(utcRect,"UTC",QTextOption(Qt::AlignCenter));

    e->accept();

}

void GanttHeaderView::paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const
{
    //
    qDebug() << "GanttHeaderView::paintSection" ;
}

bool GanttHeaderView::event( QEvent *e)
{
    qDebug() <<"event "
            << e;
    return QHeaderView::event(e);
}

void GanttHeaderView::update()
{
    QPaintEvent ev(rect());
    paintEvent(&ev);



    QHeaderView::update();
}

