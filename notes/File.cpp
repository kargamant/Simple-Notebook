#include "File.h"
#include "mainwindow.h"
#include <fstream>

namespace File
{
QString File::output(std::ostream& stream)
    {
        QString name=fileName();
        QString out=Ui::binaryToQString(name);
        out+="\ncontent:\n";
        out+=read();
        Ui::qStringOut(out, stream);
        return out;
    }
    QString File::read()
    {
        QString content;
        this->open(QIODeviceBase::ReadOnly);


        while(!this->atEnd())
        {
            content+=this->readLine();
            content+="\n";
        }
        this->close();
        return content;
    }


    void File::write(const QString& newContent)
    {
        std::ofstream file;
        file.open(fileName().toStdString());
        file<<newContent.toStdString();
        file.close();
    }

}
