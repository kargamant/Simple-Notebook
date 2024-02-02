#include "File.h"
#include "mainwindow.h"

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
        //if(!this->exists()) std::cout<<("Error. File with this filename does not exist");
        this->open(QIODeviceBase::ReadOnly);


        while(!this->atEnd())
        {
            content+=this->readLine();
            content+="\n";
        }
        this->close();
        return content;
    }


}
