#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include "File.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
QString qStringOut(const QString& str, std::ostream& stream=std::cout);
QString binaryToQString(QString& str);
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void open();
    void newFile();
    void saveFile();
private:
    Ui::MainWindow *ui;
    std::vector<File::File*> files;
    int lastCreatedTab=0;
};
#endif // MAINWINDOW_H
