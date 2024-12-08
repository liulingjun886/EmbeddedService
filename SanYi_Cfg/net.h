#ifndef NET_H
#define NET_H

#include <QWidget>
#include "DeviceCfgData.h"

namespace Ui {
class Net;
}

class Net : public QWidget,public DeviceCfgData
{
    Q_OBJECT

public:
    explicit Net(QWidget *parent = nullptr);
    ~Net();
public:
    virtual QJsonObject GetCfgJson();
    virtual void SetCfgJson(const QJsonObject& obj);
private:
    Ui::Net *ui;
};

#endif // NET_H
