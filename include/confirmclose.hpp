#ifndef CONFIRMQUIT_H
#define CONFIRMQUIT_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class ConfirmClose; }
QT_END_NAMESPACE

class ConfirmClose : public QDialog
{
private:
    //Q_OBJECT

public:
    Ui::ConfirmClose *ui;

    ConfirmClose();

    ~ConfirmClose();

public slots:
    void yes();

    void no();

};

#endif // CONFIRMQUIT_H
