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
#include <QTableWidget>
#include <QToolBar>

QString MainWindow::DEFAULT_PATH="D:\\qt_test_sources";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setUpMenu();

    connect(ui->openButton, &QPushButton::clicked, this, &MainWindow::open);
    connect(ui->newButton, &QPushButton::clicked, this, &MainWindow::newFile);
    connect(ui->saveButton, &QPushButton::clicked, this, &MainWindow::saveFile);
    connect(ui->saveAsButton, &QPushButton::clicked, this, &MainWindow::saveAsFile);
    connect(ui->closeButton, &QPushButton::clicked, this, &MainWindow::closeFile);
    connect(ui->saveAllButton, &QPushButton::clicked, this, &MainWindow::saveAllFiles);
    connect(ui->closeAllButton, &QPushButton::clicked, this, &MainWindow::closeAllFiles);
    connect(ui->exitButton, &QPushButton::clicked, this, &MainWindow::exitApp);
    connect(this, &MainWindow::closeSignal, this, &MainWindow::exitApp);
}

QMenu* MainWindow::setUpFileMenu()
{
    QMenu* fileMenu=new QMenu();

    QAction* newAction=new QAction();
    newAction->setText("New");
    QAction* openAction=new QAction();
    openAction->setText("Open");
    QAction* saveFileAction=new QAction();
    saveFileAction->setText("Save");
    QAction* saveAsFileAction=new QAction();
    saveAsFileAction->setText("SaveAs");
    QAction* closeFileAction=new QAction();
    closeFileAction->setText("closeFile");
    QAction* saveAllFilesAction=new QAction();
    saveAllFilesAction->setText("SaveAll");
    QAction* closeAllFilesAction=new QAction();
    closeAllFilesAction->setText("closeAll");
    QAction* exitAppAction=new QAction();
    exitAppAction->setText("exit");

    connect(newAction, &QAction::triggered, this, &MainWindow::newFile);
    connect(openAction, &QAction::triggered, this, &MainWindow::open);
    connect(saveFileAction, &QAction::triggered, this, &MainWindow::saveFile);
    connect(saveAsFileAction, &QAction::triggered, this, &MainWindow::saveAsFile);
    connect(closeFileAction, &QAction::triggered, this, &MainWindow::closeFile);
    connect(saveAllFilesAction, &QAction::triggered, this, &MainWindow::saveAllFiles);
    connect(closeAllFilesAction, &QAction::triggered, this, &MainWindow::closeAllFiles);
    connect(exitAppAction, &QAction::triggered, this, &MainWindow::exitApp);

    actions.push_back(newAction);
    actions.push_back(openAction);
    actions.push_back(saveFileAction);
    actions.push_back(saveAsFileAction);
    actions.push_back(closeFileAction);
    actions.push_back(saveAllFilesAction);
    actions.push_back(closeAllFilesAction);
    actions.push_back(exitAppAction);

    for(auto action: actions)
    {
        fileMenu->addAction(action);
    }

    menuSections.push_back(fileMenu);
    fileMenu->setGeometry(0, 0, 20, 20);
    fileMenu->setTitle("File");
    return fileMenu;
}

//QMenu* setUpEditMenu();
void MainWindow::setUpMenu()
{
    menu.addMenu(setUpFileMenu());
    menu.setGeometry(0, 0, 1600, 21);
    menu.setParent(this);
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    emit closeSignal();
    event->accept();
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
            fileTabs.erase(fileTabs.begin()+i);
            ui->FileTabs->removeTab(i);
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
        if(suggest("File "+ui->FileTabs->tabText(i)+" was modified. Would you like to save it before closing?", fileTabs[i]->document()->isModified()))
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
    else
    {
        std::pair<bool, QTableWidget*> answer=exitSuggest("Some files were modified, but not saved. Would you like to save them?");
        if(!answer.first)
        {
            close();
        }
        else
        {
            QTableWidget* table=answer.second;
            for(int i=0; i<table->rowCount(); i++)
            {
                int index=table->item(i, 2)->text().toInt();
                saveFileByIndex(index);
            }

            for(int i=0; i<table->rowCount(); i++)
            {
                for(int j=0; j<table->columnCount(); j++)
                {
                    delete table->item(i, j);
                }
            }
            delete table;
            close();
            /*for(int i=0; i<table->rowCount(); i++)
            {
                for(int j=0; j<table->columnCount(); j++)
                {
                    std::cout<<"item "<<i<<" "<<j<<": ";
                    Ui::qStringOut(table->item(i, j)->text());
                }
            }*/
        }
    }
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

std::pair<bool, QTableWidget*> MainWindow::exitSuggest(const QString& msg)
{
    QTableWidget* table=formTable(&MainWindow::isModified);
    if(table==nullptr) return {false, nullptr};
    QDialog* dialogue=new QDialog();
    dialogue->setParent(table);

    QPushButton* yes=new QPushButton(table);
    QPushButton* no=new QPushButton(table);

    yes->setText("yes");
    no->setText("no");

    QRect itemRect=table->visualItemRect(table->item(0, 0));
    yes->setGeometry(17, (table->rowCount()+1)*itemRect.height(), itemRect.width(), itemRect.height());
    no->setGeometry((table->columnCount()-1)*itemRect.width()+17, (table->rowCount()+1)*itemRect.height(), itemRect.width(), itemRect.height());

    connect(yes, &QPushButton::clicked, dialogue, &QDialog::accept);
    connect(no, &QPushButton::clicked, dialogue, &QDialog::reject);

    //dialogue->setAttribute(Qt::WA_DeleteOnClose);
    //yes->setAttribute(Qt::WA_DeleteOnClose);
    //no->setAttribute(Qt::WA_DeleteOnClose);
    table->show();

    bool result=dialogue->exec();
    //dialogue->close();
    //yes->close();
    //no->close();
    delete dialogue;
    delete yes;
    delete no;
    table->close();
    return {result, table};
}

QTableWidget* MainWindow::formTable(bool (*criteria)(File::FileTab*))
{
    QTableWidget* table=new QTableWidget();
    table->setColumnCount(3);
    table->setRowCount(fileTabs.size()+1);
    table->setHorizontalHeaderLabels({"Modified files", "paths", "index"});

    //table->setAttribute(Qt::WA_DeleteOnClose);
    int k=0;
    for(int i=0; i<fileTabs.size(); i++)
    {
        File::FileTab* ft=fileTabs[i];
        if(criteria(ft))
        {
            QTableWidgetItem* fileName=new QTableWidgetItem();
            fileName->setText(ui->FileTabs->tabText(i));

            QTableWidgetItem* filePath=new QTableWidgetItem();
            filePath->setText(ft->fileName());

            QTableWidgetItem* index=new QTableWidgetItem();
            index->setText(QString::fromStdString(std::to_string(i)));


            table->setItem(k, 0, fileName);
            table->setItem(k, 1, filePath);
            table->setItem(k, 2, index);
            //std::cout<<"colomn count: "<<table->columnCount()<<std::endl;
            k++;
        }
    }
    table->setRowCount(k);
    if(k==0)
    {
        delete table;
        table=nullptr;
    }
    return table;
}

bool MainWindow::isModified(File::FileTab* ft)
{
    return ft->document()->isModified();
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
    for(int i=0; i<actions.size(); i++) delete actions[i];
    for(int i=0; i<menuSections.size(); i++) delete menuSections[i];
    //delete ui;
}
