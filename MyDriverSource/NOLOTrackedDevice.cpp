#include "NOLOTrackedDevice.h"
#include <sstream>
#include "driverlog.h"
#include <thread>
static void GenerateSerialNumber(char *p, int psize, int controller)
{
	//_snprintf(p, psize, "nolo_controller_%d", controller);
	sprintf(p, "NOLO_Tracking_controller_%d", controller);
}
NOLOTrackedDevice::NOLOTrackedDevice(vr::IServerDriverHost *pDriverHost, int id) :m_pDriverHost(pDriverHost)
, m_nBase(1)
, m_nId(id)
, m_ucPoseSequenceNumber(0)
, m_eHemisphereTrackingState(k_eHemisphereTrackingDisabled)
, m_bCalibrated(false)
, m_pAlignmentPartner(NULL)
, m_eSystemButtonState(k_eIdle)
, m_unSteamVRTrackedDeviceId(vr::k_unTrackedDeviceIndexInvalid)
{
	char buf[256];
	GenerateSerialNumber(buf, sizeof(buf), m_nId);

	m_strSerialNumber = buf;
	m_strModelNumber = m_nId;

	flag_TriggerDown = false;
	flag_Throw = false;
	memset(&m_ControllerState, 0, sizeof(m_ControllerState));
	memset(&m_Pose, 0, sizeof(m_Pose));
	memset(&pre_Pose, 0, sizeof(pre_Pose));
	m_Pose.result = vr::TrackingResult_Calibrating_InProgress;

	m_firmware_revision = 1313 + m_nId;
	m_hardware_revision = 1315 + m_nId;
}

NOLOTrackedDevice::~NOLOTrackedDevice()
{
}

const char * NOLOTrackedDevice::GetSerialNumber()
{
	return m_strSerialNumber.c_str();
}

vr::EVRInitError NOLOTrackedDevice::Activate(uint32_t unObjectId)
{
	m_unSteamVRTrackedDeviceId = unObjectId;
	return vr::VRInitError_None;
}

void NOLOTrackedDevice::Deactivate()
{
	m_unSteamVRTrackedDeviceId = vr::k_unTrackedDeviceIndexInvalid;
}

void NOLOTrackedDevice::EnterStandby()
{
}

void * NOLOTrackedDevice::GetComponent(const char * pchComponentNameAndVersion)
{
	if (!_stricmp(pchComponentNameAndVersion, vr::IVRControllerComponent_Version))
	{
		return (vr::IVRControllerComponent*)this;
	}

	return NULL;
}

void NOLOTrackedDevice::DebugRequest(const char * pchRequest, char * pchResponseBuffer, uint32_t unResponseBufferSize)
{
	if (unResponseBufferSize >= 1)
		pchResponseBuffer[0] = 0;
}

vr::DriverPose_t NOLOTrackedDevice::GetPose()
{
	return m_Pose;
}

bool NOLOTrackedDevice::GetBoolTrackedDeviceProperty(vr::ETrackedDeviceProperty prop, vr::ETrackedPropertyError * pError)
{
	*pError = vr::TrackedProp_ValueNotProvidedByDevice;
	return false;
}

float NOLOTrackedDevice::GetFloatTrackedDeviceProperty(vr::ETrackedDeviceProperty prop, vr::ETrackedPropertyError * pError)
{
	*pError = vr::TrackedProp_ValueNotProvidedByDevice;
	return 0.0f;
}

int32_t NOLOTrackedDevice::GetInt32TrackedDeviceProperty(vr::ETrackedDeviceProperty prop, vr::ETrackedPropertyError * pError)
{
	int32_t nRetVal = 0;
	vr::ETrackedPropertyError error = vr::TrackedProp_UnknownProperty;
	switch (prop)
	{
	case vr::Prop_DeviceClass_Int32:
		nRetVal = vr::TrackedDeviceClass_Controller;
		error = vr::TrackedProp_Success;
		break;

	case vr::Prop_Axis0Type_Int32:
		nRetVal = vr::k_eControllerAxis_Joystick;
		error = vr::TrackedProp_Success;
		break;

	case vr::Prop_Axis1Type_Int32:
		nRetVal = vr::k_eControllerAxis_Trigger;
		error = vr::TrackedProp_Success;
		break;

	case vr::Prop_Axis2Type_Int32:
	case vr::Prop_Axis3Type_Int32:
	case vr::Prop_Axis4Type_Int32:
		error = vr::TrackedProp_ValueNotProvidedByDevice;
		break;
	}

	*pError = error;
	return nRetVal;
}

