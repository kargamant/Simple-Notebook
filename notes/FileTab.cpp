#include "FileTab.h"

namespace File
{

    void FileTab::setUp()
    {
        QString content=read();
        doc.setPlainText(content);
        setDocument(&doc);
    }

}
