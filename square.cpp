// AOtest.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include "winio.h"
using namespace std; 
#pragma comment(lib,"winio.lib")


void main()
{
	int BASE_ADDRESS=0xE880;
	int iChannel = 0;
	float fVoltage, fHiVolt, fLoVolt;
	int AOPort;
	int LByte;
	int HByte;
	int i = 1;
    char c;
	unsigned short outData;

	/* 初始化WinIO */
	if (!InitializeWinIo())
	{
		cout<<"Error In InitializeWinIo!"<<endl;
		exit(1);
	}
    int st = clock();
	fHiVolt = 10;
	fLoVolt = 0;
	AOPort = 10 + iChannel * 2;	
	
	cout<<"请设定AO输出电压："<<endl;	//两个DA通道
    cin>>fVoltage;
	outData = (unsigned short)(fVoltage / (fHiVolt - fLoVolt) * 0xfff);
	while(1)
	{
		system("cls");			//清屏函数
		_outp(BASE_ADDRESS + 14, 1);	//设置内部参考电压为5V

		
		outData = outData;
		cout<<outData << endl;
		
		int etp=clock();
		if(etp-st / 500 > i)
		{
			outData = ~outData;
			i++;
		}

		LByte=outData & 0xff;
		HByte=(outData >> 8) & 0x0f;

		cout << LByte << " " << HByte << endl;

		_outp(BASE_ADDRESS + AOPort, LByte);			//低字节部分
		_outp(BASE_ADDRESS + AOPort + 1, HByte);		//高字节部分

		cout<<endl<<endl<<"第 "<<iChannel<<" 通道输出电压为:"<<fVoltage<<"V"<<endl;
		cout<<"按n键退出"<<endl;
		if(kbhit()) 
		{
			c = getch();
			if(c == 'N' || c == 'n')
			{
				break;
			}
		}
	}
		_outp(BASE_ADDRESS + AOPort, 0);								
		_outp(BASE_ADDRESS + AOPort + 1, 0);							//清零	
		
		ShutdownWinIo();												//关闭WinIO
}