uint64_t NOLOTrackedDevice::GetUint64TrackedDeviceProperty(vr::ETrackedDeviceProperty prop, vr::ETrackedPropertyError * pError)
{
	uint64_t ulRetVal = 0;
	vr::ETrackedPropertyError error = vr::TrackedProp_ValueNotProvidedByDevice;
	switch (prop)
	{
	case vr::Prop_CurrentUniverseId_Uint64:
	case vr::Prop_PreviousUniverseId_Uint64:
		error = vr::TrackedProp_ValueNotProvidedByDevice;
		break;

	case vr::Prop_SupportedButtons_Uint64:
		ulRetVal =
			vr::ButtonMaskFromId(vr::k_EButton_ApplicationMenu) |
			vr::ButtonMaskFromId(vr::k_EButton_SteamVR_Touchpad) |
			vr::ButtonMaskFromId(vr::k_EButton_SteamVR_Trigger) |
			vr::ButtonMaskFromId(vr::k_EButton_System) |
			vr::ButtonMaskFromId(vr::k_EButton_Grip);

		error = vr::TrackedProp_Success;
		break;

	case vr::Prop_HardwareRevision_Uint64:
		ulRetVal = m_hardware_revision;
		error = vr::TrackedProp_Success;
		break;

	case vr::Prop_FirmwareVersion_Uint64:
		ulRetVal = m_firmware_revision;
		error = vr::TrackedProp_Success;
		break;

	}

	*pError = error;
	return ulRetVal;
}

vr::HmdMatrix34_t NOLOTrackedDevice::GetMatrix34TrackedDeviceProperty(vr::ETrackedDeviceProperty prop, vr::ETrackedPropertyError * pError)
{
	return vr::HmdMatrix34_t();
}

uint32_t NOLOTrackedDevice::GetStringTrackedDeviceProperty(vr::ETrackedDeviceProperty prop, char * pchValue, uint32_t unBufferSize, vr::ETrackedPropertyError * pError)
{
	std::ostringstream ssRetVal;
	switch (prop)
	{
	case vr::Prop_SerialNumber_String:
		ssRetVal << m_strSerialNumber;
		break;

	case vr::Prop_RenderModelName_String:
		// The {hydra} syntax lets us refer to rendermodels that are installed
		// in the driver's own resources/rendermodels directory.  The driver can
		// still refer to SteamVR models like "generic_hmd".
		ssRetVal << "vr_controller_vive_1_5";
		break;

	case vr::Prop_ManufacturerName_String:
		ssRetVal << "HTC";
		break;

	case vr::Prop_ModelNumber_String:
		ssRetVal << "NOLO Tracking controller";
		break;

	case vr::Prop_TrackingFirmwareVersion_String:
		ssRetVal << "cd.firmware_revision=" << m_firmware_revision;
		break;

	case vr::Prop_HardwareRevision_String:
		ssRetVal << "cd.hardware_revision=" << m_hardware_revision;
		break;
	}

	std::string sRetVal = ssRetVal.str();
	if (sRetVal.empty())
	{
		*pError = vr::TrackedProp_ValueNotProvidedByDevice;
		return 0;
	}
	else if (sRetVal.size() + 1 > unBufferSize)
	{
		*pError = vr::TrackedProp_BufferTooSmall;
		return sRetVal.size() + 1;  // caller needs to know how to size buffer
	}
	else
	{
		DriverLog(sRetVal.c_str());
		_snprintf(pchValue, unBufferSize, sRetVal.c_str());
		*pError = vr::TrackedProp_Success;
		return sRetVal.size() + 1;
	}
}

vr::VRControllerState_t NOLOTrackedDevice::GetControllerState()
{
	return vr::VRControllerState_t();
}

bool NOLOTrackedDevice::TriggerHapticPulse(uint32_t unAxisId, uint16_t usPulseDurationMicroseconds)
{

	//DriverLog("ZhenDong:====%d========================================\n", usPulseDurationMicroseconds);

	int n = usPulseDurationMicroseconds/40;
	if (n>50) {
	
		n = 50;
	}

	if (m_nId == 0) {
		
		set_Nolo_TriggerHapticPulse(NoloDeviceType::LeftControllerDevice, 50+n);
	}
	else if (m_nId == 1)
	{
		set_Nolo_TriggerHapticPulse(NoloDeviceType::RightControllerDevice,50+n);
	}
	return true;
}

void NOLOTrackedDevice::RunFrame(CtrData & data)
{
	if (m_unSteamVRTrackedDeviceId != vr::k_unTrackedDeviceIndexInvalid)
	{
		UpdateTrackingState(data);
		UpdateControllerState(data);
		m_pDriverHost->TrackedDevicePoseUpdated(m_unSteamVRTrackedDeviceId, GetPose());
	}
}

