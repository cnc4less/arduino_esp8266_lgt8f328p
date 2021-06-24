/*
 ReadAnalogVoltage

 lgt8f328p��ȡ��ѹ Wemos TTGO XI 8F328P-U Board
 */
// the setup routine runs once when you press reset:
#include <math.h >

#include "TM1637.h"
#define CLK 10//pins definitions for TM1637 and can be changed to other ports
#define DIO 9
TM1637 tm1637(CLK, DIO);
unsigned char ClockPoint = 1;
int sensorValue = 0;

unsigned char hour = 0;
unsigned char minute = 0;
int8_t TimeDisp[] = { 0x00, 0x00, 0x00, 0x00 };

void TimeUpdate(void) {
	int sensorValuesa = cbrt(sensorValue) * 100; //ȡ������
	//int sensorValue1 = constrain(sensorValue - 0.5 * 4096 / 2.56, 850, 2500);

//	int sensorValuesa = log10(sensorValue1) * 1000; //��10Ϊ�׵Ķ���

	int voltage = sensorValue * (2.56 / 4096.0) * 100;
	//int BRIGHTS = constrain(sensorValuesa, 2800, 3200);
//	int BRIGHT = map(BRIGHTS, 2800, 3200, 0, 7);

	int BRIGHTS = constrain(sensorValuesa, 1000, 1320);
	int BRIGHT = map(BRIGHTS, 1000, 1320, 0, 7);

	if (ClockPoint) {
		tm1637.point(POINT_ON);
		Serial.println(sensorValuesa / 1);
		delay(100);
		tm1637.set(BRIGHT);
		TimeDisp[0] = sensorValuesa / 100 / 10;
		TimeDisp[1] = sensorValuesa / 100 % 10;
		TimeDisp[2] = sensorValuesa % 100 / 10;
		TimeDisp[3] = sensorValuesa % 100 % 10;
	} else {
		tm1637.point(POINT_OFF);
		Serial.println(voltage / 1);
		delay(100);
		tm1637.set(BRIGHT);
		TimeDisp[0] = 0;
		TimeDisp[1] = 0;
		TimeDisp[2] = 0;
		TimeDisp[3] = BRIGHT;
	}
	Serial.println(BRIGHT);
	delay(100);
	ClockPoint = !ClockPoint;
	//int sensorValuesa = cbrt(sensorValue) * 100;
	//TimeDisp[0,1,2,3] = sensorValuesa /  1;

}

void setup() {
	// initialize serial communication at 9600 bits per second:
	Serial.begin(9600);

	tm1637.init();
	tm1637.set(BRIGHT_TYPICAL); //BRIGHT_TYPICAL = 2,BRIGHT_DARKEST = 0,BRIGHTEST = 7;

	analogReference(INTERNAL2V56); //use internal 1.1v as Avref
	//analogReference(EXTERNAL); //ʹ���ⲿ��׼

//analogReference(type)
//����
//��������ģ������Ļ�׼��ѹ�������뷶Χ�����ֵ����ѡ����:
//DEFAULT��Ĭ��5V��Ocrobot���ư�Ϊ5V����3.3���أ�Ocrobot���ư�Ϊ3.3V��Ϊ��׼��ѹ��
//INTERNAL����ATmega168��ATmega328����1.1VΪ��׼��ѹ���Լ���ATmega8����2.56VΪ��׼��ѹ��Ocrobot Mega�޴�ѡ�
////INTERNAL1V1����1.1VΪ��׼��ѹ����ѡ������Ocrobot Mega��
//INTERNAL2V56����2.56VΪ��׼��ѹ����ѡ������Ocrobot Mega��
//EXTERNAL����AREF���ţ�0��5V���ĵ�ѹ��Ϊ��׼��ѹ��
//����
//type��ʹ�����ֲο����ͣ�DEFAULT, INTERNAL, INTERNAL1V1, INTERNAL2V56, INTERNAL4V096 ���� EXTERNAL����
//����
//��

	uint32_t guid = (GUID3 << 24) | (GUID2 << 16) | (GUID1 << 8) | GUID0; // ��guid��ֵΨһID
	Serial.println(guid); // �������ΨһID
	delay(2000);
}

// the loop routine runs over and over again forever:
void loop() {
	// read the input on analog pin 0:
	sensorValue = analogRead(A0);
	// Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
	float voltage = sensorValue * (2.56 / 4096.0);
	// print out the value you read:
	Serial.print(sensorValue);
	Serial.print(' ');
	Serial.print(cbrt(sensorValue));
	Serial.print(' ');
	Serial.println(voltage);

	delay(1000);
	TimeUpdate();
	tm1637.display(TimeDisp);
}
