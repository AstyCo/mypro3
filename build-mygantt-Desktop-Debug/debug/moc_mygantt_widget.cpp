/****************************************************************************
** Meta object code from reading C++ file 'mygantt_widget.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../mygantt/mygantt_widget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mygantt_widget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_GanttWidget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      19,   13,   12,   12, 0x08,
      61,   13,   12,   12, 0x08,
      83,   13,   12,   12, 0x08,
     112,  106,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_GanttWidget[] = {
    "GanttWidget\0\0index\0"
    "on_comboBox_mode_currentIndexChanged(int)\0"
    "expanded(QModelIndex)\0collapsed(QModelIndex)\0"
    "value\0onScrollGraphicsView(int)\0"
};

void GanttWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        GanttWidget *_t = static_cast<GanttWidget *>(_o);
        switch (_id) {
        case 0: _t->on_comboBox_mode_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->expanded((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 2: _t->collapsed((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 3: _t->onScrollGraphicsView((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData GanttWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject GanttWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_GanttWidget,
      qt_meta_data_GanttWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &GanttWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *GanttWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *GanttWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GanttWidget))
        return static_cast<void*>(const_cast< GanttWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int GanttWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
