/*
* File:   Parser.h
* Author: Nycholas de Sousa
* Email: nych.sousa@gmail.com
*
* Created on June 21, 2016
*/

#pragma once

#include <stdio.h>
#include <ctime>
#include <fstream>
#include <iostream>

class Parser {
private:
	std::string fileName;
	std::ifstream reader;

	std::ofstream myFile;

	short packetSize;
	unsigned char* packet;

	//Variaveis de Transport Package
	unsigned int sync_byte;
	unsigned int transport_error_indicator;
	unsigned int payload_unit_start_indicator;
	unsigned int transport_priority;
	unsigned int PID;
	unsigned int transport_scrambling_control;
	unsigned int adaptation_field_control;
	unsigned int continuity_counter;
	unsigned int data_byte;

	//Variaveis da Tabela PAT e PMT
	unsigned int table_id;
	unsigned int section_syntax_indicator;
	unsigned int zero;
	unsigned int reserved1;
	unsigned int section_lenght;
	unsigned int transport_stream_id;
	unsigned int reserved2;
	unsigned int version_number;
	unsigned int current_next_indicator;
	unsigned int section_number;
	unsigned int last_section_number;
	unsigned int program_number;
	unsigned int reserved3;
	unsigned int network_PID;
	unsigned int program_map_PID;

	unsigned int PCR_PID;
	unsigned int program_info_lenght;
	unsigned int stream_type;
	unsigned int reserved4;
	unsigned int reserved5;
	unsigned int reserved6;
	unsigned int elementary_PID;
	unsigned int ES_info_lenght;


public:
	Parser(std::string videoFile);
	~Parser();

	void readBytes();
};