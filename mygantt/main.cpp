#include "mygantt_widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GanttWidget w;
    w.show();

    return a.exec();
}