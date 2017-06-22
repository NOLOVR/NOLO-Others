#
1, install steam and steamvr software.  
2, Install the [HDK Windows Installer](https://www.reddit.com/r/OSVR/comments/67hqrf/hdk_windows_installer_beta_080_released/) software.  
3, Install [Nolo_driver_for_windows_setup.msi](https://github.com/NOLOVR/NOLO-Driver-For-Windows/tree/master/NOLOVR).  
4,  Copy all the Json files in the OSVR-Oculus folder into the OSVR-Core / bin folder.  
5, Copy the je_nourish_openhmd.dll file from the OSVR-Oculus directory to the OSVR-Core / bin / osvr-plugins-0 folder.  
6, Copy all dll files under the [NOLO_OSVR_SteamvrDriver](https://github.com/NOLOVR/NOLO-Others/tree/master/NOLO_OSVR_SteamvrDriver) directory into the OSVR-SteamVR / osvr / bin / win64 folder.   
7, Copy the steamvr.vrsetting file from the OSVR-OCulus folder to the steam / config folder.  
8, connect Oculus to PC.  
9, turn off all Oculus driver service software.  
10, connect the NOLO device to the PC.  
11, run NOLO.  
12. Run osvr_server.exe in the OSVR-Core / bin folder. 
13, run Steamvr.  
14, the Oculus device looked at the NOLO base station equipment, run OSVR-Core / bin folder osvr_reset_yaw.exe.
#