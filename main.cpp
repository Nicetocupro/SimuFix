#include "entrancewindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 展示入口界面
    entrancewindow entranceWindow;
    entranceWindow.show();

    return a.exec();
}
