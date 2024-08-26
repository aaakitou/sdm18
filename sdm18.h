#ifndef sdm18_H_
#define sdm18_H_
#include "mbed.h"
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

class sdm18{
public:
    //sdm18(CAN &can,bool motor_type,int motor_num);
    //int sdm18_send(int* moter);
    //void sdm18_read(CANMessage &msg, short *rotation,short *speed);
    //void can_read();
    //float pid(float T,short rpm_now, short set_speed,float *delta_rpm_pre,float *ie,float KP=25,float KI=10, float KD=0);
    //void spd_control(int* set_speed,int* motor);

    sdm18(BufferedSerial &sensor);
    bool setbaudrate(char baudrate);
    uint16_t calculate_crc16(char *buf, int len);
    bool getdata();
    bool startscan();
    bool stopscan();  
private:
    //uint16_t calculate_crc16;
    //HardwareSerial &_sensor_serial;
    //CANMessage _canMessage,_canMessage2,_msg;
    //CAN &_can;
    BufferedSerial &_sensor;
    char scan_recv_start[23];
    char scan_recv_stop[10];
    char scan_recv_setbaudrate[10];
    uint16_t crc_result;
    uint16_t checksum;

};
#endif