#ifndef DEVICESRVCFG_H
#define DEVICESRVCFG_H

#include <QWidget>

namespace Ui {
class DeviceSrvCfg;
}

class DeviceSrvCfg : public QWidget
{
    Q_OBJECT

public:
    explicit DeviceSrvCfg(QWidget *parent = nullptr);
    ~DeviceSrvCfg();

private slots:
    void on_pushButton_clicked();

private:
    Ui::DeviceSrvCfg *ui;
};

#endif // DEVICESRVCFG_H
