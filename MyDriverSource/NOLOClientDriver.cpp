#include "NOLOClientDriver.h"
#include "driverlog.h"


NOLOClientDriver::NOLOClientDriver()
{
}


NOLOClientDriver::~NOLOClientDriver()
{
}

vr::EVRInitError NOLOClientDriver::Init(vr::EClientDriverMode eDriverMode, vr::IDriverLog * pDriverLog, vr::IClientDriverHost * pDriverHost, const char * pchUserDriverConfigDir, const char * pchDriverInstallDir)
{
	InitDriverLog(pDriverLog);
	DriverLog(">>>>>>>>>>>>>>>>>>>>>>>>IClientTrackedDeviceProvider Init<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");

	m_pDriverHost = pDriverHost;
	return vr::EVRInitError();
}

void NOLOClientDriver::Cleanup()
{
}

bool NOLOClientDriver::BIsHmdPresent(const char * pchUserConfigDir)
{
	return false;
}

vr::EVRInitError NOLOClientDriver::SetDisplayId(const char * pchDisplayId)
{
	return vr::VRInitError_None;
}

vr::HiddenAreaMesh_t NOLOClientDriver::GetHiddenAreaMesh(vr::EVREye eEye, vr::EHiddenAreaMeshType type)
{
	return vr::HiddenAreaMesh_t();
}

uint32_t NOLOClientDriver::GetMCImage(uint32_t * pImgWidth, uint32_t * pImgHeight, uint32_t * pChannels, void * pDataBuffer, uint32_t unBufferLen)
{
	return uint32_t();
}
