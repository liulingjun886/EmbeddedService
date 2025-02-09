#include "common.h"
#include <QFile>

Common::Common()
{

}

//获取文件内容
QByteArray Common::GetFileContent(const QString &fileName)
{
    QFile file(fileName);
    QByteArray file_byte;
    if (file.open(QIODevice::ReadOnly))
    {
        file_byte = file.readAll();
        file.close();
    }
    return file_byte;
}

//更新文件内容
void Common::UpdateFile(const QString &fileName, QByteArray data)
{
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly))
    {
        file.resize(0);
        file.write(data);
        file.flush();
        file.close();
    }
}
