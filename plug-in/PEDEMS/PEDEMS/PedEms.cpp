#include "PedEms.h"

extern "C" Device* CreateDevice(const std :: string & param)
{
	return new PedEms;
}