#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QTextBrowser>
#include <iostream>
#include "File.h"
#include "CppHighlighter.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->openButton, &QPushButton::clicked, this, &MainWindow::open);
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
