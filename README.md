#ifndef SDM18_H_
#define SDM18_H_

#include <mbed.h>

#define CRC16_POLY 0x8005



/////////////start data//////////////////////////////
 const int Packet_heade=0xA5;
 const int Device_number= 0x03;
 const int Device_type=0x20;
 const int Command_type_start=0x01;
 const int Reserved_bit =0x00;
 const int Data_length =0x00;
 const int Data_length2=0x00;
 const int CheckSum_start=0x02;
 const int CheckSum_start2=0x6E;
 ///////////////stop data///////////////////////////////////
 const int Command_type_stop=0x02;
 const int CheckSum_stop=0x46;
 const int CheckSum_stop2=0x6E; 
 /////////////set baud rate///////////////////////////////////
 const int Command_type_setbaudrate=0x10; 
 const int Data_length_setbaudrate=0x01;
 const int BaudRate=0x07;
 const int CRC_setbaudrate=0xBD;
 const int CRC_setbaudrate2=0x3F;


enum BaudRateHex {
  baudlate_9600=0x00,
  baudlate_14400=0x01,
  baudlate_19200=0x02,
  baudlate_38400=0x03, 
  baudlate_43000=0x04, 
  baudlate_57600=0x05, 
  baudlate_76800=0x06, 
  baudlate_115200=0x07, 
  baudlate_128000=0x08, 
  baudlate_230400=0x09, 
  baudlate_256000=0x0a, 
  baudlate_460800=0x0b, 
  baudlate_921600=0x0c, 
};

class SDM18 {
 public:
  bool StartScan();
  bool StopScan();
  bool SetBaudRate(BaudRateHex baud);
  bool scandata();

 private:
  byte CalculateChecksum(byte *recv, int size);
  uint16_t calculate_crc16;
  HardwareSerial &_sensor_serial;
};


#endif  // SDM18_H_
