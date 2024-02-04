/*#ifndef FILETABSIGNALRECEIVER_H
#define FILETABSIGNALRECEIVER_H
#include <QObject>
#include "FileTab.h"

class FileTabSignalReceiver : public QObject
{
private:
    File::FileTab* ft; //underlying object
public:
    FileTabSignalReceiver(File::FileTab* ft) : ft(ft) {}
    void change() {ft->change();}
};

#endif // FILETABSIGNALRECEIVER_H
*/
