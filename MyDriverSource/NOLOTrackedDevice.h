#pragma once
// OpenVR includes
#include <openvr_driver.h>
#include <string>
#include "NOLO_Utility.h"

#define NOLO_BUTTON_1        (0x01<<0)//press
#define NOLO_BUTTON_2        (0x01<<1)//trigger
#define NOLO_BUTTON_3        (0x01<<2)//menu
#define NOLO_BUTTON_4        (0x01<<3)//system
#define NOLO_BUTTON_5        (0x01<<4)//grip
#define COUNT 2
class NOLOTrackedDevice: public vr::ITrackedDeviceServerDriver, public vr::IVRControllerComponent
{

public:
	NOLOTrackedDevice(vr::IServerDriverHost *pDriverHost, int id);
	~NOLOTrackedDevice();

	typedef void (vr::IServerDriverHost::*ButtonUpdate)(uint32_t unWhichDevice, vr::EVRButtonId eButtonId, double eventTimeOffset);

	const char *GetSerialNumber();
	// Inherited via ITrackedDeviceServerDriver
	virtual vr::EVRInitError Activate(uint32_t unObjectId) override;
	virtual void Deactivate();
	virtual void EnterStandby() override;
	virtual void * GetComponent(const char * pchComponentNameAndVersion) override;
	virtual void DebugRequest(const char * pchRequest, char * pchResponseBuffer, uint32_t unResponseBufferSize) override;
	virtual vr::DriverPose_t GetPose() override;
	virtual bool GetBoolTrackedDeviceProperty(vr::ETrackedDeviceProperty prop, vr::ETrackedPropertyError * pError) override;
	virtual float GetFloatTrackedDeviceProperty(vr::ETrackedDeviceProperty prop, vr::ETrackedPropertyError * pError) override;
	virtual int32_t GetInt32TrackedDeviceProperty(vr::ETrackedDeviceProperty prop, vr::ETrackedPropertyError * pError) override;
	virtual uint64_t GetUint64TrackedDeviceProperty(vr::ETrackedDeviceProperty prop, vr::ETrackedPropertyError * pError) override;
	virtual vr::HmdMatrix34_t GetMatrix34TrackedDeviceProperty(vr::ETrackedDeviceProperty prop,vr:: ETrackedPropertyError * pError) override;
	virtual uint32_t GetStringTrackedDeviceProperty(vr::ETrackedDeviceProperty prop, char * pchValue, uint32_t unBufferSize, vr::ETrackedPropertyError * pError) override;

	// Inherited via IVRControllerComponent
	virtual vr::VRControllerState_t GetControllerState() override;
	//u发送震动数据
	virtual bool TriggerHapticPulse(uint32_t unAxisId, uint16_t usPulseDurationMicroseconds) override;

	void RunFrame(CtrData &data);

	void SendButtonUpdates(ButtonUpdate ButtonEvent, uint64_t ulMask);

	void UpdateControllerState(CtrData & cd);

	void UpdateTrackingState(CtrData &data);

	Vector3 getAcceleration(const CtrData &data);

	bool IsActivated() const;

	void Test();

	static const vr::EVRButtonId k_EButton_Button1 = vr::k_EButton_SteamVR_Touchpad;
	static const vr::EVRButtonId k_EButton_Button2 = vr::k_EButton_SteamVR_Trigger;
	static const vr::EVRButtonId k_EButton_Button3 = vr::k_EButton_ApplicationMenu;
	static const vr::EVRButtonId k_EButton_Button4 = vr::k_EButton_System;
	static const vr::EVRButtonId k_EButton_Button5 = vr::k_EButton_Grip;
private:
	//typedef void (vr::IServerDriverHost::*ButtonUpdate)(uint32_t unWhichDevice, vr::EVRButtonId eButtonId, double eventTimeOffset);
	vr::IServerDriverHost *m_pDriverHost;
	int m_nBase;
	int m_nId;
	uint8_t m_ucPoseSequenceNumber;
	enum { k_eHemisphereTrackingDisabled, k_eHemisphereTrackingButtonDown, k_eHemisphereTrackingEnabled } m_eHemisphereTrackingState;
	bool m_bCalibrated;
	NOLOTrackedDevice *m_pAlignmentPartner;
	enum { k_eIdle, k_eWaiting, k_eSent, k_ePulsed, k_eBlocked } m_eSystemButtonState;

	unsigned short m_firmware_revision;
	unsigned short m_hardware_revision;

	vr::DriverPose_t m_Pose;
	vr::DriverPose_t pre_Pose;
	std::string m_strSerialNumber;
	std::string m_strModelNumber;
	vr::VRControllerState_t m_ControllerState;
	uint32_t m_unSteamVRTrackedDeviceId;


	bool flag_TriggerDown = false;
	bool flag_Throw = false;
};

