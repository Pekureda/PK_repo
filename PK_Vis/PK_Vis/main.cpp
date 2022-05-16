#include "pk_vis.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PK_Vis w;
    w.show();
    return a.exec();
}
