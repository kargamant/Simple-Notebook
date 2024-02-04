#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include "FileTab.h"
#include <QTableWidget>

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

private:
    static QString DEFAULT_PATH;
    Ui::MainWindow *ui;
    std::vector<File::FileTab*> fileTabs;
    int lastCreatedTab=0;

    //button funcs
    void open();
    void newFile();
    void saveFile();
    void saveFileByIndex(int i);
    void saveAsFile();
    void saveAsFileByIndex(int i);
    int closeFile();
    int closeFileByIndex(int i);
    void closeAllFiles();
    void saveAllFiles();
    void exitApp();

    //msg boxes funcs
    bool check(const QString& msg, bool condition);
    bool suggest(const QString& msg, bool condition);
    std::pair<bool, QTableWidget*> exitSuggest(const QString& msg);

    //extra
    QTableWidget* formTable(bool (*criteria)(File::FileTab*));
    static bool isModified(File::FileTab* ft);
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};
#endif // MAINWINDOW_H
