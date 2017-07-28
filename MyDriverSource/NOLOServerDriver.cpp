#include "NOLOServerDriver.h"

bool NOLOServerDriver::flag_ConnectNolo = false;
bool NOLOServerDriver::flag_DisConnectNolo = false;
bool NOLOServerDriver::flag_rotQ = false;
NoloData NOLOServerDriver::noloData;
Vector3 NOLOServerDriver::posR;
NOLOServerDriver::NOLOServerDriver() : m_bStopRequested(false)
                                     , m_bLaunchedHydraMonitor(false)
{

}

NOLOServerDriver::~NOLOServerDriver()
{
	Cleanup();
}

vr::EVRInitError NOLOServerDriver::Init(vr::IDriverLog * pDriverLog, vr::IServerDriverHost * pDriverHost, const char * pchUserDriverConfigDir, const char * pchDriverInstallDir)
{
	InitDriverLog(pDriverLog);
	DriverLog(">>>>>>>>>>>>>>>>>>>>>>>>IServerTrackedDeviceProvider Init<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
	m_pDriverHost = pDriverHost;
	m_strDriverInstallDir = pchDriverInstallDir;
	flag_preDwon = false;
	flag_rotQ = false;
	rotQ.w = 0; rotQ.x = 0; rotQ.y = 1; rotQ.z = 0;
	//打开监听zeromq
	connectSuccess_FunCallBack(connect_Nolo);
	disConnect_FunCallBack(disconnect_Nolo);
	Sleep(100);
	open_Nolo_ZeroMQ();
	
	for (int i = 0; i < 30; i++) {
	
		notify_Nolo_GameBegin();
	}


	//ScanForNewControllers();
	noloLeftController=NULL;
	noloRightController=NULL;
	m_Thread = std::thread(ThreadEntry, this);
	m_Thread.detach();
	return vr::VRInitError_None;
}

void NOLOServerDriver::ThreadEntry(NOLOServerDriver * pDriver)
{
	pDriver->ThreadFunc();
}

void NOLOServerDriver::ThreadFunc()
{
	static bool flag_Once = false;
	static bool flag_left = false;
	static bool flag_right = false;
	static bool flag_LIsConnect = false;
	static bool flag_RIsConnect = false;
	static bool flag_Turn180 = false;

	while (!m_bStopRequested)
	{
		noloData = get_Nolo_NoloData();
		CtrData leftcontroller;
		CtrData rightcontroller;
		flag_LIsConnect = true;
		flag_RIsConnect = true;

		if (noloData.expandData[1] > 0 ) {
			if (flag_Turn180==false) {
				flag_rotQ =true;
				posR = noloData.hmdData.HMDPosition;
				flag_Turn180 = true;

			}
		}
		else
		{
			if (flag_Turn180) {
				flag_rotQ = false;
				flag_Turn180 = false;
			}

		}

		if (flag_rotQ)
		{

			noloData.leftData.LeftControllerRotation = rotQ * noloData.leftData.LeftControllerRotation;

			noloData.leftData.LeftControllerPosition.x = (2 * posR.x - noloData.leftData.LeftControllerPosition.x);
			noloData.leftData.LeftControllerPosition.z = 2 * posR.z - noloData.leftData.LeftControllerPosition.z;

			noloData.rightData.RightControllerRotation = rotQ *noloData.rightData.RightControllerRotation;

			noloData.rightData.RightControllerPosition.x = (2 * posR.x - noloData.rightData.RightControllerPosition.x);
			noloData.rightData.RightControllerPosition.z = 2 * posR.z - noloData.rightData.RightControllerPosition.z;
		}
		
		leftcontroller.pos[0] = noloData.leftData.LeftControllerPosition.x;
		leftcontroller.pos[1] = noloData.leftData.LeftControllerPosition.y;
		leftcontroller.pos[2] = noloData.leftData.LeftControllerPosition.z;

		leftcontroller.rot_quat[0] = noloData.leftData.LeftControllerRotation.w;
		leftcontroller.rot_quat[1] = noloData.leftData.LeftControllerRotation.x;
		leftcontroller.rot_quat[2] = noloData.leftData.LeftControllerRotation.y;
		leftcontroller.rot_quat[3] = noloData.leftData.LeftControllerRotation.z;

		rightcontroller.pos[0] = noloData.rightData.RightControllerPosition.x;
		rightcontroller.pos[1] = noloData.rightData.RightControllerPosition.y;
		rightcontroller.pos[2] = noloData.rightData.RightControllerPosition.z;


		rightcontroller.rot_quat[0] = noloData.rightData.RightControllerRotation.w;
		rightcontroller.rot_quat[1] = noloData.rightData.RightControllerRotation.x;
		rightcontroller.rot_quat[2] = noloData.rightData.RightControllerRotation.y;
		rightcontroller.rot_quat[3] = noloData.rightData.RightControllerRotation.z;

		leftcontroller.buttons = noloData.leftData.LeftButtons;
		rightcontroller.buttons = noloData.rightData.RightButtons;


		leftcontroller.joyStick_X = noloData.leftData.LeftControllerTouchAxis.x;
		leftcontroller.joyStick_Y = noloData.leftData.LeftControllerTouchAxis.y;


		rightcontroller.joyStick_X = noloData.rightData.RightControllerTouchAxis.x;
		rightcontroller.joyStick_Y = noloData.rightData.RightControllerTouchAxis.y;

		leftcontroller.touch = noloData.leftData.LeftControllerTouched;
		rightcontroller.touch = noloData.rightData.RightControllerTouched;

		leftcontroller.status = noloData.leftData.state;
		rightcontroller.status = noloData.rightData.state;

		if (flag_DisConnectNolo)
		{
			rightcontroller.status = 0;
			leftcontroller.status = 0;
			flag_LIsConnect = false;
			flag_RIsConnect = false;
		}

		if (flag_ConnectNolo) {

			if (noloData.leftData.LeftControllerBattery == 255) {
				flag_LIsConnect = false;
			}

			if (noloData.rightData.RightControllerBattery == 255) {
				flag_RIsConnect = false;
			}

			if (flag_Once == false) {

				if (flag_left == false && leftcontroller.status) {
					DriverLog("===================Create NOLO LeftTrackedDevice======================\n");
					if (m_pDriverHost != NULL) {
						try
						{
							if (noloLeftController == NULL) {
								noloLeftController = new NOLOTrackedDevice(m_pDriverHost, 0);
								m_vecControllers.push_back(noloLeftController);
								m_pDriverHost->TrackedDeviceAdded(noloLeftController->GetSerialNumber());
								flag_left = true;
							}
						}
						catch (const std::exception& e)
						{
							DriverLog("===================Create NOLO LeftTrackedDevice Exception======================\n");
						}
					}

				}

				if (flag_right == false && rightcontroller.status) {
					DriverLog("===================Create NOLO RightTrackedDevice======================\n");
					if (m_pDriverHost != NULL) {
						try
						{
							if (noloRightController == NULL) {
								noloRightController = new NOLOTrackedDevice(m_pDriverHost, 1);
								m_vecControllers.push_back(noloRightController);
								m_pDriverHost->TrackedDeviceAdded(noloRightController->GetSerialNumber());
								flag_right = true;
							}

						
						}
						catch (const std::exception&)
						{
							DriverLog("===================Create NOLO RightTrackedDevice Exception======================\n");
						}

					}

				}
				
				if (flag_left && flag_right) {
					DriverLog("===================Create NOLO Device Over======================\n");
					flag_Once = true;
				}
			}

		}

		if (flag_left) {
			leftcontroller.status = 1;//创建出来后一直显示
			leftcontroller.isConnect = flag_LIsConnect;
			noloLeftController->RunFrame(leftcontroller);
		}
		if (flag_right) {
			rightcontroller.status = 1;
			rightcontroller.isConnect = flag_RIsConnect;
			noloRightController->RunFrame(rightcontroller);
		}

		Sleep(14);
	}
	DriverLog("===================ReadDataThreadEnd======================\n");

	flag_Once = false;
	flag_left = false;
	flag_right = false;
	flag_LIsConnect = false;
	flag_RIsConnect = false;
	close_Nolo_ZeroMQ();
}

void NOLOServerDriver::connect_Nolo()
{
	DriverLog("===================Connect NOLO Server======================\n");
	flag_DisConnectNolo = false;
	flag_ConnectNolo = true;
}

void NOLOServerDriver::disconnect_Nolo()
{
	DriverLog("===================DisConnect NOLO Server======================\n");
	flag_ConnectNolo = false;
	flag_DisConnectNolo = true;
}


void NOLOServerDriver::Cleanup()
{
	DriverLog("===================Cleanup======================\n");

	for (int i = 0; i < 30; i++) {

		notify_Nolo_GameOver();
	}
	m_bStopRequested = true;
	close_Nolo_ZeroMQ();
	if (m_Thread.joinable())
	{
		m_Thread.join();
	}
	if (noloLeftController!=NULL) {
	
		noloLeftController->Deactivate();
		delete noloLeftController;
		noloLeftController = NULL;
	}
	if (noloRightController != NULL) {
		noloRightController->Deactivate();
		delete noloRightController;
		noloRightController = NULL;
	}
	m_vecControllers.clear();
}

const char * const * NOLOServerDriver::GetInterfaceVersions()
{
	return vr::k_InterfaceVersions;
}

uint32_t NOLOServerDriver::GetTrackedDeviceCount()
{
	scope_lock lock(m_Mutex);

	return m_vecControllers.size();
}

vr::ITrackedDeviceServerDriver * NOLOServerDriver::GetTrackedDeviceDriver(uint32_t unWhich)
{
	scope_lock lock(m_Mutex);

	if (unWhich < m_vecControllers.size())
		return m_vecControllers[unWhich];

	return nullptr;
}

vr::ITrackedDeviceServerDriver * NOLOServerDriver::FindTrackedDeviceDriver(const char * pchId)
{
	scope_lock lock(m_Mutex);

	for (auto it = m_vecControllers.begin(); it != m_vecControllers.end(); ++it)
	{
		if (0 == strcmp((*it)->GetSerialNumber(), pchId))
		{
			return *it;
		}
	}
	return nullptr;
}

void NOLOServerDriver::RunFrame()
{
}

bool NOLOServerDriver::ShouldBlockStandbyMode()
{
	return false;
}

void NOLOServerDriver::EnterStandby()
{
}

void NOLOServerDriver::LeaveStandby()
{
}
