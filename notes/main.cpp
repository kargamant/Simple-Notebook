#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    try
    {
        a.exec();
    }
    catch(std::invalid_argument error)
    {
        std::cout<<error.what()<<std::endl;
    }
    return 0;
}
