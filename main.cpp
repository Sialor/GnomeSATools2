/*
    Не поддерживаются mipmap
    Не поддерживаются отличные от глубины в 32 бит изображения
    Не все форматы изображений при импортировании поддерживаются
    Не поддерживается палитра, только при глубине в 8 бит
*/

#include "menu.h"
#include "mainwindow.h"
#include "confirmclose.h"
#include "imgarchiver.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>



// extern const
extern const unsigned int g_size_of_sector(2048);

extern const unsigned int g_rw_version(0x1803FFFF);


// extern
ImgArchiver* g_imgArchiver;

MainWindow* g_mainWindow;

Menu* g_menu;

ConfirmClose* g_confirmClose;



int main(int argc, char *argv[])
{
//    Q_INIT_RESOURCE(icons);

    QApplication a(argc, argv);

    QTranslator translator;

    const QStringList uiLanguages = QLocale::system().uiLanguages();

    for (const QString &locale : uiLanguages)
    {
        const QString baseName = "GnomeSATools_" + QLocale(locale).name();

        if (translator.load(":/i18n/" + baseName))
        {
            a.installTranslator(&translator);
            break;
        }
    }

    g_imgArchiver = new ImgArchiver();
    g_mainWindow = new MainWindow();

    g_menu = new Menu();

    g_confirmClose = new ConfirmClose();

    g_mainWindow->show();

    return a.exec();
}
