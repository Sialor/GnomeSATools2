#ifndef MENU_H
#define MENU_H

#include <QMenu>
#include <QAction>
#include <QMainWindow>

class Menu : public QWidget
{
    //Q_OBJECT

public:
    Menu();

    ~Menu();

private slots:
    void open();
    void save();
    void saveas();
    void close();
    void quit();

private:
    void createActions();
    void createMenus(QMenuBar* menuBar);

    QMenu *fileMenu;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *closeAct;
    QAction *quitAct;
};

#endif // MENU_H
