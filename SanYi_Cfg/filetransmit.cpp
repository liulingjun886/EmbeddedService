#include "filetransmit.h"
#include "ui_filetransmit.h"
#include <QFileDialog>
#include <QDebug>
#include <QFileInfo>
#include "StatusInfo.h"
#include <QJsonObject>
#include <QJsonDocument>
#include "MD5.h"
#include <QJsonDocument>
#include <QMessageBox>
#include <QCryptographicHash>
#include <QThread>

#define chunkSize 65520

FileTransmit::FileTransmit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FileTransmit)
{
    ui->setupUi(this);
}

FileTransmit::~FileTransmit()
{
    delete ui;
}

int FileTransmit::HandNetMsg(int nType, int nLen, const char* pData)
{
    switch(nType)
    {
        case FILE_TRANSMIT_START:
        {
            QByteArray com;
            com.append(pData,nLen);
            QJsonDocument doc = QJsonDocument::fromJson(com);
            QJsonObject obj = doc.object();
            if(0 == obj["ret"].toInt())
            {
                if(!StartSendFile())
                    SendFile();
            }
            return 0;
        }
        case FILE_TRANSMIT_COMMENT:
        {
            QByteArray com;
            com.append(pData,nLen);
            QJsonDocument doc = QJsonDocument::fromJson(com);
            QJsonObject obj = doc.object();
            int ret = obj["ret"].toInt();

            if(ret > 0)
            {
                ui->pb_fileupload->setValue(100 - (int)(ret*100.f / m_n_file_size));
                SendFile();
            }
            else if(0 == ret)
            {
                ui->pb_fileupload->setValue(100);
                QMessageBox::information(g_status_info.pMainDialog,"成功","文件传输成功");
                StopSendFile();
            }
            else
            {
                QMessageBox::information(g_status_info.pMainDialog,"失败","文件传输失败");
                StopSendFile();
            }

            return 0;
        }
        case FILE_TRANSMIT_FAILER:
        {
            StopSendFile();
            return 0;
        }
    }
    return 0;
}

void FileTransmit::on_btn_filechose_clicked()
{
    m_str_file_path = QFileDialog::getOpenFileName(nullptr,QObject::tr("open"),QDir::homePath(), QObject::tr("All Files (*)"));
    ui->edit_filepath->setText(m_str_file_path);
}

static QString calculateMd5(const QString &fileName) {
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        return "";
    }

    QCryptographicHash hash(QCryptographicHash::Md5);
    while (!file.atEnd()) {
        hash.addData(file.read(1024 * 1024)); // 每次读取1MB
    }
    file.close();

    return hash.result().toHex();
}

void FileTransmit::on_btn_upload_clicked()
{
    QJsonObject obj;
    QFileInfo fileInfo(m_str_file_path);

    QString filemd5 = calculateMd5(m_str_file_path);
    obj.insert("fileSize",fileInfo.size());
    obj.insert("MD5",filemd5);
    m_n_file_size = fileInfo.size();
    QByteArray arr;
    arr.fill(0,sizeof(NetMsg));
    NetMsg* pMsg = (NetMsg*)arr.data();
    pMsg->nType = FILE_TRANSMIT_START;
    QJsonDocument doc(obj);
    QByteArray data = doc.toJson(QJsonDocument::JsonFormat::Compact);
    pMsg->nLen = data.length();
    arr.append(data);
    SendData(arr);
}

int FileTransmit::StartSendFile()
{
    m_n_send_size = 0;
    m_upload_file = new QFile(m_str_file_path);
    if(!m_upload_file->open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(g_status_info.pMainDialog,"错误","请检查文件是否正确");
        return -1;
    }
    m_b_uploading = true;
    return 0;
}

void FileTransmit::StopSendFile()
{
    m_upload_file->close();
    delete m_upload_file;
    m_upload_file = nullptr;
    ui->pb_fileupload->setValue(0);
    m_b_uploading = false;
}

int FileTransmit::SendFile()
{
    if(!m_b_uploading)
        return 0;
    QByteArray buffer(65520+sizeof(NetMsg), 0);
    NetMsg* pMsg = (NetMsg*)buffer.data();
    pMsg->nType = FILE_TRANSMIT_COMMENT;
    qint64 bytesRead = m_upload_file->read(pMsg->data, chunkSize);
    pMsg->nLen = (int)bytesRead;
    m_n_send_size += bytesRead;
    if(SendData((char*)pMsg,bytesRead+8))
        return 0;
    return -1;
}


void FileTransmit::on_btn_cancel_clicked()
{
    if(QMessageBox::Yes == QMessageBox::question(this,"提示","您确认要取消上传吗?"))
    {
        QByteArray buffer(sizeof(NetMsg), 0);
        NetMsg* pMsg = (NetMsg*)buffer.data();
        pMsg->nType = FILE_TRANSMIT_FAILER;
        SendData((char*)pMsg,sizeof(NetMsg));
        return;
    }
}

