// 
// Copyright (c) 2023 Nikolai V. Galkin
// 
// Distributed under the ____ License, Version _._.
//
// This code implements generic building of GOOSE or SV messages from
// the raw data by using asio::generic::raw_protocol
//

#include <cstdlib>
#include <iostream>
#include <memory>
#include <pthread.h>
#include <utility>
#include <boost/asio.hpp>
#include <string>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <boost/format.hpp>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fstream>
#include <chrono>
#include <thread>
#include "goose_header.h"
#include "sv_header.h"

int main(int argc, char **argv)
{
    /* Check the validity of the command line */
    if (argc != 2)
    {
        printf("usage: %s interface (e.g. 'app.exe eth0\n')", argv[0]);
        return 0;
    }
    
    /* Open the output device */
    char *dev = argv[1];
	
	/* create SV instance */
	sv_header sv;
	/* create an empty vector of future SV frame */
	std::vector<uint8_t> ethernetII_frame;
	
	/* Define interface-related variables */
    const std::string port = "eth0";
    std::vector<boost::asio::const_buffer> transmitbuffer;
    sockaddr_ll sockaddr;
    typedef boost::asio::generic::raw_protocol raw_protocol_t;
    typedef boost::asio::generic::basic_endpoint<raw_protocol_t> raw_endpoint_t;
    boost::asio::io_service myio_service;
	
	/* Specify interface properties */
    memset(&sockaddr, 0, sizeof(sockaddr));
    sockaddr.sll_family = PF_PACKET;
    sockaddr.sll_protocol = htons(ETH_P_ALL);
    sockaddr.sll_ifindex = if_nametoindex(port.c_str());
    sockaddr.sll_hatype = 1;
    raw_protocol_t::socket d_socket(myio_service, raw_protocol_t(PF_PACKET, SOCK_RAW));
    d_socket.bind(raw_endpoint_t(&sockaddr, sizeof(sockaddr)));

	/* Define variables related to ADC conversion*/
    char value_str[7];
    int current_in = 0;
    int current_out = 0;
    int voltage = 0;
    std::stringstream stream;
    FILE* f0 = NULL;
    std::fstream fs;

	/* Specify P8_10 as output */
	fs.open("/sys/kernel/debug/omap_mux/gpmc_ad4");
	fs << "7";
	fs.close();
	fs.open("/sys/class/gpio/export");
	fs << "32";
	fs.close();
	fs.open("/sys/class/gpio/gpio68/direction");
	fs << "out";
	fs.close();

	bool gpio_set = false;
	
	/* Specify distination and source MAC addresses, and APPID of a SV message */
    std::string src_mac_address = "88:c2:55:72:8c:ef";
    std::string dst_mac_address = "00:00:00:00:00:00";
	int APPID = 16384;	//Application ID    0d16384 = 0x4000

	/* Read ADC0, ADC1 and ADC2 and send a SV message */
    for (int i = 0; i < 10; i++)
    {
        char hex_string[20];  

        f0 = fopen("/sys/bus/iio/devices/iio:device0/in_voltage0_raw", "r");
        fread(&value_str, 6, 1, f0);
        current_in = strtol(value_str,NULL,0);
        f0 = fopen("/sys/bus/iio/devices/iio:device0/in_voltage1_raw", "r");
        fread(&value_str, 6, 1, f0);
        current_out = strtol(value_str,NULL,0);
        f0 = fopen("/sys/bus/iio/devices/iio:device0/in_voltage2_raw", "r");
        fread(&value_str, 6, 1, f0);
        voltage = strtol(value_str,NULL,0);

        sprintf(hex_string, "0%x0%x0%x", current_in, current_out, voltage);
        sv.compose_frame(src_mac_address, dst_mac_address, APPID, std::string(hex_string));

		ethernetII_frame = sv.get_ethernetII_frame();
		

		transmitbuffer.push_back(boost::asio::buffer((const void *)&ethernetII_frame[0], ethernetII_frame.size()));
		d_socket.send(transmitbuffer);
		fs.open("/sys/class/gpio/gpio68/value");
		if (gpio_set)
			fs << "0"; // "0" for off
		else
			fs << "1"; // "1" for on
		fs.close();
		gpio_set = !gpio_set;
		sleep(1);
		sv.clear_frame();
		transmitbuffer.clear();
    }
	/* Close socket */
    d_socket.close();
	
	//
	// This snipet of code is reprodusing the socket initialization, frame filling and publishing of a GOOSE message
	//
	/*
	goose_header goose;
	std::vector<uint8_t> ethernetII_frame;
	goose_header::g_struct goose_stucture;
	
	const std::string port = "eth0";
    std::vector<boost::asio::const_buffer> transmitbuffer;
    sockaddr_ll sockaddr;
    typedef boost::asio::generic::raw_protocol raw_protocol_t;
    typedef boost::asio::generic::basic_endpoint<raw_protocol_t> raw_endpoint_t;
    boost::asio::io_service myio_service;
	
    memset(&sockaddr, 0, sizeof(sockaddr));
    sockaddr.sll_family = PF_PACKET;
    sockaddr.sll_protocol = htons(ETH_P_ALL);
    sockaddr.sll_ifindex = if_nametoindex(port.c_str());
    sockaddr.sll_hatype = 1;
    raw_protocol_t::socket d_socket(myio_service, raw_protocol_t(PF_PACKET, SOCK_RAW));
    d_socket.bind(raw_endpoint_t(&sockaddr, sizeof(sockaddr)));
	
	
	goose_stucture.gosbRef = "simpleIOGenericIO/LLN0$GO$gcbAnalogValues";
	goose_stucture.dataset = "simpleIOGenericIO/LLN0$AnalogValues";
	goose_stucture.goid = "simpleIOGenericIO/LLN0$GO$gcbAnalogValues";
	goose_stucture.timeAllowedtoLive = 500;
	goose_stucture.stNum = 1;
	goose_stucture.sqNum = 1;
	goose_stucture.simflag = false;
	goose_stucture.confRev = 1;
	goose_stucture.ndsComflag = false;

	//goose.initialize_frame("90:2E:16:2E:70:AD", "88:C2:55:72:8C:EF", goose_stucture);	
	//goose.initialize_frame("90:2E:16:2E:70:AD", "00:60:37:12:34:56", goose_stucture);
	goose.initialize_frame("90:2E:16:2E:70:AC", "00:60:37:12:34:56", goose_stucture);
	goose.set_goose_numDatSetEntries(6);			
	goose.set_goose_allData_ID();				
	goose.set_goose_value_type_int(1234);		
	goose.set_goose_value_type_binaryTime(1);	
	goose.set_goose_value_type_int(3456);		
	goose.set_goose_value_type_int(6789);
	goose.set_goose_value_type_binaryTime(2);	
	goose.set_goose_value_type_int(6789);
	
	ethernetII_frame = goose.get_ethernetII_frame();
	
    transmitbuffer.push_back(boost::asio::buffer((const void *)&ethernetII_frame[0], ethernetII_frame.size()));
    d_socket.send(transmitbuffer);
    d_socket.close();*/
	
    return 0;
}