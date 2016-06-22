#pragma once

#include <vector>
#include <fstream>

class Parser {
private:
	std::string fileName;
	std::ifstream reader;

	short packetSize;
	unsigned char* packet;
public:
	Parser(std::string videoFile);
	~Parser();

	void readBytes();
};