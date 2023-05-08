# About
This code is about how to build a BeagleBone Black executable file of an application that sends IEC 61850-related SV or GOOSE messages through the eth0 interface.

# Prerequisites
To build the app on a BeagleBone device, make sure the following components are installed first:
1. cmake. To install it, run: "sudo apt-get install cmake"
2. libbost. To install it, run: "sudo apt-get install libboost-all-dev"
3. Wireshark should be installed in the host PC (from which you are going SSH the BeagleBone Black).

# How to build
1. SSH into the BeagleBone Black (for example PuTTY)
2. copy "BBB_SV-GOOSE" folder on your BBB.
3. go to "~/BBB_SV-GOOSE/build" folder (if it is lokated in root), i.e. "cd ~/BBB_SV-GOOSE/build".
4. run make, i.e. "make .."
5. If make is finished successfully (iec61850 target is built), change its mode, i.e. "sudo chmod +x iec61850"
6. When it is done, run application, i.e. "sudo ./iec61850 eth0"
7. On your host machine (from which you SSH the BeagelBone) run the Wireshark application you should be able to see SV or GOOSE messages.

Note that in the existing scenario (defined in the "BBB_SV-GOOSE/source/main.cpp" file, the BBB will send 10 SV messages with a delay of 1 second), i.e.

![alt text](https://github.com/mrv-king/BBB_SV-GOOSE/blob/main/SV_BBB.PNG)

If you want to run another scenario, you need to change "main.cpp" file only.

# Screenshot of the build process (for some one, just in case)
For those of you who are not familiar with cmake, here is a screenshot of the build process. Please note that the repository will be updated during code development, which means that the screenshot below may be different.

![alt text](https://github.com/mrv-king/BBB_SV-GOOSE/blob/main/build_BBB.PNG)

