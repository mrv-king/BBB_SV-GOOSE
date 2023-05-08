# About
This code is about how to build a BeagleBone Black executable file of an application that sends IEC 61850-related SV or GOOSE messages through the eth0 interface.

# How to build
1. copy "BBB_SV-GOOSE" folder on your BBB.
2. go to "~/BBB_SV-GOOSE/build" folder (if it is lokated in root), i.e. "cd ~/BBB_SV-GOOSE/build".
3. run make, i.e. "make .."
4. If make is finished successfully (iec61850 target is built), change its mode, i.e. "sudo chmod +x iec61850"
5. When it is done, run application, i.e. "sudo ./iec61850 eth0"

Please not that in the existing scenario (defined in "BBB_SV-GOOSE/source/main.cpp" file the BBB will send 10 SV messages with a delay of 1 sec). If you want to run another scenario, you need to change "main.cpp" file only.

# How to build example
debian@beaglebone:~/niko$ cd build
debian@beaglebone:~/niko/build$ ls
libIEC61850LIB.so
debian@beaglebone:~/niko/build$ cmake ..
-- The C compiler identification is GNU 8.3.0
-- The CXX compiler identification is GNU 8.3.0
-- Check for working C compiler: /usr/bin/cc
-- Check for working C compiler: /usr/bin/cc -- works
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Detecting C compile features
-- Detecting C compile features - done
-- Check for working CXX compiler: /usr/bin/c++
-- Check for working CXX compiler: /usr/bin/c++ -- works
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Boost version: 1.67.0
-- Found the following Boost libraries:
--   system
-- Found Python: /usr/bin/python3.7 (found version "3.7.3") found components:  I                                                                                                                                                             nterpreter Development
-- Configuring done
-- Generating done
-- Build files have been written to: /home/debian/niko/build
debian@beaglebone:~/niko/build$ make
Scanning dependencies of target iec61850
[ 50%] Building CXX object CMakeFiles/iec61850.dir/source/main.cpp.o
[100%] Linking CXX executable iec61850
[100%] Built target iec61850
debian@beaglebone:~/niko/build$ sudo chmod +x iec61850
[sudo] password for debian:
debian@beaglebone:~/niko/build$ sudo ./iec61850 eth0
