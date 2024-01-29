#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QTextBrowser>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->openButton, &QPushButton::clicked, this, &MainWindow::open);
}

void MainWindow::open()
{
    QStringList files=QFileDialog::getOpenFileNames(this, "Choose a file", "C:\\Users\\Honor", "*.txt *.cpp *.c *.py");
    std::cout<<"chosen files:"<<std::endl;
    for(auto itr: files)
    {
        QString filename;
        for(QChar* qch=itr.data(); qch-itr.data()<itr.length(); ++qch)
        {
            std::cout<<qch->toLatin1();
            //if(qch->toLatin1()=='/') filename+="\\";
            filename+=qch->toLatin1();
        }
        std::cout<<std::endl;

        QTextBrowser* qb=new QTextBrowser(this); //set to textedit

        qb->setSource(QUrl::fromLocalFile(filename));
        ui->FileTabs->addTab(qb, filename);
    }
}

MainWindow::~MainWindow()
{
    for(int i=0; i<ui->FileTabs->count(); i++) delete ui->FileTabs->widget(i);
    delete ui;
}
