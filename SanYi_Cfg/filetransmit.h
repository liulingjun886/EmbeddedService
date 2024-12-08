#ifndef FILETRANSMIT_H
#define FILETRANSMIT_H

#include <QWidget>
#include <QString>
#include "DeviceCfgData.h"
#include <QFile>

namespace Ui {
class FileTransmit;
}

class FileTransmit : public QWidget, public DeviceCfgData
{
    Q_OBJECT

public:
    explicit FileTransmit(QWidget *parent = nullptr);
    ~FileTransmit();


    int HandNetMsg(int nType, int nLen, const char* pData);
private slots:
    void on_btn_filechose_clicked();

    void on_btn_upload_clicked();

    void on_btn_cancel_clicked();

private:
    int StartSendFile();
    void StopSendFile();
    int SendFile();
private:
    Ui::FileTransmit *ui;
    QString m_str_file_path;
    QFile* m_upload_file;
    bool m_b_uploading;
    int m_n_file_size;//文件大小
    int m_n_send_size;//已经发送的大小
};

#endif // FILETRANSMIT_H
