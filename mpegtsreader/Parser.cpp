/* 
 * File:   Parser.cpp
 * Author: Nycholas de Sousa
 * Email: nych.sousa@gmail.com
 *
 * Created on June 21, 2016
 */

#include "Parser.h"

//Variáveis de contagem dos pacotes
int pkt_number = 0;
int pat_number = 0;
int pmt_number = 0;

/*
Função responsável pelo início do parser no vídeo
Como a atividade requer que vídeo seja lido/aberto a cada 188 bytes, é criado o packetSize
*/
Parser::Parser(std::string videoFile) {
	fileName = videoFile;
	packetSize = 188;
	packet = new unsigned char[188]();

	reader.open(fileName, std::ifstream::in | std::ifstream::binary);
	if (!reader.is_open()) {
		throw "Could not open file";
	}
}

/*
Destructor da Classe
*/
Parser::~Parser() {
	reader.close();
	delete packet;
}

/*
Função readBytes()
Responsável pela leitura de bytes no arquivo e exibição dos valores
*/
void Parser::readBytes() {
	while (!reader.eof()) {
		myFile.open("packets.txt", std::ofstream::app);
		reader.read((char*)(&packet[0]), packetSize);
		time_t t = time(0);   // get time now
		struct tm * now = localtime(&t);

		//O valor de packet (packet[i]) muda quando passa 8 bytes 
		//sync_byte possui um valor fixo, que é 0x47, porém, caso esse valor não seja mais esse, ele faz a leitura de novo
		if (packet[0] != 0x47)
			readBytes();
		else {
			//Aqui está pegando os packets do TS, depois será feita a verificação pra ver se possui PAT ou PMT
			sync_byte = packet[0];
			transport_error_indicator = (packet[1] & 0x80) >> 7;
			payload_unit_start_indicator = (packet[1] & 0x40) >> 6;
			transport_priority = (packet[1] & 0x20) >> 5;
			PID = (((packet[1] & 0x1F) << 8) | packet[2]);
			transport_scrambling_control = (packet[3] & 0xC0) >> 6;
			adaptation_field_control = (packet[3] & 0x30) >> 4;
			continuity_counter = packet[3] & 0x0F;
			//data_byte = packet[4];

			myFile << "------------------------------------------------------------------\n";
			myFile << "Packet Number: " << ++pkt_number << "\tPID of the Packet: " << PID << std::endl;
			myFile << "Date and Time Received: " << (now->tm_year + 1900) << '-' << (now->tm_mon + 1) << '-' << now->tm_mday 
				<< "    " << (now->tm_hour + 1) << ":" << (now->tm_min) << ":" << (now->tm_sec) << std::endl;
			myFile << "------------------------------------------------------------------\n";
			
			//Esses dados são apenas do pacote TS
			myFile << "sync_byte: 0x" << std::hex << sync_byte << std::dec << std::endl;
			myFile << "transport_error_indicator: " << transport_error_indicator << std::endl;
			myFile << "payload_unit_start_indicator: " << payload_unit_start_indicator << std::endl;
			myFile << "transport_priority: " << transport_priority << std::endl;
			myFile << "PID: " << PID << std::endl;
			myFile << "transport_scrambling_control: " << transport_scrambling_control << std::endl;
			myFile << "adaptation_field_control: " << adaptation_field_control << std::endl;
			myFile << "continuity_counter: " << continuity_counter << std::endl;

			table_id = packet[5];
			//Para que ocorra a tabela PAT, o PID deve ser 0 e o table_id também deve ser 0
			if (PID == 0 && table_id != 2) {
				//Obtendo os dados da tabela
				section_syntax_indicator = (packet[6] & 0x80) >> 7;
				zero = (packet[6] & 0x40) >> 6;
				reserved1 = (packet[6] & 0x30) >> 4;
				section_lenght = (packet[6] & 0x0F | packet[7]);
				transport_stream_id = (packet[8] | packet[9]);
				reserved2 = (packet[10] & 0xC0) >> 6;
				version_number = (packet[10] & 0x3E) >> 1;
				current_next_indicator = packet[10] & 0x01;
				section_number = packet[11];
				last_section_number = packet[12];

				//Dados da tabela PAT
				myFile << "\tPAT Packet Number: " << ++pat_number << std::endl;
				myFile << "\tPAT table_id: " << table_id << std::endl;
				myFile << "\tPAT section_syntax_indicator: " << section_syntax_indicator << std::endl;
				myFile << "\tPAT '0': " << zero << std::endl;
				myFile << "\tPAT reserved_1: " << reserved1 << std::endl;
				myFile << "\tPAT section_lenght: " << section_lenght << std::endl;
				myFile << "\tPAT transport_stream_id: " << transport_stream_id << std::endl;
				myFile << "\tPAT reserved_2: " << reserved2 << std::endl;
				myFile << "\tPAT version_number: " << version_number << std::endl;
				myFile << "\tPAT current_next_indicator: " << current_next_indicator << std::endl;
				myFile << "\tPAT section_number: " << section_number << std::endl;
				myFile << "\tPAT last_section_number: " << last_section_number << std::endl;

				//Auxiliares para a procura do program_number
				int aux1 = (int)(section_lenght - 8) / 4;
				int aux2 = 13; //Proxima posição de bytes a ser lida
				for (int i = 0; i < aux1; i++) {
					program_number = (packet[aux2] | packet[aux2 + 1]);
					reserved3 = (packet[aux2 + 2] & 0xE0) >> 5;
					myFile << "\tPAT program_number: " << program_number << std::endl;
					myFile << "\tPAT reserved_3: " << reserved3 << std::endl;

					if (program_number == 0) {
						network_PID = (((packet[aux2 + 2] & 0x1F) << 8) | packet[aux2 + 3]);
						myFile << "\tPAT network_PID: " << network_PID << std::endl;
					}
					else {
						program_map_PID = (((packet[aux2 + 2] & 0x1F) << 8) | packet[aux2 + 3]);
						myFile << "\tPAT program_map_PID: " << program_map_PID << std::endl;
					}
					aux2 = aux2 + 4;
				}
				//Devido o loop, a posição do byte pode acabar variando
				myFile << "\tPAT CRC_32 = " << std::hex << (unsigned int)packet[aux2] << " " << (unsigned int)packet[aux2 + 1] <<
					" " << (unsigned int)packet[aux2 + 2]<< " " << (unsigned int)packet[aux2 + 3] << std::dec << std::endl;
			}

			//Caso o table_id seja 2, significa que possui uma Tabela PMT
			if (table_id == 2) {
				//Obtendo os dados do PMT
				section_syntax_indicator = (packet[6] & 0x80) >> 7;
				zero = (packet[6] & 0x40) >> 6;
				reserved1 = (packet[6] & 0x30) >> 4;
				section_lenght = section_lenght = (packet[6] & 0x0F | packet[7]);
				program_number = (packet[8] | packet[9]);
				reserved2 = (packet[10] & 0xC0) >> 6;
				version_number = (packet[10] & 0x3E) >> 1;
				current_next_indicator = packet[10] & 0x01;
				section_number = packet[11];
				last_section_number = packet[12];
				reserved3 = (packet[13] & 0xE0) >> 5;
				PCR_PID = (((packet[13] & 0x1F) << 8) | packet[14]);
				reserved4 = (packet[15] & 0xF0) >> 4;
				program_info_lenght = ((packet[15] & 0x0F) << 8 | packet[16]);
		
				//Exibindo os dados do PMT
				myFile << "\tPMT Packet Number: " << ++pmt_number << std::endl;
				myFile << "\tPMT table_id: " << table_id << std::endl;
				myFile << "\tPMT section_syntax_indicator: " << section_syntax_indicator << std::endl;
				myFile << "\tPMT '0': " << zero << std::endl;
				myFile << "\tPMT reserved_1: " << reserved1 << std::endl;
				myFile << "\tPMT section_lenght: " << section_lenght << std::endl;
				myFile << "\tPMT program_number: " << program_number << std::endl;
				myFile << "\tPMT reserved_2: " << reserved2 << std::endl;
				myFile << "\tPMT version_number: " << version_number << std::endl;
				myFile << "\tPMT current_next_indicator: " << current_next_indicator << std::endl;
				myFile << "\tPMT section_number: " << section_number << std::endl;
				myFile << "\tPMT last_section_number: " << last_section_number << std::endl;
				myFile << "\tPMT reserved_3: " << reserved3 << std::endl;
				myFile << "\tPMT PCR_PID: " << PCR_PID << std::endl;
				myFile << "\tPMT reserved_4: " << reserved4 << std::endl;
				myFile << "\tPMT program_info_lenght: " << program_info_lenght << std::endl;

				int aux3 = (int)(section_lenght - 13) / 5;
				int aux4 = 17; //Proxima posição de bytes a ser lida
				for (int i = 0; i < aux3; i++) {
					stream_type = packet[aux4];
					reserved5 = (packet[aux4 + 1] & 0xE0) >> 5;
					elementary_PID = ((packet[aux4 + 1] & 0x0F) << 8 | packet[aux4 + 2]);
					reserved6 = (packet[aux4 + 3] & 0xF0) >> 4;
					ES_info_lenght = ((packet[aux4 + 3] & 0x0F) << 8 | packet[aux4 + 4]);

					myFile << "\tPMT stream_type: " << stream_type << std::endl;
					myFile << "\tPMT reserved_5: " << reserved5 << std::endl;
					myFile << "\tPMT elementary_PID: " << elementary_PID << std::endl;
					myFile << "\tPMT reserved_6: " << reserved6 << std::endl;
					myFile << "\tPMT ES_info_lenght: " << ES_info_lenght << std::endl;

					aux4 = aux4 + 5;
				}
				//Devido o loop, a posição do byte pode acabar variando
				myFile << "\tPMT CRC_32 = " << std::hex << (unsigned int)packet[aux4] << " " << (unsigned int)packet[aux4 + 1] <<
					" " << (unsigned int)packet[aux4 + 2] << " " << (unsigned int)packet[aux4 + 3] << std::dec << std::endl;

			}
			myFile << "------------------------------------------------------------------";
			myFile << std::endl << std::endl;
			myFile.close();
		}
	}
}