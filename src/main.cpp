/*
    Не поддерживаются mipmap
    Не поддерживаются отличные от глубины в 32 бит изображения
    Не все форматы изображений при импортировании поддерживаются
    Не поддерживается палитра, только при глубине в 8 бит
*/

#include "mainwindow.hpp"

#include <QApplication>

// extern const
extern const unsigned int g_size_of_sector(2048);

extern const unsigned int g_rw_version(0x1803FFFF);



int main(int argc, char *argv[])
{
    //    Q_INIT_RESOURCE(icons);

    QApplication app(argc, argv);
    
    MainWindow mainWindow(app);
    mainWindow.setupDefaultLang();
    mainWindow.show();

    return app.exec();
}