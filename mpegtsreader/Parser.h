#pragma once

#include <stdio.h>
#include <fstream>
#include <iostream>

class Parser {
private:
	std::string fileName;
	std::ifstream reader;

	std::ofstream myFile;

	short packetSize;
	unsigned char* packet;
public:
	Parser(std::string videoFile);
	~Parser();

	void readBytes();
};