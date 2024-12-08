#pragma once
#include <string>


class FileTransmite
{
public:
	FileTransmite();
	~FileTransmite();

public:
	int TransmitStart(int fileSize,const std::string& MD5);
	int TransmitComment(char* pData, int size);
	int TransmitSucess(bool b = true);
	int TransmitFailer(bool b = true);

private:
	int m_file_fd;
	int m_file_size;
	std::string m_str_file_path;
	std::string m_str_file_md5;
};
