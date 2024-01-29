#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
QString qStringOut(QString& str, std::ostream& stream=std::cout);
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

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
