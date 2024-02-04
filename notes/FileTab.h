#ifndef FILETAB_H
#define FILETAB_H
#include "File.h"
#include <QTextEdit>

namespace File
{
    //wrapper for totality of file and tab
    class FileTab : public File, public QTextEdit
    {
    private:
        QTextDocument doc;

        void setUp();
    public:
        FileTab(const QString& filename, QWidget* parent) : File(filename), QTextEdit(parent), doc(QTextDocument())
        {
            setUp();
        }
        ~FileTab()
        {
            if(highlighter!=nullptr) delete highlighter;
        }
    };
}

#endif // FILETAB_H
