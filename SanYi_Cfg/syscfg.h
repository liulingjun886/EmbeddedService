#ifndef SYSCFG_H
#define SYSCFG_H

#include <QWidget>
#include "DeviceCfgData.h"

namespace Ui {
class SysCfg;
}

class SysCfg : public QWidget,public DeviceCfgData
{
    Q_OBJECT

public:
    explicit SysCfg(QWidget *parent = nullptr);
    ~SysCfg();
public:
    virtual int HandNetMsg(int nType, int nLen, const char* pData);
    virtual void GetSrvData();

private slots:
    void on_pushButton_clicked();

private:
    Ui::SysCfg *ui;
};

#endif // SYSCFG_H
