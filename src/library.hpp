#ifndef __LIBRARY__
#define __LIBRARY__

#include <windows.h>
#include <string.h>
#include <cstring>

#define DLLEXPORT __declspec(dllexport)
#define STDCALL __stdcall

#define BUFFERSIZE 1024
#define MAX_SERIAL_PORT_COUNT 256

typedef struct {
	int intPortIndex;
    int bUsbDevice;							// Provided through a USB connection?
	char strDevPath[BUFFERSIZE];			// Device path for use with CreateFile()
    char strPortName[BUFFERSIZE];			// Simple name (i.e. COM1)
    char strFriendlyName[BUFFERSIZE];		// Full name to be displayed to a user    						
    char strPortDesc[BUFFERSIZE];			// friendly name without the COMx    
} SerialPortInformation;

#endif /* __LIBRARY__ */
