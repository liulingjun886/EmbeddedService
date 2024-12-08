#ifndef CAN_H
#define CAN_H

#include <QWidget>
#include "DeviceCfgData.h"

namespace Ui {
class Can;
}

class Can : public QWidget, public DeviceCfgData
{
    Q_OBJECT

public:
    explicit Can(QWidget *parent = nullptr);
    ~Can();
public:
    virtual QJsonObject GetCfgJson();
    virtual void SetCfgJson(const QJsonObject& obj);
private:
    Ui::Can *ui;
};

#endif // CAN_H
