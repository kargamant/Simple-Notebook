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
    connect(ui->saveAllButton, &QPushButton::clicked, this, &MainWindow::saveAllFiles);
    connect(ui->closeAllButton, &QPushButton::clicked, this, &MainWindow::closeAllFiles);
    connect(ui->exitButton, &QPushButton::clicked, this, &MainWindow::exitApp);
}

void MainWindow::open()
{
    QStringList files=QFileDialog::getOpenFileNames(this, "Choose a file", DEFAULT_PATH, "*.txt *.h *.cpp *.c *.py");
    std::cout<<"chosen files:"<<std::endl;
    for(auto itr: files)
    {
        File::FileTab* ft=new File::FileTab(Ui::binaryToQString(itr), this);

        //highlighting
        /*if(QUrl::fromLocalFile(ft->fileName()).fileName().endsWith(".cpp") || QUrl::fromLocalFile(ft->fileName()).fileName().endsWith(".h"))
        {
            QSyntaxHighlighter* highlighter=new Syntax::CppHighlighter(dynamic_cast<QTextEdit*>(ft)->document(), "..\\notes\\cpp.xml");

            dynamic_cast<File::File*>(ft)->highlighter=highlighter;
        }*/
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
    saveFileByIndex(ui->FileTabs->currentIndex());
}

void MainWindow::saveFileByIndex(int i)
{
    if(!check("Error. Nothing to save.", fileTabs.empty()))
    {
        QWidget* iWidget=ui->FileTabs->widget(i);
        if(fileTabs[i]->fileName()!="") fileTabs[i]->write(dynamic_cast<QTextEdit*>(iWidget)->toPlainText());
        else
        {
            saveAsFileByIndex(i);
        }
    }
}

void MainWindow::saveAsFile()
{
    saveAsFileByIndex(ui->FileTabs->currentIndex());
}

void MainWindow::saveAsFileByIndex(int i)
{
    if(!check("Error. Nothing to save as", fileTabs.empty()))
    {
        QWidget* iWidget=ui->FileTabs->widget(i);
        QString path=QFileDialog::getSaveFileName(this, "choose directory to save "+ui->FileTabs->tabText(i), DEFAULT_PATH);
        if(!path.isEmpty())
        {
            File::File destination{path};
            destination.write(dynamic_cast<QTextEdit*>(iWidget)->toPlainText());
            closeFileByIndex(i);
            File::FileTab* ft=new File::FileTab(destination.fileName(), this);
            fileTabs.insert(fileTabs.begin()+i, ft);
            ui->FileTabs->insertTab(i, dynamic_cast<QWidget*>(ft), QUrl::fromLocalFile(ft->fileName()).fileName());
            ui->FileTabs->setCurrentIndex(i);
        }
    }
}

int MainWindow::closeFileByIndex(int i)
{
    if(!check("Error. Cant close tab. No tabs are opened.", fileTabs.empty()) && !check("Error. No tab with this index", i>=fileTabs.size()))
    {
        if(suggest("File was modified. Would you like to save it before closing?", fileTabs[i]->document()->isModified()))
        {
            saveFileByIndex(i);
        }
        fileTabs.erase(fileTabs.begin()+i);
        ui->FileTabs->removeTab(i);
        return i;
    }
}

int MainWindow::closeFile()
{
    return closeFileByIndex(ui->FileTabs->currentIndex());
}


void MainWindow::closeAllFiles()
{

    if(!check("Error. Cant close all tabs. No tabs are opened.", fileTabs.empty()))
    {
        for(int i=fileTabs.size()-1; i>=0; i--)
        {
            closeFileByIndex(i);
        }
    }
}

void MainWindow::saveAllFiles()
{
    if(!check("Error. Cant save all tabs. No tabs are opened.", fileTabs.empty()))
    {
        for(int i=fileTabs.size()-1; i>=0; i--)
        {
            saveFileByIndex(i);
        }
    }
}

void MainWindow::exitApp()
{
    if(fileTabs.empty()) close();
    /*else
    {

    }*/
}

bool MainWindow::suggest(const QString& msg, bool condition)
{
    if(condition)
    {

        QMessageBox* box=new QMessageBox(this);
        box->addButton("yes", QMessageBox::AcceptRole);
        box->addButton("no", QMessageBox::RejectRole);
        box->setText(msg);
        box->setAttribute(Qt::WA_DeleteOnClose);
        box->show();
        return !box->exec();
    }
    return false;
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
