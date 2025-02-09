#ifndef SYSCFG_H
#define SYSCFG_H

#include <QWidget>
#include "DeviceCfgData.h"
#include <QVector>

namespace Ui {
class SysCfg;
}

typedef struct stTypeSelect
{
    QString text;
    QString value;
    int     nSubDevType;        //设备管理窗口对应的类型
    stTypeSelect()
    {
        text = "";
        value = "";
        nSubDevType = -1;
    }
}TypeSelect;

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
    void on_cb_type_currentIndexChanged(int index);

private:
    int LoadModelInfo();

private:
    Ui::SysCfg *ui;
    QVector< QVector<TypeSelect> > m_vec_model_type_infos;
};

#endif // SYSCFG_H
