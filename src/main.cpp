#include "EnumSerial.h"

#include <iostream>
#include <algorithm>
#include <string>

#define DELIMITER "|"

std::string quoted(std::string& str) {
	std::replace( str.begin(), str.end(), '"', '\"');
	return "\"" + str + "\"";
}

int main(int argc, char* argv[]) {
	std::vector<SSerInfo> asi;

	std::cout <<
		"\"Index\"" << DELIMITER <<
		"\"DevicePath\"" << DELIMITER <<
		"\"Name\"" << DELIMITER <<
		"\"FriendlyName\"" << DELIMITER <<
		"\"IsUSBDevice\"" << DELIMITER <<
		"\"Description\"" << std::endl;
		
	// Populate the list of serial ports.
	EnumSerialPorts(asi, FALSE/*include all*/);
	for (int ii = 0; ii < asi.size(); ii++) {
		SSerInfo item = asi[ii];
		std::cout <<
			item.intPortIndex << DELIMITER <<
			quoted(item.strDevPath) << DELIMITER <<
			quoted(item.strPortName) << DELIMITER <<
			quoted(item.strFriendlyName) << DELIMITER <<
			(item.bUsbDevice ? "TRUE" : "FALSE") << DELIMITER <<
			quoted(item.strPortDesc) << std::endl;
	}
	
	return 0;
}
