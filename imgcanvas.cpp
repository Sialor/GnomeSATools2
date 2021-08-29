#include "imgcanvas.h"

#include "ui_mainwindow.h"

#include "imgarchiver.h"
#include "mainwindow.h"

extern ImgArchiver* g_imgArchiver;

extern MainWindow* g_mainWindow;

ImgCanvas::ImgCanvas()
{

}

void ImgCanvas::wheelEvent(QWheelEvent *event)
{
    // up
    int sign = pixmap()->width() * 0.25;

    //down
    if(event->angleDelta().y() < 0)
    {
        sign *= -1;
    }

    setPixmap(QPixmap::fromImage(m_img.scaledToWidth(pixmap()->width() + sign)));
}

void ImgCanvas::show_texture()
{
    auto items = g_mainWindow->m_ui->treeWidget->selectedItems();

    // number of texture which need to be show
    int indexOfTextureInTxd(0);

    for (int i(items.count() - 1); i >= 0; --i)
    {
        if (items.at(i)->text(0).right(3) == "txd")
        {
            indexOfTextureInTxd = 0;

            if (items.at(i)->childCount() < 1)
            {
                break;
            }
        }
        else if (items.at(i)->text(0).right(3) == "dff" || items.at(i)->text(0).right(3) == "img")
        {
            continue;
        }
        else
        {
            for (; indexOfTextureInTxd < items.at(i)->parent()->childCount()
                 && items.at(i)->parent()->child(indexOfTextureInTxd)->text(0) != items.at(i)->text(0); ++indexOfTextureInTxd)
            {}
        }

        unsigned int index;

        if (g_imgArchiver->get_index(items.at(i)->text(0), index) == false &&
                g_imgArchiver->get_index(items.at(i)->parent()->text(0), index) == false)
        {
            return;
        }

        TxdArchive &txd = *g_imgArchiver->m_txds.at(index - g_imgArchiver->m_dffs.count());

        TxdTextureData &data = txd.m_textures.at(indexOfTextureInTxd)->m_textureData;

        m_img = QImage(data.m_width, data.m_height, QImage::Format_ARGB32);

        for (int y = 0; y < m_img.height(); y++)
        {
            memcpy(m_img.scanLine(y), &data.m_data[y * m_img.bytesPerLine()], m_img.bytesPerLine());
        }

        setPixmap(QPixmap::fromImage(m_img));

        m_item = items.at(i);

        return;
    }
}
