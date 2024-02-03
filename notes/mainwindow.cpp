#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QTextBrowser>
#include <iostream>
#include "File.h"
#include "CppHighlighter.h"
#include <fstream>
#include "FileTab.h"
#include <QWindow>
#include <QtQuick/qquickwindow.h>
#include <QMessageBox>

QString MainWindow::DEFAULT_PATH="D:\\qt_test_sources";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->openButton, &QPushButton::clicked, this, &MainWindow::open);
    connect(ui->newButton, &QPushButton::clicked, this, &MainWindow::newFile);
    connect(ui->saveButton, &QPushButton::clicked, this, &MainWindow::saveFile);
    connect(ui->saveAsButton, &QPushButton::clicked, this, &MainWindow::saveAsFile);
    connect(ui->closeButton, &QPushButton::clicked, this, &MainWindow::closeFile);
}

void MainWindow::open()
{
    QStringList files=QFileDialog::getOpenFileNames(this, "Choose a file", DEFAULT_PATH, "*.txt *.h *.cpp *.c *.py");
    std::cout<<"chosen files:"<<std::endl;
    for(auto itr: files)
    {
        File::FileTab* ft=new File::FileTab(Ui::binaryToQString(itr), this);

        //highlighting
        if(QUrl::fromLocalFile(ft->fileName()).fileName().endsWith(".cpp") || QUrl::fromLocalFile(ft->fileName()).fileName().endsWith(".h"))
        {
            QSyntaxHighlighter* highlighter=new Syntax::CppHighlighter(dynamic_cast<QTextEdit*>(ft)->document(), "..\\notes\\cpp.xml");

            dynamic_cast<File::File*>(ft)->highlighter=highlighter;
        }
        ui->FileTabs->addTab(dynamic_cast<QTextEdit*>(ft), QUrl::fromLocalFile(ft->fileName()).fileName());
        this->fileTabs.push_back(ft);
    }
}

void MainWindow::newFile()
{
    File::FileTab* ft=new File::FileTab("", this);
    fileTabs.push_back(ft);
    QString tabName="noname";

    if(lastCreatedTab!=0) tabName+=std::to_string(lastCreatedTab);
    ui->FileTabs->addTab(dynamic_cast<QTextEdit*>(ft), tabName);
    lastCreatedTab++;

}


void MainWindow::saveFile()
{
    if(!check("Error. Nothing to save.", fileTabs.empty()))
    {
        int i=ui->FileTabs->currentIndex();
        QWidget* curWidget=ui->FileTabs->currentWidget();
        if(fileTabs[i]->fileName()!="") fileTabs[i]->write(dynamic_cast<QTextEdit*>(curWidget)->toPlainText());
        else
        {
            saveAsFile();
        }
    }
}

void MainWindow::saveAsFile()
{
    if(!check("Error. Nothing to save as", fileTabs.empty()))
    {
        QWidget* curWidget=ui->FileTabs->currentWidget();
        QString path=QFileDialog::getSaveFileName(this, "choose directory", DEFAULT_PATH);
        File::File destination{path};
        destination.write(dynamic_cast<QTextEdit*>(curWidget)->toPlainText());
    }


    /*delete
    ui->FileTabs->removeTab(i);


    ui->FileTabs->insertTab()*/
}


int MainWindow::closeFile()
{
    if(!check("Error. Cant close tab. No tabs are opened.", fileTabs.empty()))
    {
        int i=ui->FileTabs->currentIndex();
        fileTabs.erase(fileTabs.begin()+i);
        ui->FileTabs->removeTab(i);
        return i;
    }
}

QString Ui::qStringOut(const QString& str, std::ostream& stream)
{
    QString latin_str;
    for(const QChar* qch=str.data(); qch-str.data()<str.length(); ++qch)
    {
        stream<<qch->toLatin1();
        latin_str+=qch->toLatin1();
    }
    stream<<std::endl;
    return latin_str;
}

QString Ui::binaryToQString(QString& str)
{
    QString latin_str;
    for(QChar* qch=str.data(); qch-str.data()<str.length(); ++qch)
    {
        latin_str+=qch->toLatin1();
    }
    return latin_str;
}


bool MainWindow::check(const QString& msg, bool condition)
{
    QMessageBox* box=nullptr;
    if(condition)
    {
        box=new QMessageBox();
        box->addButton("ok", QMessageBox::DestructiveRole);
        box->setAttribute(Qt::WA_DeleteOnClose);
        box->setText(msg);
        box->show();

    }
    return box!=nullptr;
}

MainWindow::~MainWindow()
{
    for(int i=0; i<fileTabs.size(); i++)
    {
        delete fileTabs[i];
        //delete files[i];
        //delete dynamic_cast<QTextEdit*>(ui->FileTabs->widget(i))->document();
        //delete ui->FileTabs->widget(i);
    }
    //delete ui;
}
