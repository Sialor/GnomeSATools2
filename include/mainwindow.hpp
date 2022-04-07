#pragma once

#include <QMainWindow>
#include <QVector>

#include "imgarchiver.hpp"
#include "ImgTree.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    //Q_OBJECT

    const QApplication& m_app;

    Ui::MainWindow& m_ui;

    ImgTree m_tree;
public:
    MainWindow(QApplication& app, QWidget *parent = nullptr);

    ~MainWindow();

    // setup default app language
    bool setupDefaultLang();

public slots:
    void pushBtn_pack();
    void pushBtn_unpack();
    void pushBtn_delete();
};