#ifndef DI_H
#define DI_H

#include <QWidget>
#include "DeviceCfgData.h"

namespace Ui {
class Di;
}

class Di : public QWidget,public DeviceCfgData
{
    Q_OBJECT

public:
    explicit Di(QWidget *parent = nullptr);
    ~Di();
public:
    virtual QJsonObject GetCfgJson();
    virtual void SetCfgJson(const QJsonObject& obj);
private:
    Ui::Di *ui;
};

#endif // DI_H
