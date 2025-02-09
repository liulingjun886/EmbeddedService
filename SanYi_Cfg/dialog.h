#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QTreeWidgetItem>
#include <QByteArray>

QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

public:
    int HandNetMsg(int nType, int nLen, const char* pData);
    int UpdateSubDeviceWidget();

private slots:
    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

    void on_stackedWidget_currentChanged(int arg1);

private:
    void InitTreeItem();
    QTreeWidgetItem * AddItem(QString text, int data);
    void AddSubItem(QTreeWidgetItem *item, QString text, int data);
    void OnDeviceManager(int nSubCode);
private:
    Ui::Dialog *ui;
};
#endif // DIALOG_H
