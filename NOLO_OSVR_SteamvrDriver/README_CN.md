# 使用说明  
## 安装步骤

1. 安装 [Steam 和 Steamvr](http://store.steampowered.com/).

2. 安装 [HDK Windows Installer](http://www.osvr.org/getting-started.html).  

3. 安装 [NOLO Driver for Windows](https://github.com/NOLOVR/NOLO-Driver-For-Windows).

4. 下载 [NOLO_OSVR_SteamvrDriver](https://github.com/NOLOVR/NOLO-Others/tree/master/NOLO_OSVR_SteamvrDriver) 文件夹并将里面的dll插件复制到“C:\Program Files\HDK-Software-Suite\OSVR-SteamVR\osvr\bin\win64”文件夹中,并备份原有的dll以便之后还原使用, 如果没有备份请从[此处](https://github.com/NOLOVR/NOLO-Others/raw/master/Windows-SDK-Others/OSVR-SteamVR/OSVR-SteamVR.zip)下载.  
    <div><img src="https://github.com/NOLOVR/NOLO-Others/blob/master/Windows-SDK-Others/picture/15.jpg"/></div>

5. 如果安装了riftcat软件的用户请按下图操作.
    <div><img src="https://github.com/NOLOVR/NOLO-Others/blob/master/Windows-SDK-Others/picture/14.jpg"/></div>

6. 运行NOLO PC软件,启动Nolo基站,连接Nolo头盔定位器到电脑.   

7. 连接OSVR头盔到电脑  

8. 运行OSVR Server,点击Start,然后将OSVR头盔对准NOLO基站,点击OSVR Server软件上的"Recenter".
    <div><img src="https://github.com/NOLOVR/NOLO-Others/blob/master/Windows-SDK-Others/picture/16.jpg"/></div>  

9. 运行Steamvr.  

10. 如果有404错误或未就绪则将NOLO_OSVR_SteamvrDriver文件夹里的"steamvr.vrsettings"文件复制到"X: \Steam\config"文件夹中.
    <div><img src="https://github.com/NOLOVR/NOLO-Others/blob/master/Windows-SDK-Others/picture/17.jpg"/></div>  

11. 将NOLO头盔定位器放置地面，并按下头盔按钮进行房间设置（设置地面高度）,如果要进行Steamvr自带的房间设置请将NOLO头盔设备放置地下并里基站1.5米远对准基站进行房间设置步骤. 

12. 将手柄对准基站，双击手柄上的电源键进行手柄标定，双击menu按键则是一键转身.  
#
## 所有下载链接
- [Steam & SteamVR](http://store.steampowered.com/)
- [OSVR HDK Windows Installer](http://www.osvr.org/getting-started.html)
- [NOLO_OSVR_SteamvrDriver](https://github.com/NOLOVR/NOLO-Others/tree/master/NOLO_OSVR_SteamvrDriver)
- [NOLO-Driver-For-Windows](https://github.com/NOLOVR/NOLO-Driver-For-Windows)
#