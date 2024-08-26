#include "sdm18.h"
#include "mbed.h"

sdm18::sdm18(BufferedSerial &sensor) : _sensor(sensor){

}

uint16_t sdm18::calculate_crc16(char *buf, int len)
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

bool sdm18::startscan(){
    char cmd_start[9]={Packet_heade,Device_number,Device_type,Command_type_start,Reserved_bit,Data_length,Data_length2,CheckSum_start,CheckSum_start2};
    _sensor.write(cmd_start, sizeof(cmd_start));
    _sensor.read(scan_recv_start, 23);
    crc_result=calculate_crc16(scan_recv_start,21);
    checksum=(scan_recv_start[21]<<8)+scan_recv_start[22];
    if(checksum==crc_result&&checksum!=65535){
        return true;
    }else{
        return false;
    }
}

bool sdm18::stopscan(){
    char cmd_stop[9]={Packet_heade,Device_number,Device_type,Command_type_stop,Reserved_bit,Data_length,Data_length2,CheckSum_stop,CheckSum_stop2};
    _sensor.write(cmd_stop, sizeof(cmd_stop));
    _sensor.read(scan_recv_stop, 10);
    crc_result=calculate_crc16(scan_recv_stop,8);
    checksum=(scan_recv_start[8]<<8)+scan_recv_start[9];
    if(checksum==crc_result){
        return true;
    }else{
        return false;
    }
}

bool sdm18::getdata(){
    if(_sensor.readable())
    {
        _sensor.read(scan_recv_start, 23);
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
    }else{
        return false;
    }
}

bool sdm18::setbaudrate(char BaudRate)
{
    char setbaudrate[10]={Packet_heade,Device_number,Device_type,Command_type_setbaudrate,Reserved_bit,Data_length,Data_length_setbaudrate,BaudRate,CRC_setbaudrate,CRC_setbaudrate2};
    _sensor.write(setbaudrate,sizeof(setbaudrate));
    _sensor.read(scan_recv_setbaudrate, 10);
    crc_result=calculate_crc16(scan_recv_setbaudrate,8);
    checksum=(scan_recv_start[8]<<8)+scan_recv_start[9];
    if(checksum==crc_result){
        return true;
    }else{
        return false;
    }
}