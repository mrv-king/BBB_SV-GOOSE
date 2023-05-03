// 
// Copyright (c) 2023 Nikolai V. Galkin
// 
// Distributed under the ____ License, Version _._.
//
// This code implements generic building of a SV message from
// the raw data by using asio::generic::raw_protocol
//

#ifndef SV_HEADER_H
#define SV_HEADER_H

#define SV_ETHERNET2_VLAN_TYPE_H 	0x81
#define SV_ETHERNET2_VLAN_TYPE_L 	0x00
#define SV_VLAN_PROPERTIES_H     	0x80
#define SV_VLAN_PROPERTIES_L     	0x00
#define SV_VALN_IEC61850_SV_H 		0x88
#define SV_VALN_IEC61850_SV_L 		0xba
#define SV_APPID_H					0x40
#define SV_APPID_L					0x01
#define SV_LENGTH_H					0x00
#define SV_LENGTH_L					0x00
#define SV_RESERVED					0x00
#define SV_PDUID					0x60
#define SV_NOASDU_H					0x80
#define SV_NOASDU_L					0x01
#define SV_SEQASDU  				0xa2
#define SV_ASDU						0x30
#define SV_SVID_H					0x80
#define SV_SVID_L					0x04
#define SV_SMPCNT_H					0x82
#define SV_SMPCNT_L					0x02
#define SV_CONFREV_H				0x83
#define SV_CONFREV_L				0x04
#define SV_SMPSYNCH_H				0x85
#define SV_SMPSYNCH_L				0x01
#define SV_SEQDATA					0x87

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <iomanip>
#include <time.h>
#include <chrono>
#include <cstddef>

class sv_header
{
	public:

            void compose_frame(const std::string src_addr, const std::string dst_addr, const int APPID, const std::string data);
            std::vector<uint8_t> get_ethernetII_frame() {update_sv_frame();
	                                                     ethernetII_frame.insert(ethernetII_frame.end(), sv_frame.begin(), sv_frame.end());
		                                                 return ethernetII_frame;}
            void clear_frame() {	sv_frame.clear();
									ethernetII_frame.clear();
									it_sv_frame = 0;
									it_sv_seqData = 0;
									it_svaPdu = 0;
									it_seqASDU = 0;
									it_ASDU = 0;
									sv_length = 0;
									seqData_length = 0;
									seqASDU_length = 0;
                       }

	private:
		uint8_t convert_int2hex(int value);
		std::vector<uint8_t> ethernetII_frame;
		std::vector<uint8_t> sv_frame;
		int it_sv_frame = 0;
		int it_sv_seqData = 0;
		int it_svaPdu = 0;
		int it_seqASDU = 0;
		int it_ASDU = 0;
		int sv_length = 0;
		int seqData_length = 0;
		int seqASDU_length = 0;
		
		void set_destination_address(const std::string dst_addr);
		void set_source_address(const std::string src_addr);
		void set_EthernetII_as_8021Q_virtual_LAN() {ethernetII_frame.push_back(SV_ETHERNET2_VLAN_TYPE_H);
													ethernetII_frame.push_back(SV_ETHERNET2_VLAN_TYPE_L);}
		void set_8021Q_VLAN_properties() {ethernetII_frame.push_back(SV_VLAN_PROPERTIES_H);
										  ethernetII_frame.push_back(SV_VLAN_PROPERTIES_L);}
		void set_8021Q_VLAN_type_as_IEC61850_SV() {ethernetII_frame.push_back(SV_VALN_IEC61850_SV_H);
												   ethernetII_frame.push_back(SV_VALN_IEC61850_SV_L);}
												   
												   
		void set_SV_APPID(const int APPID);
		void set_SV_LENGTH() {sv_frame.push_back(SV_LENGTH_H);
						      sv_frame.push_back(0x00);
							  it_sv_frame = sv_frame.size();
							  sv_length += 2;}
		void set_SV_Reserved_bytes();
		void set_SV_PDU() {sv_frame.push_back(SV_PDUID);
						   sv_frame.push_back(0x00);
						   it_svaPdu = sv_frame.size();
						   sv_length += 2;}
		void set_SV_noASDU_ID() {sv_frame.push_back(SV_NOASDU_H);
								 sv_frame.push_back(SV_NOASDU_L);
								 sv_length += 2;}
		void set_SV_noASDUValue(const int noASDU);
		void set_SV_seqASDU_ID() {sv_frame.push_back(SV_SEQASDU);
								  sv_frame.push_back(0x00);
								  it_seqASDU = sv_frame.size();
								  sv_length += 2;
								  seqASDU_length = sv_length;}
		void set_SV_ASDU_ID() {sv_frame.push_back(SV_ASDU);
							   sv_frame.push_back(0x00);
							   it_ASDU = sv_frame.size();
							   sv_length += 2;}
		void set_SV_SVID_ID() {sv_frame.push_back(SV_SVID_H);
							sv_frame.push_back(SV_SVID_L);
							sv_length += 2;}
		void encode_SVID_as_ASN1(const int SVID);
		void set_SV_smpCnt_ID() {sv_frame.push_back(SV_SMPCNT_H);
							     sv_frame.push_back(SV_SMPCNT_L);
							     sv_length += 2;}
		void set_SV_smpCntValue(const int smpCnt);	
		void set_SV_confRev() {sv_frame.push_back(SV_CONFREV_H);
							   sv_frame.push_back(SV_CONFREV_L);
							   sv_length += 2;}	
		void set_SV_confRevValue(const int confRev);							   
		void set_SV_smpSynch_ID() {sv_frame.push_back(SV_SMPSYNCH_H);
							       sv_frame.push_back(SV_SMPSYNCH_L);
							       sv_length += 2;}						
		void set_SV_smpSynchValue(const int smpSynch);
		void set_SV_seqData_ID() {sv_frame.push_back(SV_SEQDATA);
						          sv_frame.push_back(0x00);
							      it_sv_seqData = sv_frame.size();
							      sv_length += 2;}
		void set_SV_seqData(const std::string seqData);	
		void update_sv_frame() {sv_frame[it_sv_frame-1]=convert_int2hex(sv_length);
								sv_frame[it_svaPdu-1]=convert_int2hex(sv_length-10);
								sv_frame[it_seqASDU-1]=convert_int2hex(sv_length-seqASDU_length);
								sv_frame[it_ASDU-1]=convert_int2hex(sv_length-seqASDU_length - 2);
								sv_frame[it_sv_seqData-1]=convert_int2hex(seqData_length);
								}		
};

#endif 