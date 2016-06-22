#pragma once

#include <vector>
#include <fstream>
#include "PAT.h"
#include "PMT.h"

class Parser {
private:
	std::string fileName;
	std::ifstream reader;

	std::vector<PAT> patTable;
	std::vector<PMT> pmtTable;

	short packetSize;
	unsigned char* packet;
public:
	Parser(std::string videoFile);
	~Parser();

	void readBytes();
};