#include "ImgTree.hpp"

QTreeWidgetItem* ImgTree::add_item(QTreeWidgetItem* parent, QString name)
{
    QTreeWidgetItem* tmp = new QTreeWidgetItem(parent);

    tmp->setText(0, name);
    tmp->setIcon(0, QIcon(":/resources/file.png"));

    if (!parent)
    {
        m_imgTree = tmp;
        m_ui.treeWidget->insertTopLevelItem(0, m_imgTree);
    }
    else if (parent->icon(0).name() != ":/resources/folder.png")
    {
        parent->setIcon(0, QIcon(":/resources/folder.png"));
    }

    return tmp;
}

void ImgTree::fill_items()
{
    auto dirs = g_imgArchiver->m_dirs;
    auto dffs = g_imgArchiver->m_dffs;
    auto txds = g_imgArchiver->m_txds;

    add_item(nullptr, g_imgArchiver->get_filename());

    for (int index(0); index < dirs.count(); ++index)
    {
        if (QString(dirs.at(index)->m_name).right(3) == "txd")
        {
            auto parent = add_item(m_imgTree, QString(dirs.at(index)->m_name));

            for (auto texture : txds.at(index - dffs.count())->m_textures)
            {
                add_item(parent, QString(texture->m_textureData.m_textureName));
            }
        }
        else // dff
        {
            add_item(m_imgTree, QString(dirs.at(index)->m_name));
        }
    }
}