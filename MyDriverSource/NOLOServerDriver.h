#pragma once
#include <openvr_driver.h>
#include <chrono>
#include <mutex>
#include <vector>
#include <atomic>
#include "driverlog.h"
#include "NOLOTrackedDevice.h"
#include<thread>
//==================================================================================================
// NOLOServerDriver
//==================================================================================================

class NOLOServerDriver: public vr::IServerTrackedDeviceProvider
{
public:
	NOLOServerDriver();
	~NOLOServerDriver();
	// Inherited via IServerTrackedDeviceProvider
	virtual vr::EVRInitError Init(vr::IDriverLog * pDriverLog, vr::IServerDriverHost * pDriverHost, const char * pchUserDriverConfigDir, const char * pchDriverInstallDir) override;

#pragma region 接收数据
	static void ThreadEntry(NOLOServerDriver * pDriver);

	void ThreadFunc();
#pragma endregion

	//晓阳代码
	static void connect_Nolo();
	static void disconnect_Nolo();
	//搜索设备
	//void ScanForNewControllers();

	virtual void Cleanup() override;
	virtual const char * const * GetInterfaceVersions() override;
	virtual uint32_t GetTrackedDeviceCount() override;
	virtual vr::ITrackedDeviceServerDriver * GetTrackedDeviceDriver(uint32_t unWhich) override;
	virtual vr::ITrackedDeviceServerDriver * FindTrackedDeviceDriver(const char * pchId) override;
	virtual void RunFrame() override;
	virtual bool ShouldBlockStandbyMode() override;
	virtual void EnterStandby() override;
	virtual void LeaveStandby() override;
private:
	vr::IServerDriverHost *m_pDriverHost;
	std::string m_strDriverInstallDir;
	std::recursive_mutex m_Mutex;
	typedef std::lock_guard<std::recursive_mutex> scope_lock;

	std::vector< NOLOTrackedDevice * > m_vecControllers;

	NOLOTrackedDevice * noloLeftController;
	NOLOTrackedDevice * noloRightController;

	std::thread m_Thread;
	bool m_bLaunchedHydraMonitor;
	std::atomic<bool> m_bStopRequested;

	bool flag_Create = false;


	static NoloData noloData;
	static bool flag_rotQ;
	bool flag_preDwon;//左手menu键按下
	static Vector3 posR;
	Quaternion rotQ;
	static bool flag_ConnectNolo;
	static bool flag_DisConnectNolo;
};
