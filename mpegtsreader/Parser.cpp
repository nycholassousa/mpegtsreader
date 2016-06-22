#include "Parser.h"

#include <iostream>
//PMT SÓ APARECE QUANDO PAT TABLE_ID = 2

void printbincharpad(char c)
{
	for (int i = 7; i >= 0; --i)
	{
		putchar((c & (1 << i)) ? '1' : '0');
	}
	putchar('\n');
}

Parser::Parser(std::string videoFile) {
	fileName = videoFile;
	packetSize = 188;
	packet = new unsigned char[188]();
	
	reader.open(fileName, std::ifstream::in | std::ifstream::binary);
	if (!reader.is_open()) {
		throw "Could not open file";
	}
}

Parser::~Parser() {
	reader.close();
	delete packet;
}

void Parser::readBytes() {
	reader.read((char*)(&packet[0]), packetSize);
	
	if (packet[0] != 0x47)
		readBytes();
	else {
		std::cout << std::endl;
		std::cout << "sync_byte = " << "0x47" << std::endl;
		std::cout << "transport_error_indicator = " << ((packet[1] & 0x80) >> 7) << std::endl;
		std::cout << "payload_unit_start_indicator = " << ((packet[1] & 0x40) >> 6) << std::endl;
		std::cout << "transport_priority = " << ((packet[1] & 0x20) >> 5) << std::endl;
		std::cout << "PID = " << (((packet[1] & 0x1F) << 8) | packet[2]) << std::endl;

		if ((((packet[1] & 0x1F) << 8) | packet[2]) == 0) {
			std::cout << "\tDue to the PID is 0:"<< std::endl;
			std::cout << "\tPAT table_id = " << (unsigned int)packet[5] << std::endl;
			std::cout << "\tPAT section_syntax_indicator = " << ((packet[6] & 0x80) >> 7) << std::endl;

			//std::cout << "\tPAT reserved1 = " << ((packet[6] & 0x20) >> 5) << std::endl;

			std::cout << "\tPAT section_lenght = " << ((packet[6] & 0x03) | packet[7]) << std::endl;
			std::cout << "\tPAT transport_stream_id = " << (packet[8] | packet[9]) << std::endl;
			std::cout << "\tPAT version_number = " << (unsigned int)((packet[10] & 0x3E) >> 1) << std::endl;
			std::cout << "\tPAT current_next_indicator = " << (packet[10] & 0x01) << std::endl;
			std::cout << "\tPAT section_number = " << (unsigned int)(packet[11]) << std::endl;
			std::cout << "\tPAT last_section_number = " << (unsigned int)(packet[12]) << std::endl;

			int table_count = (((packet[6] & 0x03) | packet[7]) & 0xFFF);
			
			//section_lenght = ((packet[6] & 0x03) | packet[7])
			for (int i = 0; i < ((table_count - 9) / 4); i++) {
				std::cout << "\tPAT program_number = " << (unsigned int)(packet[13 + (i * 4)] | packet[14 + (i * 4)]) << std::endl;
				if ((unsigned int)(packet[13 + (i * 4)] | packet[14 + (i * 4)]) == 0) //if program_number ==0
					std::cout << "\tPAT network_PID = " << (unsigned int)((packet[15 + (i * 4)] & 0x1F) | packet[16 + (i * 4)]) << std::endl;
				else
					std::cout << "\tPAT program_map_PID = " << (unsigned int)((packet[15 + (i * 4)] & 0x1F) | packet[16 + (i * 4)]) << std::endl;
					//std::cout << (unsigned int)((packet[15 + i * 4]) & 0x1F) << "   " << (unsigned int)(packet[16 + i * 4]) << std::endl;
			}

			//convert each byte to hex
			std::cout << "\tPAT CRC_32 = " << std::hex << (unsigned int)packet[17] << " " << (unsigned int)packet[18] << 
				" " << (unsigned int)packet[19] << " " << (unsigned int)packet[20] << std::dec << std::endl;

			//std::cout << ((packet[6] & 0x03) | packet[7]) << std::endl;
		}

		std::cout << "transport_scrambling_control = " << (packet[3] & 0xC0) << std::endl;
		/*
		switch ((packet[3] >> 6)){
		case 0x00:
			std::cout << "    Not Scrambled" << std::endl;
			break;
		case 0x01:
			std::cout << "    Reserved for future use" << std::endl;
			break;
		case 0x02:
			std::cout << "    Scrambled with even key" << std::endl;
			break;  
		case 0x03:
			std::cout << "    Scrambled with odd key" << std::endl;
			break;
		}
		*/
		std::cout << "adaptation_field_control = " << ((packet[3] & 0x30) >> 4) << std::endl;
		std::cout << "continuity_counter = " << (packet[3] & 0xF) << std::endl;
		
		//int a;
		//std::cin >> a;

	}
}