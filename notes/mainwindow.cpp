#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QTextBrowser>
#include <iostream>
#include "File.h"
#include "CppHighlighter.h"
#include <fstream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->openButton, &QPushButton::clicked, this, &MainWindow::open);
    connect(ui->newButton, &QPushButton::clicked, this, &MainWindow::newFile);
    connect(ui->saveButton, &QPushButton::clicked, this, &MainWindow::saveFile);
    connect(ui->saveAsButton, &QPushButton::clicked, this, &MainWindow::saveAsFile);
}

void MainWindow::open()
{
    QStringList files=QFileDialog::getOpenFileNames(this, "Choose a file", "C:\\Users\\Honor", "*.txt *.h *.cpp *.c *.py");
    std::cout<<"chosen files:"<<std::endl;
    for(auto itr: files)
    {
        QString filename=Ui::binaryToQString(itr);
        File::File* file=new File::File{filename};
        //debug output
        //file->output();

        //perhaps uneffective way to read file, but gonna try
        QTextDocument* doc=new QTextDocument();
        QString content=file->read();
        doc->setPlainText(content);

        QTextEdit* qte=new QTextEdit(this);
        qte->setDocument(doc);

        //highlighting
        if(QUrl::fromLocalFile(filename).fileName().endsWith(".cpp") || QUrl::fromLocalFile(filename).fileName().endsWith(".h"))
        {
            QSyntaxHighlighter* highlighter=new Syntax::CppHighlighter(qte->document(), "..\\notes\\cpp.xml");

            file->highlighter=highlighter;
        }
        ui->FileTabs->addTab(qte, QUrl::fromLocalFile(filename).fileName());
        this->files.push_back(file);
    }
}

void MainWindow::newFile()
{
    //obeying the rule: One tab <=> one file.
    //Tab cant exist without file, even empty or non created yet
    File::File* file=new File::File("");
    QTextDocument* doc=new QTextDocument();
    QTextEdit* qte=new QTextEdit(this);
    qte->setDocument(doc);
    files.push_back(file);
    QString tabName="noname";

    if(lastCreatedTab!=0) tabName+=std::to_string(lastCreatedTab);
    ui->FileTabs->addTab(qte, tabName);
    lastCreatedTab++;

    /*std::fstream file;
    file.open("../noname.txt");
    if(!file)
    {
        file.open("../noname.txt", std::ios::out);
        file.close();
    }
    else
    {
        file.close();
    }*/
}


void MainWindow::saveFile()
{
    int i=ui->FileTabs->currentIndex();
    QWidget* curWidget=ui->FileTabs->currentWidget();
    if(files[i]->fileName()!="") files[i]->write(dynamic_cast<QTextEdit*>(curWidget)->toPlainText());
    else
    {
        saveAsFile();
    }
}

void MainWindow::saveAsFile()
{
    int i=ui->FileTabs->currentIndex();
    QWidget* curWidget=ui->FileTabs->currentWidget();
    QFileDialog::saveFileContent(dynamic_cast<QTextEdit*>(curWidget)->toPlainText().toLatin1(), files[i]->fileName());
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

MainWindow::~MainWindow()
{
    for(int i=0; i<ui->FileTabs->count(); i++)
    {
        delete files[i];
        delete dynamic_cast<QTextEdit*>(ui->FileTabs->widget(i))->document();
        delete ui->FileTabs->widget(i);
    }
    delete ui;
}
