#pragma once
#ifndef _NOLO_PIMAX_API_H_
#define _NOLO_PIMAX_API_H_
#define WIN32_LEAN_AND_MEAN    
#include <windows.h>
#include <math.h>

#ifndef NOLO_API
#define NOLO_API extern "C"
#else
#define NOLO_API
#endif
///Nolo_USBHID.dll api
namespace NOLO {
	typedef struct Vector2
	{
		float x;
		float y;
	}Vector2;
	typedef struct Vector3
	{
		float x;
		float y;
		float z;
	}Vector3;

	typedef struct Quaternion
	{
		float x;
		float y;
		float z;
		float w;
	}Quaternion;
    //nolo controller struct
	typedef struct Controller
	{

		Vector3 ControllerPosition;

		Quaternion ControllerRotation;
        /*
		  0x0000_XXX1:press
          0x0000_XX1X:trigger
          0x0000_X1XX:menu
          0x0000_1XXX:system
          0x0001_XXXX:grip
		*/
		UINT Buttons;
        /**
		0~1
		nolo touch，只要触摸了触摸板状态就为1,没有触摸则为0
		*/
		int ControllerTouched;
        /*
		   x:-1~1
		   y:-1~1
		*/
		Vector2 ControllerTouchAxis;
        /*
		  0~100：normal,正常显示电量为0~100
		  255:Shut down,长按system按键直到关闭手柄，值为255
		*/
		int ControllerBattery;
        /*
		  0：Be blocked,被遮挡
		  1：normal
		*/
		int state;
	}Controller;
	///nolo head tracking struct
	typedef struct HMD
	{

		Vector3 HMDPosition;
        ///Handle Init Position
		Vector3 HMDInitPosition;

		Quaternion HMDRotation;
        /*
		  0：Be blocked
		  1：normal
		*/
		int state;
	}HMD;
    ///nolo base station struct
	typedef struct BaseStation
	{
        /*
		  0~100:normal
		*/
		int BaseStationPower;
	}BaseStation;
    ///nolo all data struct
	typedef struct NoloData {
        
		Controller left_Handle_Data;

		Controller right_Handle_Data;

		HMD hmdData;

		BaseStation baseStationData;

	}NoloData;
    /// nolo Device type enumerate
	typedef enum NoloDeviceType
	{
		HmdDevice = 0,
		LeftControllerDevice,
		RightControllerDevice,
		BaseStationDevice
	}NoloDeviceType;

	typedef struct  Nolo_Pose
	{
		Vector3 pos;
		Quaternion rot;
	}Nolo_Pose;
	//
	typedef struct ControllerStates
	{
	   /*
		  0x0000_XXX1:press
          0x0000_XX1X:trigger
          0x0000_X1XX:menu
          0x0000_1XXX:system
          0x0001_XXXX:grip
		*/
		UINT noloButtons;
	    ///Handle with touch
		UINT noloButtonTouch;
		/*
		   x:-1~1
		   y:-1~1
		*/
		Vector2 touchpadAxis;
	}ControllerStates;
	
	typedef enum ExpandMsgType {

	  DoubleClickMenu=1,
	  DoubleClickSystem
    }ExpandMsgType;
	
	typedef void(__cdecl *funcCallBack)();
	typedef void(__cdecl *expandMsg_FuncCallBack)(ExpandMsgType expandMsgType);
	///搜索NOLO设备
	NOLO_API  void _cdecl open_Nolo_ZeroMQ();
	///释放与NOLO设备连接的资源
	NOLO_API  void _cdecl close_Nolo_ZeroMQ();
	///断开连接NOLO设备时进行回调通知
	NOLO_API  bool _cdecl disConnect_FunCallBack(funcCallBack fun);
	///连接NOLO设备时进行回调通知
	NOLO_API  bool _cdecl connectSuccess_FunCallBack(funcCallBack fun);
	///参数为设备类型和震动强度，设备类型请选择左手和右手，震动强度范围0~100,值越大震动越激烈
	NOLO_API  bool _cdecl set_Nolo_TriggerHapticPulse(NoloDeviceType deviceType, int intensity);
	///获取NOLO数据接口函数
	NOLO_API  NoloData _cdecl get_Nolo_NoloData();
	
	///returns nolo controller data
	NOLO_API  Controller _cdecl get_Nolo_LeftControllerData();
	///returns nolo controller data
	NOLO_API  Controller _cdecl get_Nolo_RightControllerData();
	///returns nolo head tracking data
	NOLO_API HMD _cdecl get_Nolo_HMDData();

	///returns nolo device state by device type
	NOLO_API int _cdecl get_Nolo_StateByDeviceType(NoloDeviceType type);
	///returns nolo device battery by device type
	NOLO_API int _cdecl get_Nolo_Battery(NoloDeviceType deviceType);
	
	///get NOLO Controller States By Device Type
	NOLO_API ControllerStates _cdecl get_Nolo_ControllerStates(NoloDeviceType devicetype);
	///get NOLO Pose By Device Type
	NOLO_API Nolo_Pose _cdecl get_Nolo_Pose(NoloDeviceType devicetype);
	//双击menu按键或双击system按键回调通知接口函数
	NOLO_API  bool _cdecl expandDataNotify_FuncCallBack(expandMsg_FuncCallBack fun);
	//
	NOLO_API  Vector3 _cdecl get_Nolo_BaseStionPosition();
}
#endif // _NOLO_PIMAX_API_H_