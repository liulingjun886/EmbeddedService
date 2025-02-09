#include "FileTransmit.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include "MD5.h"
#include "PublicTool.h"
#include <stdlib.h>
#include "Log.h"

#define UPDATE_DIR "uploaddir/"

FileTransmite::FileTransmite()
{
	if(!PublicTool::FileIsExists(PublicTool::GetProgramLocation() + UPDATE_DIR))
	{
		char mkdirCmd[256] = {0};
		sprintf(mkdirCmd,"mkdir -p %s%s",PublicTool::GetProgramLocation().c_str(), UPDATE_DIR);
		if(system(mkdirCmd))
			printf("create log file failer\n");
	}
	m_str_file_path = PublicTool::GetProgramLocation() + UPDATE_DIR;
}

FileTransmite::~FileTransmite()
{
	
}

int FileTransmite::TransmitStart(int fileSize,const std::string& MD5)
{
	std::string file = m_str_file_path + "trunk.zip";
	m_file_size = fileSize;
	m_str_file_md5 = MD5;
	m_file_fd = open(file.c_str(),O_RDWR | O_CREAT | O_APPEND | O_TRUNC, 0764);
	if(m_file_fd <= 0)
		return -1;
	return 0;
}

int FileTransmite::TransmitComment(char* pData, int size)
{
	if(m_file_fd <= 0)
		return -1;
	
	int nSize = ::write(m_file_fd,pData,size);
	if(nSize > 0)
		m_file_size -= nSize;
	else 
	{
		TransmitFailer();
		return -1;
	}

	if(0 == m_file_size)
	{
		if(TransmitSucess())
		{
			TransmitFailer(false);
			return -1;
		}
		return 0;
	}
	return m_file_size;
}

int FileTransmite::TransmitSucess(bool b)
{
	int ret = 0;
	if(b)
	{
		close(m_file_fd);
		m_file_fd = -1;
	}
	
	do{
		ifstream in(m_str_file_path + "trunk.zip");
		if(!in.is_open())
		{
			log_error("transmitfile open failer");
			ret = -1;
			break;
		}

		MD5 md5(in);
		if(md5.toString() != m_str_file_md5)
		{
			log_error("md5 wrong filemd5 = %s, md5 = %s",md5.toString().c_str(),m_str_file_md5.c_str());
			ret = -2;
			break;
		}

		char com[512] = {0};
		sprintf(com,"cd %s && tar -xvf trunk.zip && sh run.sh && cd -", m_str_file_path.c_str());
		int ret = 0;
		if(ret = system(com))
		{
			log_error("uploadfile tar error %d",ret);
		}
	}while(0);
	PublicTool::ExecShellCommand("rm -f " + m_str_file_path + "*");
	return ret;
}

int FileTransmite::TransmitFailer(bool b)
{
	if(b)
	{
		close(m_file_fd);
		m_file_fd = -1;
	}
	
	std::string command = "rm -f " + m_str_file_path + "*";
	if(system(command.c_str()))
		return 0;
	return 0;
}
