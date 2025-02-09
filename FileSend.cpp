#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <string.h>
#include <string>
#include <arpa/inet.h>
#include <iostream>
#include <fstream>


#define FILESIZE 3020800
#define PACKSIZE 65520

typedef struct stMsg
{
	int nType;
	int nLength;
	char data[0];
}Msg;

int main()
{
	int fd = socket(AF_INET,SOCK_STREAM,0);
	if(-1 == fd)
		return -1;

	sockaddr_in servaddr = {0};
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(8899);
	int n = inet_pton(AF_INET,"192.168.56.101",&servaddr.sin_addr);
	if(n <= 0)
		return -1;
	connect(fd, (sockaddr*)&servaddr, sizeof(servaddr));

	std::string msg = "{\"fileSize\":3020800,\"MD5\":\"f1bc93d877ad387a1bedd0d0ef275f8d\"}";
	char buff[65536] = {0};
	Msg* pMsg = (Msg*)buff;
	pMsg->nType = 1;
	pMsg->nLength = msg.length();
	memcpy(&pMsg->data,msg.c_str(),msg.length());
	
	send(fd,buff,msg.length()+8,0);
	memset(buff,0,sizeof(buff));
	int ret = recv(fd,buff,65536,0);
	printf("type = %d, length = %d, msg = %s\n",pMsg->nType, pMsg->nLength,pMsg->data);

	std::ifstream fin("./trunk.zip",std::ios::binary);
	if (fin.is_open() == false)
		printf("file open failer\n");
	
	int onread=0; // 每次调用 fin.read()时打算读取的字节数。 每次应搬砖头数。
	int totalbytes=0; // 从文件中已读取的字节总数。 已搬砖头数。
	while (true)
	{
		memset(buff,0,sizeof(buff));
		pMsg->nType = 2;
		// 计算本次应该读取的字节数，如果剩余的数据超过 4096 字节，就读 4096 字节。
		if (FILESIZE-totalbytes>PACKSIZE) 
			onread=PACKSIZE;
		else 
			onread=FILESIZE-totalbytes;

		pMsg->nLength = onread;
		// 从文件中读取数据。
		fin.read(pMsg->data,onread);
		// 把读取到的数据发送给对端。
		ret = send(fd,buff,onread+8,0);
		printf("ret = %d\n",ret);

		memset(buff,0,sizeof(buff));
		recv(fd,buff,65536,0);
		printf("type = %d, length = %d, msg = %s\n",pMsg->nType, pMsg->nLength,pMsg->data);
		
		// 计算文件已读取的字节总数，如果文件已读完，跳出循环。
		totalbytes=totalbytes+onread;
		if (totalbytes==FILESIZE) break;
	}

}

