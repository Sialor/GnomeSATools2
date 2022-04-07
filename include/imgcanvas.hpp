#ifndef IMGCANVAS_H
#define IMGCANVAS_H

#include <QLabel>
#include <QWheelEvent>
#include <QTreeWidgetItem>

class ImgCanvas : public QLabel
{
private:
    double m_mult; // multiplier

public:
    QTreeWidgetItem *m_item;

    QImage m_img;

    ImgCanvas();

    void wheelEvent(QWheelEvent *event);

    // texture in txd arch assign m_img, and assign m_item
    // переписать метод таким образом чтобы внутри не было обращений ко внешним объектам
    void show_texture();

    // The call setPixmap() and apply scale factor
    void show_image();
};

#endif // IMGCANVAS_H