void NOLOTrackedDevice::SendButtonUpdates(ButtonUpdate ButtonEvent, uint64_t ulMask)
{
	if (!ulMask)
		return;

	for (int i = 0; i< vr::k_EButton_Max; i++)
	{
		vr::EVRButtonId button = (vr::EVRButtonId)i;

		uint64_t bit = ButtonMaskFromId(button);

		if ((bit & ulMask))
		{
			(m_pDriverHost->*ButtonEvent)(m_unSteamVRTrackedDeviceId, button, 0.0);
		}
	}
}

void NOLOTrackedDevice::UpdateControllerState(CtrData & cd)
{
	vr::VRControllerState_t NewState = { 0 };
	NewState.unPacketNum = m_ControllerState.unPacketNum + 1;
	if (cd.buttons & NOLO_BUTTON_1) {
		NewState.ulButtonPressed |= vr::ButtonMaskFromId(k_EButton_Button1);
	}
	if (cd.buttons & NOLO_BUTTON_2) {
		NewState.ulButtonTouched |= vr::ButtonMaskFromId(k_EButton_Button2);
	}
	if (cd.buttons & NOLO_BUTTON_3) {
		NewState.ulButtonPressed |= vr::ButtonMaskFromId(k_EButton_Button3);
	}

	if (cd.buttons & NOLO_BUTTON_4) {
		NewState.ulButtonPressed |= vr::ButtonMaskFromId(k_EButton_Button4);
	}
	if (cd.buttons & NOLO_BUTTON_5) {
		NewState.ulButtonPressed |= vr::ButtonMaskFromId(k_EButton_Button5);
	}

	// All pressed buttons are touched
	if (cd.buttons & NOLO_BUTTON_1)
		NewState.ulButtonPressed |= vr::ButtonMaskFromId(vr::k_EButton_Axis0);
	if (cd.buttons & NOLO_BUTTON_2)
		NewState.ulButtonTouched |= vr::ButtonMaskFromId(vr::k_EButton_Axis1);
	if (cd.buttons & NOLO_BUTTON_2)
		NewState.ulButtonPressed |= vr::ButtonMaskFromId(vr::k_EButton_Axis1);
	if (cd.touch)
		NewState.ulButtonTouched |= vr::ButtonMaskFromId(vr::k_EButton_Axis0);

	NewState.ulButtonTouched |= NewState.ulButtonPressed;

	uint64_t ulChangedTouched = NewState.ulButtonTouched ^ m_ControllerState.ulButtonTouched;
	uint64_t ulChangedPressed = NewState.ulButtonPressed ^ m_ControllerState.ulButtonPressed;

	SendButtonUpdates(&vr::IServerDriverHost::TrackedDeviceButtonTouched, ulChangedTouched & NewState.ulButtonTouched);
	SendButtonUpdates(&vr::IServerDriverHost::TrackedDeviceButtonPressed, ulChangedPressed & NewState.ulButtonPressed);
	SendButtonUpdates(&vr::IServerDriverHost::TrackedDeviceButtonUnpressed, ulChangedPressed & ~NewState.ulButtonPressed);
	SendButtonUpdates(&vr::IServerDriverHost::TrackedDeviceButtonUntouched, ulChangedTouched & ~NewState.ulButtonTouched);


	NewState.rAxis[0].x = cd.joyStick_X;
	NewState.rAxis[0].y = cd.joyStick_Y;
	NewState.rAxis[1].x = cd.buttons & NOLO_BUTTON_2;
	NewState.rAxis[1].y = 0.0f;

	if (NewState.rAxis[0].x != m_ControllerState.rAxis[0].x || NewState.rAxis[0].y != m_ControllerState.rAxis[0].y)
		m_pDriverHost->TrackedDeviceAxisUpdated(m_unSteamVRTrackedDeviceId, 0, NewState.rAxis[0]);
	if (NewState.rAxis[1].x != m_ControllerState.rAxis[1].x)
		m_pDriverHost->TrackedDeviceAxisUpdated(m_unSteamVRTrackedDeviceId, 1, NewState.rAxis[1]);

	m_ControllerState = NewState;
}

