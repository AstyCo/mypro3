#include "mygantt_treedelegate.h"

#include "utcdatetime.h"


#include <QDateTimeEdit> //

#include <QDebug>

GanttTreeDelegate::GanttTreeDelegate(QObject *parent) :
    QItemDelegate(parent)
{
    qDebug() << "GanttTreeDelegate";

}

QWidget *GanttTreeDelegate::createEditor(QWidget *parent,
                                             const QStyleOptionViewItem &/*option*/,
                                             const QModelIndex &/*index*/) const
{
    qDebug() << "GanttTreeDelegate::createEditor";

    QDateTimeEdit *editor = new QDateTimeEdit(parent);
    editor->setCalendarPopup(true);
    return editor;

}

void GanttTreeDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{

    qDebug() << "GanttTreeDelegate::setEditorData";


    qDebug() << index.model()->data(index, Qt::EditRole);

    QDateTime value = index.model()->data(index, Qt::EditRole).toDateTime();
    QDateTimeEdit *dtEdit = static_cast<QDateTimeEdit*>(editor);
    dtEdit->setDateTime(value);

}

void GanttTreeDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{

    qDebug() << "GanttTreeDelegate::setModelData";

    QDateTimeEdit *dtEdit = static_cast<QDateTimeEdit*>(editor);
    QDateTime value = dtEdit->dateTime();
    model->setData(index, value, Qt::EditRole);

}

void GanttTreeDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const
{
    qDebug() << "GanttTreeDelegate::updateEditorGeometry";

    editor->setGeometry(option.rect);

}

void GanttTreeDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
//    qDebug() << "GanttTreeDelegate::paint";
    QItemDelegate::paint(painter,option,index);
}
