program TestDLL;

uses
  Windows;

const
  ENUMCOM_LIBRARY = 'enumcom.dll';
  BUFFERSIZE = 1024;

type
  TSerialPortInformation = packed record
    intPortIndex: Integer;
    bUsbDevice: Integer;                                    // Provided through a USB connection?
    strDevPath: array[0..BUFFERSIZE - 1] of AnsiChar;       // Device path for use with CreateFile()
    strPortName: array[0..BUFFERSIZE - 1] of AnsiChar;      // Simple name (i.e. COM1)
    strFriendlyName: array[0..BUFFERSIZE - 1] of AnsiChar;  // Full name to be displayed to a user
    strPortDesc: array[0..BUFFERSIZE - 1] of AnsiChar;      // friendly name without the COMx*)
  end;
  PSerialPortInformation = ^TSerialPortInformation;
  TSerialPortInformationArray = array of TSerialPortInformation;

  TGetSerialPorts = function(outArray: PSerialPortInformation; maxCount: Integer): Integer; stdcall;
  TGetSerialPortsCount = function(): Integer; stdcall;

var
  DLLHandle: THandle;
  GetSerialPorts: TGetSerialPorts;
  GetSerialPortsCount: TGetSerialPortsCount;
  SerialPortInformationArray: TSerialPortInformationArray;
  i,
  SerialPortsCount: Integer;

begin
  SerialPortInformationArray := Default(TSerialPortInformationArray);
  DLLHandle := LoadLibrary(ENUMCOM_LIBRARY);
  if DLLHandle <> 0 then
  begin
    GetSerialPorts := TGetSerialPorts(GetProcAddress(DLLHandle, 'GetSerialPorts'));
    GetSerialPortsCount := TGetSerialPortsCount(GetProcAddress(DLLHandle, 'GetSerialPortsCount'));

    if Assigned(GetSerialPorts) and Assigned(GetSerialPortsCount) then
    begin
      SerialPortsCount := GetSerialPortsCount();
      SetLength(SerialPortInformationArray, SerialPortsCount);
      if Assigned(SerialPortInformationArray) then
      begin
        GetSerialPorts(@SerialPortInformationArray[0], SerialPortsCount);
        for i := 0 to SerialPortsCount - 1 do
        begin
          WriteLn('PortIndex: "', SerialPortInformationArray[i].intPortIndex, '"');
          WriteLn('strDevPath: "', SerialPortInformationArray[i].strDevPath, '"');
          WriteLn('strDevPath: "', SerialPortInformationArray[i].strPortName, '"');
          WriteLn('strPortName: "', SerialPortInformationArray[i].strPortName, '"');
          WriteLn('strFriendlyName: "', SerialPortInformationArray[i].strFriendlyName, '"');
          WriteLn('strPortDesc: "', SerialPortInformationArray[i].strPortDesc, '"');
          WriteLn('');
        end;
      end;
    end
    else
      WriteLn('Error: SerialPortInformationArray not assigned');

    FreeLibrary(DLLHandle);
  end
  else
    Writeln('Error: Library not loaded');

  ReadLn;
end.

