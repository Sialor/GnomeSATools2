#ifndef IMGCANVAS_H
#define IMGCANVAS_H

#include <QLabel>
#include <QWheelEvent>
#include <QTreeWidgetItem>

class ImgCanvas : public QLabel
{
public:
    QTreeWidgetItem *m_item;

    QImage m_img;

    ImgCanvas();

    void wheelEvent(QWheelEvent *event);

    // texture in txd arch assign m_img, and assign m_item
    void show_texture();
};

#endif // IMGCANVAS_H
