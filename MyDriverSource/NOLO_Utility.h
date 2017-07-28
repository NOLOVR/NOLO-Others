#pragma once
#define WIN32_LEAN_AND_MEAN    
#include <windows.h>
#include <math.h>



typedef struct _CtrData {
	float pos[3];
	float rot_quat[4];
	float joyStick_X;
	float joyStick_Y;
	unsigned int buttons;
	bool isConnect;
	int status;

	int touch;
}CtrData;

struct Vector2
{
	float x;
	float y;
};
struct Vector3
{
public:
	float x;
	float y;
	float z;
	Vector3() {}
	Vector3(float _x, float _y, float _z) {

		x = _x; y = _y; z = _z;
	}
	Vector3(double _data[3]) {

		x = _data[0];
		y = _data[1];
		z = _data[2];
	}
	Vector3(const float _data[3]) {

		x = _data[0];
		y = _data[1];
		z = _data[2];
	}

	void normalize() {

		float len = sqrt(x*x + y*y + z*z);
		x /= len;
		y /= len;
		z /= len;
	}

	friend Vector3 operator -(const Vector3 _v1, const Vector3 _v2) {

		Vector3 v;
		v.x = _v1.x - _v2.x; v.y = _v1.y - _v2.y; v.z = _v1.z - _v2.z;
		return v;
	}

	Vector3 operator ^(const Vector3 rhs) {

		Vector3 rhs_const_copy(rhs);
		return Vector3(y*rhs_const_copy.z - z*rhs_const_copy.y, z*rhs_const_copy.x - rhs_const_copy.z*x, x*rhs_const_copy.y - y*rhs_const_copy.x);
	}

	float operator *(const Vector3 rhs) {

		Vector3 rhs_const_copy(rhs);
		return x*rhs_const_copy.x + y*rhs_const_copy.y + z*rhs_const_copy.z;
	}
};

struct Quaternion
{
public:
	float x;
	float y;
	float z;
	float w;

	Quaternion() {}
	Quaternion(Vector3 anix, const float & angle) {

		Vector3 vt(anix);
		vt.normalize();
		float cosa = cos(angle);
		float sina = sin(angle);
		x = sina*vt.x;
		y = sina*vt.y;
		z = sina*vt.z;
		w = cosa;
	}

	Vector3 GetEulerAngle() {

		Vector3 ret;
		float test = y*z + x*w;
		if (test>0.4999f) {

			ret.z = 2.0f*atan2(y, w);
			ret.y = 3.1415926 / 2;
			ret.x = 0.0f;
			return ret;
		}
		else if (test<-0.4999f)
		{
			ret.z = 2.0f*atan2(y, w);
			ret.y = -3.1415926 / 2;
			ret.x = 0.0f;
			return ret;
		}

		float sqx = x*x;
		float sqy = y*y;
		float sqz = z*z;

		ret.z = atan2(2.0f*z*w - 2.0f*y*x, 1.0f - 2.0f*sqz - 2.0f*sqx);
		ret.y = asin(2.0f*test);
		ret.x = atan2(2.0f*y*w - 2.0f*z*x, 1.0f - 2.0f*sqy - 2.0f*sqx);
		return ret;
	}

	friend Quaternion operator*(const Quaternion qa, const Quaternion qb) {

		Quaternion qa_const_copy(qa), qb_const_copy(qb), qc;

		qc.w = qa_const_copy.w*qb_const_copy.w - qa_const_copy.x *qb_const_copy.x
			- qa_const_copy.y*qb_const_copy.y - qa_const_copy.z*qb_const_copy.z;

		qc.x = qa_const_copy.x*qb_const_copy.w + qa_const_copy.w *qb_const_copy.x
			+ qa_const_copy.y*qb_const_copy.z - qa_const_copy.z*qb_const_copy.y;

		qc.y = qa_const_copy.y*qb_const_copy.w + qa_const_copy.w *qb_const_copy.y
			+ qa_const_copy.z*qb_const_copy.x - qa_const_copy.x*qb_const_copy.z;

		qc.z = qa_const_copy.z*qb_const_copy.w + qa_const_copy.w *qb_const_copy.z
			+ qa_const_copy.x*qb_const_copy.y - qa_const_copy.y*qb_const_copy.x;
		return qc;
	}
};
struct LeftController
{
	//1 5A
	//2 左手硬件版本ID 不需要解析
	int LeftControllerVersionID;
	//4~9bytes
	Vector3 LeftControllerPosition;
	//10~17
	Quaternion LeftControllerRotation;
	//18   不需要解析
	UINT LeftButtons;
	//19   不需要解析
	int LeftControllerTouched;
	//20~21 不需要解析
	Vector2 LeftControllerTouchAxis;
	//22    不需要解析
	int LeftControllerBattery;

	int state;
};

struct  RightController
{
	//43 右手硬件版本ID 不需要解析
	int RightControllerVersionID;
	//45~50
	Vector3 RightControllerPosition;
	//51~58
	Quaternion RightControllerRotation;
	//59
	UINT RightButtons;
	//60
	int RightControllerTouched;
	//61~62
	Vector2 RightControllerTouchAxis;
	//63
	int RightControllerBattery;

	int state;
};

struct HMD
{
	//(21)85  不需要解析
	int HMDVersionID;
	//23(87)~28(92)
	Vector3 HMDPosition;
	//29(93)~34(98)
	Vector3 HMDInirPostion;
	//35 (99) 不需要解析
	UINT HMDTwoPointDriftAngle;
	//36(100)~43(107)
	Quaternion HMDRotation;

	int state;
};

struct BaseStation
{
	//54(118) 不需要解析
	int BaseStationVersionID;
	//56(120) 不需要解析
	int BaseStationPower;
};

struct NoloData {

	LeftController leftData;

	RightController rightData;

	HMD hmdData;

	BaseStation bsData;
	//扩充区,0位为1表示:双击
	BYTE expandData[64];
};
enum NoloDeviceType
{
	HmdDevice = 0,
	LeftControllerDevice,
	RightControllerDevice,
	BaseStationOne
};

typedef enum ExpandMsgType {

	DoubleClickMenu = 1,
	DoubleClickSystem
}ExpandMsgType;

typedef void( *expandMsg_FuncCallBack)(ExpandMsgType expandMsgType);
typedef void(*funcCallBack)();
extern "C" _declspec(dllimport) void open_Nolo_ZeroMQ();
extern "C" _declspec(dllimport) void close_Nolo_ZeroMQ();
extern "C" _declspec(dllimport) void set_Nolo_TriggerHapticPulse(NoloDeviceType deviceType, int intensity);
extern "C" _declspec(dllimport) NoloData get_Nolo_NoloData();
extern "C" _declspec(dllimport) bool disConnect_FunCallBack(funcCallBack fun);
extern "C" _declspec(dllimport) bool connectSuccess_FunCallBack(funcCallBack fun);
extern "C"  _declspec(dllimport) bool _cdecl expandDataNotify_FuncCallBack(expandMsg_FuncCallBack fun);
extern "C"  _declspec(dllimport) bool _cdecl notify_Nolo_GameBegin();
extern "C"  _declspec(dllimport) bool _cdecl notify_Nolo_GameOver();