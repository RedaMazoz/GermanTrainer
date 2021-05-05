#include <QApplication>
#include "GermanTrainer.h"

int main(int argc, char* argv[])
{
    QApplication app(argc,argv);
    GermanTrainer germanApp;
    germanApp.show();
    return app.exec();
}
