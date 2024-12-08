#ifndef DO_H
#define DO_H

#include <QWidget>
#include "DeviceCfgData.h"

namespace Ui {
class Do;
}

class Do : public QWidget,public DeviceCfgData
{
    Q_OBJECT

public:
    explicit Do(QWidget *parent = nullptr);
    ~Do();
public:
    virtual QJsonObject GetCfgJson();
    virtual void SetCfgJson(const QJsonObject& obj);
private:
    Ui::Do *ui;
};

#endif // DO_H
