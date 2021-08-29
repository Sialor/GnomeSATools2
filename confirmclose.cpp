#include "confirmclose.h"
#include "ui_confirmclose.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QWidget>
#include <QApplication>

extern ImgArchiver* g_imgArchiver;

extern MainWindow* g_mainWindow;

ConfirmClose::ConfirmClose()
    : ui(new Ui::ConfirmClose)
{
    ui->setupUi(this);

    connect(ui->Yes, SIGNAL (clicked()), this, SLOT (yes()));
    connect(ui->No, SIGNAL (clicked()), this, SLOT (no()));
}

ConfirmClose::~ConfirmClose()
{
    delete ui;
}

void ConfirmClose::yes()
{
    if (!g_imgArchiver->is_empty())
    {
        g_imgArchiver->forget();
        g_mainWindow->m_ui->treeWidget->clear();
        g_mainWindow->m_item = nullptr;
    }

    this->hide();
}

void ConfirmClose::no()
{
    this->hide();
}
