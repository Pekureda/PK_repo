#include "PK_Steganography.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PK_Steganography w;
    w.show();
    return a.exec();
}
