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

/*
 * strncpy_s is only exported by msvcrt.dll starting from Windows Vista, so
 * using it when linking against msvcrt.dll breaks loading on Windows XP.
 * Use it only with runtimes that always provide it (MSVC and UCRT),
 * otherwise fall back to a portable implementation with the same behavior
 * as strncpy_s(..., _TRUNCATE): copy at most size - 1 chars, always terminate.
 */
static inline void safe_strncpy(char *dest, size_t size, const char *src)
{
	if (!dest || size == 0)
	{
		return;
	}
#if defined(_MSC_VER) || defined(_UCRT)
	strncpy_s(dest, size, src ? src : "", _TRUNCATE);
#else
	if (!src)
	{
		dest[0] = '\0';
		return;
	}
	strncpy(dest, src, size - 1);
	dest[size - 1] = '\0';
#endif
}

#endif /* __LIBRARY__ */
