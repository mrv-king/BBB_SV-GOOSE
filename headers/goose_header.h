// 
// Copyright (c) 2023 Nikolai V. Galkin
// 
// Distributed under the ____ License, Version _._.
//
// This code implements generic building of a GOOSE message from
// the raw data by using asio::generic::raw_protocol
//


#ifndef GOOSE_HEADER_H
#define GOOSE_HEADER_H

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <iomanip>
#include <time.h>
#include <chrono>
#include <cstddef>

#define ETHERNET2_VLAN_TYPE_H 	0x81
#define ETHERNET2_VLAN_TYPE_L 	0x00
#define VLAN_PROPERTIES_H     	0x80
#define VLAN_PROPERTIES_L     	0x00
#define VALN_IEC61850_GOOSE_H 	0x88
#define VALN_IEC61850_GOOSE_L 	0xb8
#define GOOSE_APPID_H			0x03
#define GOOSE_APPID_L			0xe8
#define GOOSE_RESERVED			0x00
#define GOOSE_PDUID_B1			0x61
#define GOOSE_PDUID_B2			0x81
#define GOSB_REFID_H			0x80
#define GOSB_REFID_L			0x29
#define GOOSPDU_TIME2LIVE_H		0x81
#define GOOSPDU_TIME2LIVE_L		0x02
#define DATASETID_H				0x82
#define DATASETID_L				0x23
#define GOID_H					0x83
#define GOID_L					0x29
#define T_H						0x84
#define T_L						0x08
#define STNUMID_H				0x85
#define STNUMID_L				0x01
#define SQNUMID_H				0x86
#define SQNUMID_L				0x01
#define SIMULATION_H			0x87
#define SIMULATION_L			0x01
#define CONFREF_H				0x88
#define CONFREF_L				0x01
#define NDSCOMID_H				0x89
#define NDSCOMID_L				0x01
#define NUMDATSETENTRIES_H		0x8a
#define NUMDATSETENTRIES_L		0x01
#define ALLDATA					0xab
#define GOOSE_TYPE_INT_H		0x85
#define GOOSE_TYPE_INT_L		0x02
#define GOOSE_TYPE_BINTIME_H	0x8c
#define GOOSE_TYPE_BINTIME_L	0x06

using fractions = std::chrono::duration<std::int64_t, std::ratio<1, 0x100000000>>; 
using namespace std::chrono_literals;

class goose_header
{	
	public:
		struct g_struct{
			std::string gosbRef;
			std::string dataset;
			std::string goid;
			int timeAllowedtoLive;
			int stNum;
			int sqNum;
			bool simflag;
			int confRev;
			bool ndsComflag;
		};

		void initialize_frame(const std::string src_addr, const std::string dst_addr, g_struct user_structure);

		std::vector<uint8_t> get_ethernetII_frame() {get_goose_frame();
							     ethernetII_frame.insert(ethernetII_frame.end(), goose_frame.begin(), goose_frame.end());
							     return ethernetII_frame;}
		void set_goose_numDatSetEntries(const int confRev);
		void set_goose_allData_ID() {goose_frame.push_back(ALLDATA);
									 goose_frame.push_back(0x00);
									 goosePdu_bit_counter +=2;
									 it_allData = goose_frame.size();}
		void set_goose_value_type_int(const int value);
		void set_goose_value_type_binaryTime(const int value);	
			
	private:
		uint8_t convert_int2hex(int value);
		void calculate_goose_frame_length(int value);		
		std::vector<uint8_t> ethernetII_frame;
		std::vector<uint8_t> goose_frame;
		int it_goosePdu = 0;
		int it_allData = 0;		
		int alldata_bit_counter = 0;
		int goosePdu_bit_counter = 0;
		
