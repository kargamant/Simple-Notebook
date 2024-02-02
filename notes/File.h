#ifndef FILE_H
#define FILE_H
#include <QFile>
#include <iostream>
#include <QSyntaxHighlighter>

namespace File
{
    class File : public QFile
    {
    public:
        QSyntaxHighlighter* highlighter=nullptr;
        File(const QString& filename) : QFile(filename) {}
        ~File()
        {
            if(highlighter!=nullptr) delete highlighter;
        }
        //QString getFileName();
        QString output(std::ostream& stream=std::cout);
        QString read();
        //QString highlight();
    };
}
#endif // FILE_H
