#include "SDM18.h"

// ScanData sdm15_data;
// byte scan_recv[9];
// byte checksum = 0;
char scan_recv_start[23];
char scan_recv_stop[10];
char scan_recv_setbaudrate[10];
uint16_t checksum;

// byte SDM18::CalculateChecksum(byte *recv, int size) {
//   checksum = 0;
//   for (int i = 0; i < size - 1; i++) {
//     checksum += recv[i];
//   }
//   checksum = checksum & 0xFF;
//   return checksum;
// }
uint16_t SDM18::calculate_crc16(char *buf, int len)
{
	uint16_t crc = 0xFFFF;
	unsigned int i = 0;
	char bit = 0;
	for( i = 0; i < len; i++ )
	{
		crc ^= buf[i];

		for( bit = 0; bit < 8; bit++ )
		{
			if( crc & 0x0001 )
			{
				crc >>= 1;
				crc ^= 0xA001;
			}
			else
			{
				crc >>= 1;
			}
		}
	}
    return crc;
    
}

// bool SDM15::StartScan() {
//   // clear buffer
//   ClearBuffer();

//   // send cmd : PACKET_HED1, PACKET_HED2, START_SCAN, NO_DATA, 0x5F
//   byte cmd[5] = {PACKET_HED1, PACKET_HED2, START_SCAN, NO_DATA, 0x5F};
//   _sensor_serial.write(cmd, sizeof(cmd));
//   _sensor_serial.flush();

//   byte recv[5];

//   // receive byte data response : PACKET_HED1, PACKET_HED2, START_SCAN, NO_DATA,
//   // checksum
//   _sensor_serial.readBytes(recv, sizeof(recv));

//   // calculate checksum
//   checksum = CalculateChecksum(recv, sizeof(recv));

//   // check checksum
//   if (checksum != recv[4])
//     return false;
//   else
//     return true;
// }
bool SDM18::startscan(){
    char cmd_start[9]={Packet_heade,Device_number,Device_type,Command_type_start,Reserved_bit,Data_length,Data_length2,CheckSum_start,CheckSum_start2};
    sensor.write(cmd_start, sizeof(cmd_start));
    sensor.read(scan_recv_start, 23);
    crc_result=calculate_crc16(scan_recv_start,21);
    checksum=(scan_recv_start[21]<<8)+scan_recv_start[22];
    if(checksum==crc_result&&checksum!=65535){
        return true;
        led=1;
    }else{
        return false;
    }
}

// bool SDM15::StopScan() {
//   // clear buffer
//   ClearBuffer();

//   // send cmd : PACKET_HED1, PACKET_HED2, STOP_SCAN, NO_DATA, 0x60
//   byte cmd[5] = {PACKET_HED1, PACKET_HED2, STOP_SCAN, NO_DATA, 0x60};
//   _sensor_serial.write(cmd, sizeof(cmd));
//   _sensor_serial.flush();

//   byte recv[5];

//   // receive byte data response : PACKET_HED1, PACKET_HED2, STOP_SCAN, NO_DATA,
//   // checksum
//   _sensor_serial.readBytes(recv, sizeof(recv));

//   // calculate checksum
//   checksum = CalculateChecksum(recv, sizeof(recv));

//   // check checksum
//   if (checksum != recv[4])
//     return false;
//   else
//     return true;
// }
bool SDM18::stopscan(){
    char cmd_stop[9]={Packet_heade,Device_number,Device_type,Command_type_stop,Reserved_bit,Data_length,Data_length2,CheckSum_stop,CheckSum_stop2};
    sensor.write(cmd_stop, sizeof(cmd_stop));
    sensor.read(scan_recv_stop, 10);
    crc_result=calculate_crc16(scan_recv_stop,8);
    checksum=(scan_recv_start[8]<<8)+scan_recv_start[9];
    led=0;
    if(checksum==crc_result){
        return true;
        led=0;
    }else{
        return false;
    }
}

