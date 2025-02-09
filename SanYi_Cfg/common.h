#ifndef COMMON_H
#define COMMON_H

#include <QString>

class Common
{
public:
    Common();

    //获取文件内容
    static QByteArray GetFileContent(const QString &fileName);
    //更新文件内容
    static void UpdateFile(const QString &fileName, QByteArray data);
};

#endif // COMMON_H