void NOLOTrackedDevice::UpdateTrackingState(CtrData & data)
{

	m_Pose.poseTimeOffset = -0.016f;
	m_Pose.qWorldFromDriverRotation.w = 1.0;
	m_Pose.qWorldFromDriverRotation.x = 0.0;
	m_Pose.qWorldFromDriverRotation.y = 0.0;
	m_Pose.qWorldFromDriverRotation.z = 0.0;
	m_Pose.vecWorldFromDriverTranslation[0] = 0.0;
	m_Pose.vecWorldFromDriverTranslation[1] = 0.0;
	m_Pose.vecWorldFromDriverTranslation[2] = 0.0;

	m_Pose.qDriverFromHeadRotation.w = 1.0f;
	m_Pose.qDriverFromHeadRotation.x = 0.0f;
	m_Pose.qDriverFromHeadRotation.y = 0.0f;
	m_Pose.qDriverFromHeadRotation.z = 0.0f;
	m_Pose.vecDriverFromHeadTranslation[0] = 0.000f;
	m_Pose.vecDriverFromHeadTranslation[1] = 0.007f;
	m_Pose.vecDriverFromHeadTranslation[2] = -0.073f;

	m_Pose.vecAcceleration[0] = 0.0;
	m_Pose.vecAcceleration[1] = 0.0;
	m_Pose.vecAcceleration[2] = 0.0;
	m_Pose.vecAngularAcceleration[0] = 0.0;
	m_Pose.vecAngularAcceleration[1] = 0.0;
	m_Pose.vecAngularAcceleration[2] = 0.0;

	m_Pose.result = vr::TrackingResult_Running_OK;
	m_Pose.willDriftInYaw = false;
	m_Pose.shouldApplyHeadModel = false;

	m_Pose.poseIsValid = data.status;//这是控制未就绪的地方


	//data
	m_Pose.vecPosition[0] = data.pos[0];
	m_Pose.vecPosition[1] = data.pos[1];
	m_Pose.vecPosition[2] = -data.pos[2];
	m_Pose.qRotation.w = -data.rot_quat[0];
	m_Pose.qRotation.x = data.rot_quat[1];
	m_Pose.qRotation.y = data.rot_quat[2];
	m_Pose.qRotation.z = -data.rot_quat[3];

	m_Pose.deviceIsConnected = data.isConnect;

	if (data.buttons & NOLO_BUTTON_2) {

		flag_TriggerDown = true;
	}
	if (flag_TriggerDown && (data.buttons & NOLO_BUTTON_2) == 0) {

		flag_TriggerDown = false;
		flag_Throw = true;

		//std::thread ta(&NOLOTrackedDevice::Test,this);
		//ta.detach();
	}

	if (flag_Throw ) {

		Vector3 vecAcceleration = getAcceleration(data);

		m_Pose.vecVelocity[0] = (data.pos[0] - pre_Pose.vecPosition[0]) * 60*2;
		m_Pose.vecVelocity[1] = (data.pos[1] - pre_Pose.vecPosition[1]) * 60*2;
		m_Pose.vecVelocity[2] = (-data.pos[2] - pre_Pose.vecPosition[2]) * 60*2;

		m_Pose.vecAngularVelocity[0] = vecAcceleration.x * 60 ;
		m_Pose.vecAngularVelocity[1] = vecAcceleration.y * 60 ;
		m_Pose.vecAngularVelocity[2] = vecAcceleration.z * 60 ;

		flag_Throw = false;
		//DriverLog("vecVelocity::%f         %f        %f  ，vecAngularVelocity : %f      %f      %f\n", m_Pose.vecVelocity[0], m_Pose.vecVelocity[1], m_Pose.vecVelocity[2], m_Pose.vecAngularVelocity[0], m_Pose.vecAngularVelocity[1], m_Pose.vecAngularVelocity[2]);
	
	}
	else
	{
		m_Pose.vecVelocity[0] = 0.0f;
		m_Pose.vecVelocity[1] = 0.0f;
		m_Pose.vecVelocity[2] = 0.0f;
		m_Pose.vecAngularVelocity[0] = 0.0f;
		m_Pose.vecAngularVelocity[1] = 0.0f;
		m_Pose.vecAngularVelocity[2] = 0.0f;
	}


	if (flag_Throw==false && pre_Pose.vecPosition[0] != m_Pose.vecPosition[0] && pre_Pose.vecPosition[1] != m_Pose.vecPosition[1]) {
		
		pre_Pose = m_Pose;
	}

	m_pDriverHost->TrackedDevicePoseUpdated(m_unSteamVRTrackedDeviceId, m_Pose);
}

Vector3 NOLOTrackedDevice::getAcceleration(const CtrData & data)
{
	Quaternion pre;
	pre.w = -pre_Pose.qRotation.w;
	pre.x = pre_Pose.qRotation.x;
	pre.y = pre_Pose.qRotation.y;
	pre.z = -pre_Pose.qRotation.z;
	Quaternion cur;
	cur.w = data.rot_quat[0];
	cur.x = data.rot_quat[1];
	cur.y = data.rot_quat[2];
	cur.z = data.rot_quat[3];
	Vector3 t;
	t = cur.GetEulerAngle() - pre.GetEulerAngle();

	return t;
}

bool NOLOTrackedDevice::IsActivated() const
{
	return m_unSteamVRTrackedDeviceId != vr::k_unTrackedDeviceIndexInvalid;
}

void NOLOTrackedDevice::Test()
{
	bool flag = true;
	while (flag)
	{
		Sleep(16);
		flag_Throw = false;
		flag = false;
	}
}