// ScanData SDM15::GetScanData() {
//   // clear buffer
//   ClearBuffer();

//   // receive byte data response : PACKET_HED1, PACKET_HED2, START_SCAN,
//   // data_length, data[4], checksum
//   _sensor_serial.readBytes(scan_recv, sizeof(scan_recv));

//   // calculate checksum
//   checksum = CalculateChecksum(scan_recv, sizeof(scan_recv));

//   // check checksum
//   if (checksum != scan_recv[8]) {
//     sdm15_data.checksum_error = true;
//     return sdm15_data;
//   }

//   sdm15_data.checksum_error = false;

//   sdm15_data.distance =
//       static_cast<int>(scan_recv[4]) + (static_cast<int>(scan_recv[5]) << 8);
//   sdm15_data.intensity = static_cast<int>(scan_recv[6]);
//   sdm15_data.disturb = static_cast<int>(scan_recv[7]);
  
//   return sdm15_data;
// }
bool SDM18::getdata(){
    if(sensor.readable())
    {
        sensor.read(scan_recv_start, 23);
        if(scan_recv_start[0]==0xa5)
        {
            for(int i=0;i<=22;i++)
            {
                printf("%02x ",scan_recv_start[i]);
            }
            printf("\r\n");
            printf("%d\r\n",static_cast<int>(scan_recv_start[13]) + (static_cast<int>(scan_recv_start[14]) << 8));
            checksum=(scan_recv_start[21]<<8)+scan_recv_start[22];
            crc_result = calculate_crc16(scan_recv_start,21);
            //printf("CRC-16: 0x%04X\n", sum);
            //printf("%d\r\n",result);
            //led = !led; // LEDを点滅させる
            //CANMessage msg(0x123,scan_recv,23); // ID: 0x123, データ: "Test", データ長: 4バイ
            //can1.write(msg);
            
        }
    }

    if(checksum==crc_result&&checksum!=65535)
    {
        return true;
            led=1;
    }else{
        return false;
    }
}

// bool SDM15::SetBaudRate(BaudRateHex baud_rate = BAUD_460800) {
//   // clear buffer
//   ClearBuffer();

//   // send cmd : PACKET_HED1, PACKET_HED2, SET_SERIAL_BAUD, data_length, baud,
//   // checksum
//   byte cmd[6] = {PACKET_HED1, PACKET_HED2, SET_SERIAL_BAUD,
//                  0x01,        baud_rate,        NO_DATA};
//   byte send_checksum = CalculateChecksum(cmd, sizeof(cmd));
//   cmd[5] = send_checksum;
//   _sensor_serial.write(cmd, sizeof(cmd));
//   _sensor_serial.flush();

//   byte recv[6];

//   // receive byte data response : PACKET_HED1, PACKET_HED2,
//   // SET_SERIAL_BAUD, data_length, baud, checksum
//   _sensor_serial.readBytes(recv, sizeof(recv));

//   // calculate checksum
//   byte recv_checksum = CalculateChecksum(recv, sizeof(recv));

//   // check checksum
//   if (recv_checksum != recv[5])
//     return false;
//   else
//     return true;
// }

bool SDM::setbaudrate(BaudRateHex baud_rate = baud_115200)
{
  char setbaudrate[10]={Packet_heade,Device_number,Device_type,Command_type_setbaudrate,Reserved_bit,Data_length,Data_length_setbaudrate,BaudRate,CRC_setbaudrate,CRC_setbaudrate2};
  sensor.write(setbaudrate,sizeof(setbaudrate));
  sensor.read(scan_recv_setbaudrate, 10);
  crc_result=calculate_crc16(scan_recv_setbaudrate,8);
  checksum=(scan_recv_start[8]<<8)+scan_recv_start[9];
  led=0;
  if(checksum==crc_result){
    return true;
    led=0;
  }else{
      return false;
  }
}