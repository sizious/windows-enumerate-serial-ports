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
			safe_strncpy(outArray[i].strDevPath, sizeof(outArray[i].strDevPath), g_serial_ports[i].strDevPath);
			safe_strncpy(outArray[i].strPortName, sizeof(outArray[i].strPortName), g_serial_ports[i].strPortName);
			safe_strncpy(outArray[i].strFriendlyName, sizeof(outArray[i].strFriendlyName), g_serial_ports[i].strFriendlyName);
			safe_strncpy(outArray[i].strPortDesc, sizeof(outArray[i].strPortDesc), g_serial_ports[i].strPortDesc);
		}

		return actualCount;
	}
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    if (fdwReason == DLL_PROCESS_ATTACH) {
		std::vector<SSerInfo> asi;

		g_serial_ports_count = 0;

		// Exceptions must never escape DllMain, or the host process will crash
		try {
			EnumSerialPorts(asi, FALSE /*include all*/);
		}
		catch (...) {
			return TRUE; // no serial ports reported
		}

		// Never write past the end of the fixed-size array
		int count = (asi.size() < MAX_SERIAL_PORT_COUNT)
			? static_cast<int>(asi.size()) : MAX_SERIAL_PORT_COUNT;

		for (int i = 0; i < count; i++) {
			const SSerInfo& item = asi[i];
			g_serial_ports[i].intPortIndex = item.intPortIndex;
			safe_strncpy(g_serial_ports[i].strDevPath, BUFFERSIZE, item.strDevPath.c_str());
			safe_strncpy(g_serial_ports[i].strPortName, BUFFERSIZE, item.strPortName.c_str());
			safe_strncpy(g_serial_ports[i].strFriendlyName, BUFFERSIZE, item.strFriendlyName.c_str());
			g_serial_ports[i].bUsbDevice = item.bUsbDevice;
			safe_strncpy(g_serial_ports[i].strPortDesc, BUFFERSIZE, item.strPortDesc.c_str());
		}

		g_serial_ports_count = count;
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