		void set_destination_address(const std::string dst_addr);
		void set_source_address(const std::string src_addr);
		void set_EthernetII_as_virtual_LAN() {ethernetII_frame.push_back(ETHERNET2_VLAN_TYPE_H);
											    ethernetII_frame.push_back(ETHERNET2_VLAN_TYPE_L);}
		void set_VLAN_properties() {ethernetII_frame.push_back(VLAN_PROPERTIES_H);
										  ethernetII_frame.push_back(VLAN_PROPERTIES_L);}
		void set_VLAN_type_as_GOOSE() {ethernetII_frame.push_back(VALN_IEC61850_GOOSE_H);
												  ethernetII_frame.push_back(VALN_IEC61850_GOOSE_L);}		
		
		void set_GOOSE_APPID() {goose_frame.push_back(GOOSE_APPID_H);
							goose_frame.push_back(GOOSE_APPID_L);}
		void set_GOOSE_Reserved_bytes();
		void set_GOOSE_PDU_ID() {goose_frame.push_back(GOOSE_PDUID_B1); goose_frame.push_back(GOOSE_PDUID_B2); goose_frame.push_back(0xff); it_goosePdu = goose_frame.size();}
		void set_GOOSE_CBREFID() {goose_frame.push_back(GOSB_REFID_H);
							  goose_frame.push_back(GOSB_REFID_L);
							  goosePdu_bit_counter +=2;}
		void set_gosbRef(const std::string gosbRef);
		void set_goosePDU_Time2Live_ID() {goose_frame.push_back(GOOSPDU_TIME2LIVE_H);
									  goose_frame.push_back(GOOSPDU_TIME2LIVE_L);
									  goosePdu_bit_counter +=2;}
		void set_timeAllowedtoLive(const int timeAllowedtoLive);		
		void set_goose_dataset_ID() {goose_frame.push_back(DATASETID_H);
							     goose_frame.push_back(DATASETID_L);
							     goosePdu_bit_counter +=2;}
		void set_goose_dataset(const std::string dataset);
		void set_goose_goid_ID() {goose_frame.push_back(GOID_H);
							  goose_frame.push_back(GOID_L);
							  goosePdu_bit_counter +=2;}
		void set_goose_goid(const std::string goid);
		void set_goose_t_ID() {goose_frame.push_back(T_H);
						   goose_frame.push_back(T_L);
						   goosePdu_bit_counter +=2;}	
		void generate_currentTime_in_UTC_frame();
		void set_goose_stNum() {goose_frame.push_back(STNUMID_H);
							goose_frame.push_back(STNUMID_L);
						    goosePdu_bit_counter +=2;}
		void set_goose_stNumValue(const int intstNum);

		void set_goose_sqNum() {goose_frame.push_back(SQNUMID_H);
							goose_frame.push_back(SQNUMID_L);
					        goosePdu_bit_counter +=2;}
		void set_goose_sqNumValue(const int sqNum);
		void set_goose_simulation() {goose_frame.push_back(SIMULATION_H);
							     goose_frame.push_back(SIMULATION_L);
							     goosePdu_bit_counter +=2;}
		void set_goose_simulation_flag(const bool simflag);		
		void set_goose_confRev_ID() {goose_frame.push_back(CONFREF_H);
								 goose_frame.push_back(CONFREF_L);
							     goosePdu_bit_counter +=2;}
		void set_goose_confRev(const int confRev);
		void set_goose_ndsCom_ID() {goose_frame.push_back(NDSCOMID_H);
									goose_frame.push_back(NDSCOMID_L);
									goosePdu_bit_counter +=2;}
		void set_goose_ndsCom_flag(const bool ndsComflag);							
		void set_goose_numDatSetEntries_ID() {goose_frame.push_back(NUMDATSETENTRIES_H);
											  goose_frame.push_back(NUMDATSETENTRIES_L);
											  goosePdu_bit_counter +=2;}
		void get_goose_frame() {calculate_goose_frame_length(goose_frame.size());
								goose_frame[it_goosePdu-1]=convert_int2hex(goosePdu_bit_counter);
								goose_frame[it_allData-1]=convert_int2hex(alldata_bit_counter);}

};

#endif 