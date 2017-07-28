// nolo_driver.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "nolo_driver.h"
#include "NOLOServerDriver.h"
#include "NOLOClientDriver.h"
#include <openvr_driver.h>
static NOLOServerDriver g_ServerDriverNOLO;
static NOLOClientDriver g_ClientDriverNOLO;

HMD_DLL_EXPORT void * HmdDriverFactory(const char * pInterfaceName, int * pReturnCode)
{
	if (0 == strcmp(vr::IServerTrackedDeviceProvider_Version, pInterfaceName))
	{

		return &g_ServerDriverNOLO;
	}
	if (0 == strcmp(vr::IClientTrackedDeviceProvider_Version, pInterfaceName))
	{

		return &g_ClientDriverNOLO;
	}

	if (pReturnCode)
	{

		*pReturnCode = vr::VRInitError_Init_InterfaceNotFound;
	}

	return NULL;
}
