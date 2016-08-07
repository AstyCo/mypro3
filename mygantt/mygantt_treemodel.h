#ifndef GANTTTREEMODEL_H
#define GANTTTREEMODEL_H

#include "mygantt_infoleaf.h"
#include "mygantt_infonode.h"

#include <QAbstractItemModel>

class GanttWidget;

class GanttTreeModel : public QAbstractItemModel
{

public:
    enum Field
    {
        nameField,
        startField,
        finishField,
        durationField,

        Fields_count

    };


public:
    GanttTreeModel(GanttInfoNode *root,QObject * parent = 0);

    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    void addItems(const QList<GanttInfoItem*>& items);
    void addItems(GanttInfoItem* item);


    QModelIndex indexForItem(const GanttInfoItem* item) const;


private:
    GanttInfoItem* itemForIndex(const QModelIndex& index) const;
    GanttInfoLeaf* leafForIndex(const QModelIndex& index) const;
    GanttInfoNode* nodeForIndex(const QModelIndex& index) const;

    void initIndexes(GanttInfoItem* item);


private:

    GanttInfoNode *m_root;

    friend class GanttWidget;

};

#endif // GANTTTREEMODEL_H
