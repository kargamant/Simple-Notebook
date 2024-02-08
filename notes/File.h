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
        QString output(std::ostream& stream=std::cout);
        QString read();
        void write(const QString& newContent);
    };
}
#endif // FILE_H
