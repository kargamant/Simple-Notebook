#include "FileTab.h"
#include "CppHighlighter.h"
#include "FileTabSignalReceiver.h"

namespace File
{

    void FileTab::setUp()
    {
        QString content=read();
        doc.setPlainText(content);
        setDocument(&doc);
        if(QUrl::fromLocalFile(fileName()).fileName().endsWith(".cpp") || QUrl::fromLocalFile(fileName()).fileName().endsWith(".h"))
        {
            QSyntaxHighlighter* highlighter=new Syntax::CppHighlighter(&doc, "..\\notes\\cpp.xml");

            this->highlighter=highlighter;
        }
        doc.setModified(false);
    }

}
