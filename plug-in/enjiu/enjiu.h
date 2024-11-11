#include "PCSDevice.h"
#include "core/ASyncCallSupport.h"
#include "core/TimeEventHander.h"
#include "core/TimeNode.h"
#include "json/json.h"

class enjiu : public PCSDevice, public CAsyncCallSupport, public CTimeEventHander
{
public:
	enjiu();
	~enjiu();
public:
	int Initialize(Json::Value & value);
	
};
