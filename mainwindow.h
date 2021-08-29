#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QVector>

#include "imgarchiver.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

    QTreeWidgetItem* add_item(QTreeWidgetItem* parent, QString name);

    void fill_items();

    Ui::MainWindow* m_ui;

    QTreeWidgetItem* m_item;

public slots:
    void pushBtn_pack();
    void pushBtn_unpack();
    void pushBtn_delete();
};

#endif // MAINWINDOW_H
