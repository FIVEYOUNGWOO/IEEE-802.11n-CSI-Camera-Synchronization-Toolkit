# IEEE-802.11n-CSI-Camera-Synchronization-Toolkit
* This repository is part of an early-stage WiFi and camera data fusion-based human detection and motion estimation project.
* Our research aims to develop a multiple modality-based computer vision technology to detect objects beyond walls by adding IEEE 802.11 standard WiFi signals.
* A multimodal approach utilizing WiFi signals can complement problems such as obstacles and object obscuration in existing camera-based security and computer vision through the radio frequency (RF) signal characteristics.

This repository supports building the channel state information (CSI)-enabled **TP-Link AC1750** Wi-Fi drivers for **Intel 5300 NIC** adapters on Linux distributions with **Ubuntu 14.01** versions. This code has been tested on Ubuntu **14.01**, **16.04**, and **18.04**, where we recommend setting up the CSI toolkit on the **Ubuntu 18.04 version**.

The code presented here comprises a modified and merged version of the Linux kernel, such as the firmware command, and error lines of the original baseline. The modifications were made by examining the code provided by [dhalperi/linux-80211n-csitool](https://github.com/dhalperi/linux-80211n-csitool) and adapting them to more recent Linux kernel versions **-iwlwifi** and **wlp1s0** wireless communication modules. The building and installation instructions were taken from [the original Linux 802.11n CSI Tool website](https://dhalperi.github.io/linux-80211n-csitool/) and adapted accordingly. Moreover, I referred to the synchronization method between CSI and Camera from [CSI-Tool-Camera-Shooting](https://github.com/qiyinghua/CSI-Tool-Camera-Shooting).

Researchers in the field of mobility detection using WiFi-CSI have discussed the lack of support for signal processing, analysis, and visualization methods in conventional CSI toolkits. To address this issue, we developed **newly_csi_analyzer.m**, a function designed for signal processing and visualization of collected CSI samples. This function includes a process for decomposing the amplitude and phase characteristics inherent in complex numbers ($a+bi$) in WiFi-CSI data. The visualization step incorporates advanced signal processing techniques, such as phase unwrapping filters, uniform fitting, median filtering, and linear regression fitting, enabling more accurate and insightful analysis. In developing this approach, I referred to methodologies introduced in [DensePose From WiFi](https://arxiv.org/abs/2301.00250) and [Can WiFi Estimate Person Pose?](https://arxiv.org/abs/1904.00277).

# Desktop-Based Experimental Configuration (Pre-miniaturization)
- The toolkit demonstration video is uploaded to [YouTube channel](https://www.youtube.com/watch?v=X-kNQRrQUlE).

<table>
  <tr>
    <td><img src="/README_images/configulation 1.jpg" width="300" height="180"/></td>
    <td><img src="/README_images/configulation 2.jpg" width="300" height="180"/></td>
    <td><img src="/README_images/configulation 3.jpg" width="300" height="180"/></td>
  </tr>
</table>

# Miniaturization of H/W Setups for Portable Experimental Configuration
* Our previous hardware setup consisted of a desktop-based configuration, which limited experimental flexibility in various environments. To this end, we miniaturized the conventional desktop-based setup into an embedded hardware system. We reduced computational and memory complexity by leveraging an embedded board(_Up-Squared Board_) compatible with the Intel 5300 WLAN card and eliminating unnecessary S/W modules such as various real-time WiFi signal analysis and visualization processes. The experimental results demonstrated that our portable configuration can successfully acquire synchronized multimodal data pairs.

<table>
  <tr>
    <td><img src="/README_images/configulation 1.jpg" width="300" height="220"/></td>
    <td><img src="/README_images/portable_configuration.png" width="300" height="220"/></td>
    <td><img src="/README_images/configulation_results.png" width="300" height="220"/></td>
  </tr>
</table>

# WiFi signals and human posture correlation
- The experimental results demonstrate that WiFi signal characteristics vary according to human postures. Specifically, the difference in phase values between standing and sitting postures is more pronounced. This suggests that the distinct signal features can be employed for accurately training multi-modal learning models for object detection, tracking, and pose estimation without solely relying on vision information.

<table>
  <tr>
    <td><img src="/README_images/handsup.jpg" width="300" height="180"/></td>
    <td><img src="/README_images/standsup.jpg" width="300" height="180"/></td>
    <td><img src="/README_images/sitdown.jpg" width="300" height="180"/></td>
  </tr>
  <tr>
    <td><img src="/README_images/handsup_plot.png" width="300" height="180"/></td>
    <td><img src="/README_images/standup_plot.png" width="300" height="180"/></td>
    <td><img src="/README_images/sitdown_plot.png" width="300" height="180"/></td>
  </tr>
</table>

# Project Members
#### [Youngwoo Oh](https://ohyoungwoo.com/) (M.S. student, Project leader from May 2023 to Feb. 2024 (for 10 months))
- Integrated sensor fusion between the WiFi and captured image from the TP-Link router and USB camera by developing the [Linux toolkit codes](https://github.com/FIVEYOUNGWOO/IEEE-802.11n-CSI-Camera-Synchronization-Toolkit).
- Responsible for sensor fusion and SW/HW configuration and produced a Teacher-Student architecture to detect and track objects beyond walls and obstacles.
- Wrote papers for the [2024 Winter Conference on Korea Information and Communications Society (KICS)](https://conf.kics.or.kr/) titled "*Collection and Analysis of CSI in IEEE 802.11n Wireless LAN Environments for WiFi Signal-Based Human Mobility Detection*" and "*Design and Implementation of a MultiModal Learning Model for RF-Based Object Tracking Methods*".
  
#### Islam Helemy (Ph.D. student, Project member)
- Responsible for the development of the Multi-modal AI and the pre-processing to generate training data pairs (CSI samples-captured images).
- He will receive a *project leader* position on this future project after Mar. 2024.

#### Iftikhar Ahmad (Ph.D. student, Project member)
- Focused on developing the Teacher network in the multi-modal AI model.

#### Manal Mosharaf (M.S. student, Project member)
- Engaged in developing the Student network in the multi-modal AI model.

#### [Jungtae Kang](https://kangjeongtae.com/) (Undergraduate student, Project follower)
- Supporting the generation of CSI samples and Camera images.

# 1. Installation instructions of integrated CSI toolkit
## (1). Kernel version:
Before proceeding further, you need to check the version of your kernel. It should be **4.15**, otherwise, the commands below won't work. The following command will print that information:
```ruby
uname -r
```

## (2). First download the essential packages:
```ruby
sudo apt-get install build-essential linux-headers-$(uname -r) git-core
```
```ruby
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
```
```ruby
sudo apt-get update
```
If you don't need to install this gcc version, skip the below line :)
```ruby
sudo apt-get install gcc-8 g++-8
```
```ruby
sudo apt-get install libgtk2.0-dev
```
```ruby
sudo apt-get install pkg-config 
```
```ruby
sudo apt-get install cmake
```
```ruby
sudo apt update
```

## (3). Check the GCC version:
```ruby
ls -l /usr/bin/gcc /usr/bin/g++
```

**Now change the version of gcc/g++ to version 8:**

```ruby
sudo rm /usr/bin/gcc
```
```ruby
sudo rm /usr/bin/g++
```
```ruby
sudo ln -s /usr/bin/gcc-8 /usr/bin/gcc
```
```ruby
sudo ln -s /usr/bin/g++-8 /usr/bin/g++
```

## (4). Build and install the modified wireless LAN driver:
```ruby
sudo apt-get install gcc make linux-headers-$(uname -r) git-core
```
```ruby
CSITOOL_KERNEL_TAG=csitool-$(uname -r | cut -d . -f 1-2)
```
> **This step is not necessary.**  
The command is for setting up the environment and only works with the files from the [https://github.com/spanev/linux-80211n-csitool](https://github.com/spanev/linux-80211n-csitool) repository.
```ruby
git clone https://github.com/FIVEYOUNGWOO/IEEE-802.11n-CSI-Camera-Synchronization-Toolkit.git
```
```ruby
cd IEEE-802.11n-CSI-Camera-Synchronization-Toolkit/linux-80211n-csitool
```
```ruby
git checkout ${CSITOOL_KERNEL_TAG}
```
> **This command will not work unless the necessary steps are followed.**  
It only works with the files from the [https://github.com/spanev/linux-80211n-csitool](https://github.com/spanev/linux-80211n-csitool) repository, and there must be a tag for `csitool-4.15` in that repository.
```ruby
make -C /lib/modules/$(uname -r)/build M=$(pwd)/drivers/net/wireless/iwlwifi modules
```
> **Prior steps are necessary!**  
To execute this command successfully, you must overwrite all the files in the 'IEEE-802.11n-CSI-Camera-Synchronization-Toolkit/linux-80211n-csitool/drivers/net/wireless/' directory with the corresponding 'spanev' files from 'linux-80211n-csitool/drivers/net/wireless/intel/' directory.


> Download the files from [https://github.com/spanev/linux-80211n-csitool](https://github.com/spanev/linux-80211n-csitool) and follow the instructions to overwrite the files as described above.
```ruby
sudo make -C /lib/modules/$(uname -r)/build M=$(pwd)/drivers/net/wireless/iwlwifi INSTALL_MOD_DIR=updates
```

## (5). Install the Modified firmware:
```ruby
for file in /lib/firmware/iwlwifi-5000-*.ucode; do sudo mv $file $file.orig; done
```
```ruby
sudo cp IEEE-802.11n-CSI-Camera-Synchronization-Toolkit/supplementary/firmware/iwlwifi-5000-2.ucode.sigcomm2010 /lib/firmware/
```
> **To execute this command successfully, change the terminal's directory to 'home'.**  
```ruby
sudo ln -s iwlwifi-5000-2.ucode.sigcomm2010 /lib/firmware/iwlwifi-5000-2.ucode
```

## (6). Build the userspace logging tool:
```ruby
make -C IEEE-802.11n-CSI-Camera-Synchronization-Toolkit/supplementary/netlink
```
> **To execute this command successfully, change the terminal's directory to 'home'.**  

## (7). Unzip OpenCV to utilize the USB camera:
```ruby
cd IEEE-802.11n-CSI-Camera-Synchronization-Toolkit/camera_tool
```
```ruby
unzip opencv-2.4.13.6.zip
```
> **After running the command, press the 'A' key to extract all the files.**  
```ruby
cd opencv-2.4.13.6/install
```

## (8). Compile and install OpenCV:
```ruby
cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local ..
```
```ruby
cmake ..
```
```ruby
make
```
```ruby
sudo make install
```

## (9). Configuration OpenCV:
```ruby
sudo gedit /etc/ld.so.conf.d/opencv.conf
```
> **When you run this command, a 'gedit' (text editor) window will open.**  
```ruby
/usr/local/lib
```
> **Write the above content in the editor, then save the file.**  
Then add the /usr/local/lib command to the file.
```ruby
sudo ldconfig
```
```ruby
sudo gedit /etc/bash.bashrc
```
> **When you run this command, a 'gedit' (text editor) window will open.**  
```ruby
PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/usr/local/lib/pkgconfig
export PKG_CONFIG_PATH
```
> **Write the above content in the editor, then save the file.**  
Then add the following command to the end of the file:
```ruby
sudo reboot
```

## (10). Compile user-application:
```ruby
cd IEEE-802.11n-CSI-Camera-Synchronization-Toolkit/supplementary/netlink
```
```ruby
sudo gedit /etc/ld.so.conf.d/opencv.conf
```
> **When you run this command, a 'gedit' (text editor) window will open.**  
```ruby
/usr/local/lib
```
> **Write the above content in the editor, then save the file.**  
Then add the /usr/local/lib command to the file.
```ruby
make
```

# 2. Guideline Operation of integrated CSI toolkit
## (1). setting of the WiFi router and wireless LAN card:
The below comments (1) only need to be performed once after turning on the computer.
```ruby
iw dev wlp1s0 link
```
```ruby
sudo modprobe -r iwlwifi mac80211
```
```ruby
sudo modprobe iwlwifi connector_log=0x1
```
The below comment returns the MAC address, such as XX:XX:XX:XX:XX:XX.
```ruby
sudo ls /sys/kernel/debug/ieee80211/phy0/netdev:wlp1s0/stations/
```
Check the supported modulation, and transmission rate table from the connected WiFi
```ruby
sudo cat /sys/kernel/debug/ieee80211/phy0/netdev:wlp1s0/stations/XX:XX:XX:XX:XX:XX/rate_scale_table
```
Set the transmit rate from the checked table. In our case, we set it as 0x4007 (16-QAM, 1/2).
```ruby
echo 0x4007 | sudo tee /sys/kernel/debug/ieee80211/phy0/netdev:wlp1s0/stations/XX:XX:XX:XX:XX:XX/rate_scale_table
```
```ruby
echo 0x4007 | sudo tee /sys/kernel/debug/ieee80211/phy0/iwlwifi/iwldvm/debug/bcast_tx_rate
```
```ruby
echo 0x4007 | sudo tee /sys/kernel/debug/ieee80211/phy0/iwlwifi/iwldvm/debug/monitor_tx_rate
```

## (2). Starting this toolkit program (utilized three kernel terminals):
```ruby
sudo dhclient wlp1s0
```
comment under kernel terminal, 

where the camera parameters constructed [*Camera ID*] [*Save Interval*] [*Auto Exit*]

[*Camera ID*]: This parameter controls which camera to use when the computer has multiple cameras. When set to 0, the program will use the first camera. When set to 1, the program will use the second camera.

[*Save Interval*]: This parameter controls the speed at which images are saved. When set to 0, the program will save each frame of the camera. When set to 1, the program will save an image every other frame.

[*Auto Exit*]: This parameter controls whether the program automatically exits when CSI collects stops. When set to 0, This program will always run. When set to 1, This program will automatically exit when no CSI is acquired within 1 second.

Open the first Linux kernel and write the below command:
```ruby
cd IEEE-802.11n-CSI-Camera-Synchronization-Toolkit/supplementary/netlink/camera 0 1 0
```
Open the second Linux kernel to execute 'log_to_file':
```ruby
cd IEEE-802.11n-CSI-Camera-Synchronization-Toolkit/supplementary/netlink/log_to_file test.dat
```
Open another kernel terminal, where ping testing is a way to encourage the collection of more CSI samples:
```ruby
ping 192.xxx.xx.xx -i 0.3
```
