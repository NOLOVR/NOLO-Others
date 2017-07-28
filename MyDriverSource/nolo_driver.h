// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 NOLO_DRIVER_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// NOLO_DRIVER_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#define HMD_DLL_EXPORT extern "C" __declspec(dllexport)
#ifdef _DEBUG
#pragma comment(lib,"..\\x64\\debug\\noloRuntime.lib")

#else
#pragma comment(lib,"..\\x64\\release\\noloRuntime.lib")
#endif // DEBUG
//-----------------------------------------------------------------------------
// Purpose:steam调入入口，建立一个CServerDriver_Nolo，一个CClientDriver_Nolo
//-----------------------------------------------------------------------------
HMD_DLL_EXPORT void *HmdDriverFactory(const char *pInterfaceName, int *pReturnCode);

