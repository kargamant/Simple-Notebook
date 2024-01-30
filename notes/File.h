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
        QSyntaxHighlighter* highlighter;
        File(const QString& filename) : QFile(filename) {}
        ~File()
        {
            delete highlighter;
        }
        //QString getFileName();
        QString output(std::ostream& stream=std::cout);
        QString read();
        //QString highlight();
    };
}
#endif // FILE_H
