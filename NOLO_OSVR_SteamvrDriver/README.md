# Instructions  [中文](https://github.com/NOLOVR/NOLO-Others/blob/master/NOLO_OSVR_SteamvrDriver/README_CN.md)
## Setup
1. Install [Steam & SteamVR](http://store.steampowered.com/). 

2. Install [OSVR HDK Windows Installer](http://www.osvr.org/getting-started.html).  

3. Install [NOLO-Driver-For-Windows](https://github.com/NOLOVR/NOLO-Driver-For-Windows).

4. Please back up all .dll files in "C:\Program Files\HDK-Software-Suite\OSVR-SteamVR\osvr\bin\win64'' for later use, in case restoration is needed. (If you did not back up the original .dll files, you can download them [here](https://github.com/NOLOVR/NOLO-Others/raw/master/Windows-SDK-Others/OSVR-SteamVR/OSVR-SteamVR.zip).
Then copy all .dll files from [NOLO_OSVR_SteamvrDriver](https://github.com/NOLOVR/NOLO-Others/tree/master/NOLO_OSVR_SteamvrDriver) folder to "C:\Program Files\HDK-Software-Suite\OSVR-SteamVR\osvr\bin\win64".  
    <div><img src="https://github.com/NOLOVR/NOLO-Others/blob/master/Windows-SDK-Others/picture/15.jpg"/></div>

5. If you have Riftcat software installed, please follow the steps shown in the image below.
    <div><img src="https://github.com/NOLOVR/NOLO-Others/blob/master/Windows-SDK-Others/picture/14.jpg"/></div>  

6. Run NOLO Driver for Windows and Turn on the base station by pressing the power button, then connect the NOLO headset marker to the PC (via USB).  

7. Connect OSVR HDK2 to PC.  

8. Run OSVR Server and click start. Place OSVR HDK2 facing towards NOLO base station, then click "Recenter" on OSVR Server.
    <div><img src="https://github.com/NOLOVR/NOLO-Others/blob/master/Windows-SDK-Others/picture/16.jpg"/></div>  

9. Run steamvr.  

10. If error occurs on Steam VR, please copy the “steamvr.vrsettings” file from "NOLO_OSVR_SteamvrDriver" folder to "X: \Steam\config".
    <div><img src="https://github.com/NOLOVR/NOLO-Others/blob/master/Windows-SDK-Others/picture/17.jpg"/></div> 

11. Place the headset marker horizontally on the ground, then press the pair button on it to complete Room Setup. If you want to use Steam VR built-in Room Setup instead, please place the headset marker horizontally on the ground, facing towards the base station at a distance of 1.5m, and then to complete the Room Setup.  

12. Double click the power/system button on either controller to calibrate the controllers' orientation (Controllers must point towards the base station at this step). Double click the menu button on it to make a 180 degree turn in the virtual world.

13. Choose a SteamVR game on PC and enjoy NOLO.
#
## All Downloads
- [Steam & SteamVR](http://store.steampowered.com/)
- [OSVR HDK Windows Installer](http://www.osvr.org/getting-started.html)
- [NOLO_OSVR_SteamvrDriver](https://github.com/NOLOVR/NOLO-Others/tree/master/NOLO_OSVR_SteamvrDriver)
- [NOLO-Driver-For-Windows](https://github.com/NOLOVR/NOLO-Driver-For-Windows)
#