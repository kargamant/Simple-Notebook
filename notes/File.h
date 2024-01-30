#ifndef FILE_H
#define FILE_H
#include <QFile>
#include <iostream>

namespace File
{
    class File : public QFile
    {
    public:
        File(const QString& filename) : QFile(filename) {}
        //QString getFileName();
        QString output(std::ostream& stream=std::cout);
        QString read();
    };
}
#endif // FILE_H
