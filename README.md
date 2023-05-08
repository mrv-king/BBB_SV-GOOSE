# About
This code is about how to build a BeagleBone Black executable file of an application that sends IEC 61850-related SV or GOOSE messages through the eth0 interface.

# Prerequisites
1. cmake. To install it, run: "sudo apt-get install cmake"
2. libbost. To install it, run: "sudo apt-get install libboost-all-dev"

# How to build
1. copy "BBB_SV-GOOSE" folder on your BBB.
2. go to "~/BBB_SV-GOOSE/build" folder (if it is lokated in root), i.e. "cd ~/BBB_SV-GOOSE/build".
3. run make, i.e. "make .."
4. If make is finished successfully (iec61850 target is built), change its mode, i.e. "sudo chmod +x iec61850"
5. When it is done, run application, i.e. "sudo ./iec61850 eth0"

Please not that in the existing scenario (defined in "BBB_SV-GOOSE/source/main.cpp" file the BBB will send 10 SV messages with a delay of 1 sec), i.e.

![alt text](https://github.com/mrv-king/BBB_SV-GOOSE/blob/main/SV_BBB.PNG)

If you want to run another scenario, you need to change "main.cpp" file only

