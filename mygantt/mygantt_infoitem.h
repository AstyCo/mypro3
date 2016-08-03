#ifndef GANTTINFOABSTRACT_H
#define GANTTINFOABSTRACT_H

#include <QString>

class GanttInfoNode;

class GanttInfoItem
{
public:
    GanttInfoItem(GanttInfoNode *parent = NULL);

    GanttInfoNode *parent() const;

    virtual int columnCount() const = 0;

    QString title() const;
    void setTitle(const QString &title);

    int row() const;

    void setParent(GanttInfoNode *parent);

    virtual qreal height() const = 0;
    virtual qreal pos() const;

protected:
    virtual int indexOf(const GanttInfoItem* p_item) const;

private:
    GanttInfoNode* m_parent;

    QString m_title;
};

#endif // GANTTINFOABSTRACT_H
