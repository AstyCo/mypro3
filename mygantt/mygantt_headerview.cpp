#include "mygantt_headerview.h"

#include "mygantt_widget.h"
#include "mygantt_treeview.h"

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
    static GanttWidget* p_parentWidget = NULL;
    if(!p_parentWidget)
    {
        if(parentWidget()
                && parentWidget()->parentWidget())
            if(!(p_parentWidget = dynamic_cast<GanttWidget*>(parentWidget()
                                                             ->parentWidget()
                                                             ->parentWidget())))
                return;
    }


    QPainter painter(viewport());

    QPen headerPen(QBrush(Qt::white)
                   ,1,Qt::SolidLine,Qt::SquareCap,Qt::MiterJoin);
    QFont dtFont("Goudy Old Style", 10,QFont::DemiBold),
            utcFont("Goudy Old Style", 12,QFont::DemiBold);

    QFontMetrics fmetrics(utcFont,painter.device());


    QRect utcRect = fmetrics.tightBoundingRect("UTC").adjusted(-3,-3,3,3);

    QRect dtRect = rect().adjusted(0,0, - utcRect.width() - HEADERVIEW_MARGIN , 0);

    utcRect.moveTop((dtRect.bottom() - utcRect.height())/2);
    utcRect.moveLeft(dtRect.right() - HEADERVIEW_MARGIN);

    painter.fillRect(rect(),QBrush(Qt::blue));

    painter.setPen(headerPen);

    painter.setFont(dtFont);
    painter.drawText(dtRect,p_parentWidget->slidersDt().toString("dd.MM.yyyy HH:mm:ss"),QTextOption(Qt::AlignCenter));

    painter.drawRect(utcRect);  
    painter.setFont(utcFont);
    painter.drawText(utcRect,"UTC",QTextOption(Qt::AlignCenter));

    e->accept();

}



