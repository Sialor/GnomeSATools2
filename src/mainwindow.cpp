#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include <QTranslator>
#include <QApplication>
#include <QFileDialog>

#include "menu.hpp"
#include "imgcanvas.hpp"

extern ImgArchiver* g_imgArchiver;

extern Menu* g_menu;

MainWindow::MainWindow(QApplication& app, QWidget *parent)
    : m_app(app),
      QMainWindow(parent),
      m_ui(*(new Ui::MainWindow)),
      m_imgTree(nullptr)
{
    m_ui.setupUi(this);

    ImgCanvas *tmpCanvas = new ImgCanvas();
    tmpCanvas->setObjectName("imgCanvas");

    m_ui.splitter->addWidget(tmpCanvas);

    connect(m_ui.pushButton_pack, &QPushButton::clicked, this, &MainWindow::pushBtn_pack);
    connect(m_ui.pushButton_unpack, &QPushButton::clicked, this, &MainWindow::pushBtn_unpack);
    connect(m_ui.pushButton_delete, &QPushButton::clicked, this, &MainWindow::pushBtn_delete);

    connect(m_ui.treeWidget, &QTreeWidget::itemSelectionChanged, tmpCanvas, &ImgCanvas::show_texture);

    m_ui.treeWidget->setHeaderLabel(tr("Имя"));
    
    m_ui.pushButton_pack->setIcon(QIcon(":/new/prefix1/pushbutton_pack.png"));
    m_ui.pushButton_unpack->setIcon(QIcon(":/new/prefix1/pushbutton_unpack.png"));
    m_ui.pushButton_delete->setIcon(QIcon(":/new/prefix1/pushbutton_delete.png"));
    
    m_ui.pushButton_pack->setToolTip("Запаковать");
    m_ui.pushButton_unpack->setToolTip("Распаковать");
    m_ui.pushButton_delete->setToolTip("Удалить");
}

MainWindow::~MainWindow()
{
    if (g_menu != nullptr)
    {
        delete g_menu;

        g_menu = nullptr;
    }
}

bool MainWindow::setupDefaultLang()
{
    QTranslator translator;

    const QStringList uiLanguages = QLocale::system().uiLanguages();

    for (const QString& locale : uiLanguages)
    {
        const QString baseName = "GnomeSATools_" + QLocale(locale).name();

        if (translator.load(":/i18n/" + baseName))
        {
            if (m_app.installTranslator(&translator))
            {
                return true;
            }
        }
    }

    return false;
}

void MainWindow::pushBtn_pack()
{
    auto items = m_ui.treeWidget->selectedItems();

    if (items.count() != 1)
    {
        // предупреждение что выбран не один объект

        return;
    }

    QString fileName;

    if (items[0]->text(0).right(3) == "img")
    {
        fileName = QFileDialog::getOpenFileName(
                    this, tr("Добавить"), "C:/", tr("Model/Texture file (*.dff *.txd)"));

        if (fileName == "")
        {
            return;
        }

        // подготавливаем данные

        // ищем в imgarch место на которое добавим данные
    }
    else if (items[0]->text(0).right(3) == "txd")
    {
        fileName = QFileDialog::getOpenFileName(
                    this, tr("Добавить"), "C:/", tr("Image Files (*.png *.jpg *.bmp)"));

        if (fileName == "")
        {
            return;
        }

        // подготавливаем данные

        // ищем в imgarch место на которое добавим данные
        unsigned int index;

        if (!g_imgArchiver->get_index(items.at(0)->text(0), index))
        {
            return;
        }

        // показать окно с вводом названия файла

        TxdArchive &arch = *g_imgArchiver->m_txds.at(index - g_imgArchiver->m_dffs.count());

        arch.import_texture(fileName);

        add_item(items[0], arch.m_textures.last()->m_textureData.m_textureName);
    }
    else //dff
    {
        return;
    }
}

void MainWindow::pushBtn_unpack()
{
    auto items = m_ui.treeWidget->selectedItems();

    if (items.count() == 0)
    {
        // предупреждение не выбрано ни один item

        return;
    }

    QString folderName;

    folderName = QFileDialog::getExistingDirectory(this, tr("Распаковать"), "C:/", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (folderName == "")
    {
        return;
    }

    bool isOnlyImg(false);

    for (auto item : items)
    {
        if (item->text(0).right(3) == "img")
        {
            isOnlyImg = true;
            break;
        }
    }

    // https://russianblogs.com/article/5317924515/
    if (isOnlyImg)
    {
        for (int i(0); i < m_imgTree->childCount(); ++i)
        {
            QString name = m_imgTree->child(i)->text(0);
            g_imgArchiver->unpack(name, folderName);
        }

        return;
    }

    // else if is not img
    for (auto item : items)
    {
        if (item->text(0).right(3) == "txd")
        {
            g_imgArchiver->unpack(item->text(0), folderName);
        }
        else // if this is the texture
        {
            ((ImgCanvas*)m_ui.splitter->widget(1))->m_img.save(folderName + "/" + item->text(0) + ".png", "PNG");
        }
    }
}

void MainWindow::pushBtn_delete()
{
    auto items = m_ui.treeWidget->selectedItems();

    for (auto item : items)
    {
        if (item->text(0).right(3) == "txd")
        {
            g_imgArchiver->delete_file(item->text(0));
            item->parent()->removeChild(item);
            delete item;
        }
        else if (item->text(0).right(3) == "img")
        {
            return;
        }
        else
        {
            unsigned int index;
            g_imgArchiver->get_index(item->parent()->text(0), index);
            g_imgArchiver->m_txds.at(index - g_imgArchiver->m_dffs.count())->delete_texture(item->text(0));
            item->parent()->removeChild(item);
            delete item;
        }
    }
}