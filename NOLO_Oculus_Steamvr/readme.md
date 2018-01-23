# Instructions
1. Install [Steam & SteamVR](http://store.steampowered.com/about/).  

2. Install [HDK Windows Installer](http://www.osvr.org/getting-started.html).   

3. Install [Nolo_driver_for_windows_setup.msi](https://github.com/NOLOVR/NOLO-Driver-For-Windows/tree/master/NOLOVR).  

4. Download the [NOLO_Oculus_Steamvr](https://github.com/NOLOVR/NOLO-Others) folder.

5. Copy all .json files from the "OSVR-Oculus" folder to the "OSVR-Core/bin" folder.  

6. Copy the "je_nourish_openhmd.dll" file from the "OSVR-Oculus" folder to the "OSVR-Core/bin/osvr-plugins-0" folder.  

7. Copy all .dll files in the "NOLO_OSVR_SteamvrDriver" folder into the OSVR-SteamVR/osvr/bin/win64 folder.   

8. Copy "steamvr.vrsetting" file from the "OSVR-OCulus" folder to the steam/config folder.  

9. Connect Oculus DK2 (headset) to PC.

10. Turn off all Oculus Driver services.  

11. Turn on all NOLO devices, then connect the NOLO headset marker to PC.  
12. Run NOLO Driver.  

13. Run OSVR Server. ("osvr_server.exe" from folder "OSVR-Core/bin") 

14. Run SteamVR. 

15. Place Oculus devices facing towards Nolo base stateion, then run "osvr_reset_yaw.exe" from folder "OSVR-Core/bin".

### * [Video tutorial](https://www.youtube.com/watch?v=qgL7NHixIX8)  
#