#include "menu.hpp"

#include <QFileDialog>
#include <QApplication>

#include "confirmclose.hpp"
#include "mainwindow.hpp"
#include "ui_mainwindow.h"

extern ImgArchiver* g_imgArchiver;

extern MainWindow* g_mainWindow;

extern ConfirmClose* g_confirmClose;

Menu::Menu()
{
    createActions();
    createMenus(g_mainWindow->m_ui->menubar);

    saveAsAct->setShortcut(tr("CTRL+SHIFT+S"));
}

Menu::~Menu()
{
    delete quitAct;
    delete closeAct;
    delete saveAsAct;
    delete saveAct;
    delete openAct;
}

void Menu::createActions()
{
    openAct = new QAction(tr("&Открыть"), this);
    openAct->setShortcuts(QKeySequence::Open);
    connect(openAct, &QAction::triggered, this, &Menu::open);

    saveAct = new QAction(tr("&Сохранить"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    connect(saveAct, &QAction::triggered, this, &Menu::save);

    saveAsAct = new QAction(tr("&Сохранить как"), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    connect(saveAsAct, &QAction::triggered, this, &Menu::saveas);

    closeAct = new QAction(tr("&Закрыть"), this);
    closeAct->setShortcuts(QKeySequence::Close);
    connect(closeAct, &QAction::triggered, this, &Menu::close);

    quitAct = new QAction(tr("&Выйти"), this);
    quitAct->setShortcuts(QKeySequence::Quit);
    connect(quitAct, &QAction::triggered, this, &Menu::quit);
}

void Menu::createMenus(QMenuBar* menuBar)
{
    fileMenu = menuBar->addMenu(tr("&Файл"));
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addAction(closeAct);
    fileMenu->addSeparator();
    fileMenu->addAction(quitAct);
}

void Menu::open()
{
    QString fileName = QFileDialog::getOpenFileName(
                this, tr("Открыть"), "C:/", tr("Rage img file (*.img)"));

    if (fileName.isEmpty())
    {
        return;
    }

    g_imgArchiver->set_filename(fileName);

    if (!g_imgArchiver->is_empty())
    {
        g_imgArchiver->forget();
        g_mainWindow->m_ui->treeWidget->clear();
        g_mainWindow->m_item = nullptr;
    }

    g_imgArchiver->read();

    g_mainWindow->fill_items();
}

void Menu::save()
{
    g_imgArchiver->write();
}

void Menu::saveas()
{
    QString fileName = QFileDialog::getSaveFileName(
                this, tr("Сохранить как"), "C:/", tr("Rage img file (*.img)"));

    if (fileName.isEmpty())
    {
        return;
    }

    g_imgArchiver->set_filename(fileName);
    g_imgArchiver->write();
}

void Menu::close()
{
    g_confirmClose->show();
}

void Menu::quit()
{
    QApplication::quit();
}
