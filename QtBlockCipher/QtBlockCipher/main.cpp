#include "QtBlockCipher.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtBlockCipher w;
    w.show();
    return a.exec();
}
