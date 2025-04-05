#include <iostream>
#include <algorithm>
#include <string>

#include "EnumSerial.h"

#ifdef LIBRARY

#include "library.hpp"

static SerialPortInformation g_serial_ports[MAX_SERIAL_PORT_COUNT];
static int g_serial_ports_count = 0;

extern "C" {
	
	DLLEXPORT int STDCALL GetSerialPortsCount()
	{
		return g_serial_ports_count;
	}
	
	DLLEXPORT int STDCALL GetSerialPorts(SerialPortInformation* outArray, int maxCount)
	{
		if (!outArray || maxCount <= 0 || g_serial_ports_count <= 0)
		{
			return 0;
		}

		int actualCount = (maxCount < g_serial_ports_count) ? maxCount : g_serial_ports_count;

		for (int i = 0; i < actualCount; i++) {
			outArray[i].intPortIndex = g_serial_ports[i].intPortIndex;
			outArray[i].bUsbDevice = g_serial_ports[i].bUsbDevice;
			strncpy_s(outArray[i].strDevPath, sizeof(outArray[i].strDevPath), g_serial_ports[i].strDevPath, _TRUNCATE);
			strncpy_s(outArray[i].strPortName, sizeof(outArray[i].strPortName), g_serial_ports[i].strPortName, _TRUNCATE);
			strncpy_s(outArray[i].strFriendlyName, sizeof(outArray[i].strFriendlyName), g_serial_ports[i].strFriendlyName, _TRUNCATE);
			strncpy_s(outArray[i].strPortDesc, sizeof(outArray[i].strPortDesc), g_serial_ports[i].strPortDesc, _TRUNCATE);
		}

		return actualCount;
	}
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    if (fdwReason == DLL_PROCESS_ATTACH) {
		std::vector<SSerInfo> asi;
		
		EnumSerialPorts(asi, FALSE /*include all*/);
		
		for (int i = 0; i < asi.size(); i++) {
			SSerInfo item = asi[i];
			g_serial_ports[i].intPortIndex = item.intPortIndex;
			strncpy_s(g_serial_ports[i].strDevPath, BUFFERSIZE, item.strDevPath.c_str(), _TRUNCATE); 
			strncpy_s(g_serial_ports[i].strPortName, BUFFERSIZE, item.strPortName.c_str(), _TRUNCATE);
			strncpy_s(g_serial_ports[i].strFriendlyName, BUFFERSIZE, item.strFriendlyName.c_str(), _TRUNCATE);
			g_serial_ports[i].bUsbDevice = item.bUsbDevice;
			strncpy_s(g_serial_ports[i].strPortDesc, BUFFERSIZE, item.strPortDesc.c_str(), _TRUNCATE);
		}

		g_serial_ports_count = asi.size();
    }
    return TRUE;
}

#else

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

#endif
