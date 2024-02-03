#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include "FileTab.h"

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
    static QString DEFAULT_PATH;
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void open();
    void newFile();
    void saveFile();
    void saveAsFile();
    int closeFile();
    bool check(const QString& msg, bool condition);
private:
    Ui::MainWindow *ui;
    std::vector<File::FileTab*> fileTabs;
    int lastCreatedTab=0;
};
#endif // MAINWINDOW_H
