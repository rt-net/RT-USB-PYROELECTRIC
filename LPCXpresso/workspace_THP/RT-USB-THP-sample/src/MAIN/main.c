/**
 * @file   main.c
 * @brief  温湿気圧センサのサンプルプログラム<br>
 *
 * @author RTCorp. Ryota Takahashi
 */

//OHTER
#include "LPC13xx.h"
#include "core_cm3.h"
#include "type.h"
//PERIHERAL
#include "debug.h"
#include "i2c.h"
#include "io.h"
#include "gpio.h"
//#include "ssp.h"
#include "SystemTickTimer.h"
#include "timer.h"
#include "usbTransmission.h"
#include "uart.h"
//MAIN
#include "MainFunction.h"
#include "UserInterface.h"
#include "bme280.h"

//USB
#include "usb.h"
#include "usbcfg.h"
#include "usbhw.h"
#include "usbcore.h"
#include "cdc.h"
#include "cdcuser.h"

/////プロトタイプ宣言//////////////
void init(void);
/////////////////////////////

/**
 * main関数
 *
 * @param void
 * @return 0
 */
int main(void) {

	init();
	GPIOInit();
	tryUSBConnect();        //USB接続を試みる

	setSendDataEnable(1);
	if (isUSBConfig() == 1)  flashLED(300, 300); //USBの接続使用
	else flashLED(100, 100); //USBの接続不使用

	//bme280の初期化
	get_trimdata();
	bme280_init();

	GPIOSetDir(PORT1,8,0);

	GPIOSetDir(PORT0,11,0);

	//myPrintfUSB("test");

	unsigned int i;
	int test = 1;
	while (1){
		/*
		test = (LPC_GPIO0->DATA & 0x1<<11)>>1;
		myPrintfUSB("test %d \n",test);
		for(i=0;(i/2)<100000;i++){
			//GPIOSetDir(PORT1,8,1);
		}
		test = (LPC_GPIO0->DATA & 0x1<<11)>>1;
		myPrintfUSB("test %d \n",test);
		for(i=0;(i/2)<100000;i++){
			//GPIOSetDir(PORT1,8,0);
		}
		*/
		///*
		test =(LPC_GPIO0->DATA & 0x1<<11)>>1;
		if(test == 1024)
		{
			GPIOSetDir(PORT1,8,1);
			myPrintfUSB("test %d \n",test);
		}else{
			GPIOSetDir(PORT1,8,0);
			myPrintfUSB("test %d \n",test);
		}
		for(i=0;(i/2)<1000;i++){
			//GPIOSetDir(PORT1,8,1);
		}
		//*/

		//LPC_GPIO0->DATA & 0x11;
		/*
		for(i=0;(i/2)<100000;i++){
			GPIOSetDir(PORT1,8,1);
		}
		for(i=0;(i/2)<100000;i++){
			GPIOSetDir(PORT1,8,0);
		}
		*/
	}

	return 0;
}

/**
 * 各種ペリフェラルの初期化
 *
 * @param  void
 * @return void
 */
void init(void) {
	//IOポートの初期化
	initIO();
	//LEDを一回だけ点滅させる
	turnLED(1);
	wait1msec(500);
	turnLED(0);
	wait1msec(500);
	//各ペリフェラルの初期化
	USBIOClkConfig();
	initTimer32_0();
	initTimer32_1();
	USB_Init();
	UARTInit(115200);
	while (!I2CInit((uint32_t) I2CMASTER)); // I2Cの初期化が終わるまで待つ
	//UIの初期化
	initUI();
	initMainFunction();

}

/******************************************************************************
 **                            End Of File
 ******************************************************************************/
