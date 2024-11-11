#include "SysCfgSockItem.h"
#include "SysCfgModule.h"
#include <json/json.h>
#include <Log.h>

int SysCfgSockItem::onRecvCompelete(char* pData, UINT32 nLen)
{

	log_debug("SysCfgSockItem::onRecvCompelete");
	//校验长度
	if(nLen < 8)
		return 0;

	int nType = *((int*)&pData[0]);
	int nLengh = *((int*)&pData[4]);

	if(nLengh + 8 > nLen)
		return 0;

	//校验格式对不对暂定为 json 格式
	std::string strValue;
	strValue.append(&pData[8],nLengh);
	Json::Value root;
	Json::Reader reader;
	if(!reader.parse(strValue, root))
	{
		log_error("SysCfgDataFormat ERROR");
		return -1;
	}

	//处理逻辑
	SysCfgModule::GetInstance()->HandSysCfgMsg(this,nType,strValue);
	return nLengh + 8;
}
