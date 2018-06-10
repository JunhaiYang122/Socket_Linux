#include "Client.h"
#include <errno.h>
#include <iostream>

using namespace std;
Client::Client(const char* Addr, int Port, int Id)
{
	address = Addr;
	port = Port;
	id = Id;
}

int Client::Connect()
{
	int rlt = 0;

	//用于记录错误信息并输出
	int iErrMsg;
	//启动WinSock
	WSAData wsaData;
	iErrMsg = WSAStartup(MAKEWORD(1, 1), &wsaData);
	if (iErrMsg != NO_ERROR)//有错误
	{
		printf("failed with wsaStartup error : %d\n", iErrMsg);
		rlt = 1;
		return rlt;
	}
	else 
	{
		printf("wsaStartup Succussfully : %d\n", iErrMsg);
	}
	
	//创建Socket
	m_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_sock == INVALID_SOCKET)//创建Socket失败
	{
		printf("socket failed with error : %d\n", WSAGetLastError());
		rlt = 2;
		return rlt;
	}
	else 
	{
		printf("Create Socket Succussfully: %d\n", WSAGetLastError());
	}

	//目标服务器数据
	sockaddr_in sockaddrServer;
	sockaddrServer.sin_family = AF_INET;
	sockaddrServer.sin_port = htons(port);
	sockaddrServer.sin_addr.s_addr = inet_addr(address);

	//连接,sock与目标服务器连接
	iErrMsg = connect(m_sock, (sockaddr*)&sockaddrServer, sizeof(sockaddrServer));
	
	if (iErrMsg < 0)//连接失败
	{
		printf("connect failed with error : %d\n", iErrMsg);
		printf("Error: %d\n", errno); 
		rlt = 3;
		return rlt;
	}
	else 
	{
		printf("Connect Succussfully: %d\n", iErrMsg);
	}
	return rlt;
}


int Client::SendMsg(const char* msg, int len)
{
	int rlt = 0;

	int iErrMsg = 0;

	//发送消息，指定sock发送消息
	iErrMsg = send(m_sock, msg, len, 0);
	if (iErrMsg < 0)
		//发送失败
	{
		printf("send msg failed with error : %d\n", iErrMsg);
		rlt = 1;
		return rlt;
	}
	else 
	{
		printf("Message Send Succussfully: %d\n", iErrMsg);
	}
	return rlt;
}

void Client::Close()
{
	closesocket(m_sock);
	printf("Socket closed Succussfully!\n");
}

//写单个寄存器
void Client::writeRegister(int Ref, int addr, int value)  //写一个寄存器，使用功能码16，修改后就可以写多个
{
	unsigned char Temp_buf[20];
	unsigned char Read_buf[20];
	int iErrMsg = 0;

	Temp_buf[0] = 0;//识别号
	Temp_buf[1] = 2;
	
	Temp_buf[2] = 0;//协议
	Temp_buf[3] = 0;
	
	Temp_buf[4] = 0;//从ID开始到最后的字节数
	Temp_buf[5] = 6;
	
	Temp_buf[6] = 0;//从机ID
	
	Temp_buf[7] = 5;//命令代码
	Temp_buf[8] = 0;//addr head //开始的地址
	Temp_buf[9] = 228;
	Temp_buf[10] = 255;//data
	Temp_buf[11] = 0;
	SendMsg((char*)Temp_buf, 12);   //将报文发出，15为报文长度，这里是固定的
	iErrMsg = recv(m_sock,(char*)Read_buf, 12,0);
	if (iErrMsg < 0)//接收失败
	{
		printf("receive msg failed with error : %d\n", iErrMsg);
	}
	else 
	{
		printf("Message Receive Succussfully: %d\n", iErrMsg);
	}
	cout<<Read_buf<<endl;
}

//写多个寄存器
void Client::writeRegisters(int Ref, int addr, int value)  //写多个寄存器
{
	unsigned char Temp_buf[20];
	Temp_buf[0] = Ref;
	Temp_buf[1] = 1;
	Temp_buf[2] = 0;
	Temp_buf[3] = 0;
	Temp_buf[4] = 0;//从ID开始到最后的字节数
	Temp_buf[5] = 11;
	Temp_buf[6] = 1;//从机ID
	Temp_buf[7] = 16;//命令代码
	Temp_buf[8] = (addr-1)/256;//addr head //开始的地址
	Temp_buf[9] = (addr-1) % 256;
	Temp_buf[10] = 0;//number of addr   //地址的长度
	Temp_buf[11] = 2;
	Temp_buf[12] = 4;//# of Bytes for values    //一共多少byte的值
	Temp_buf[13] = value/256;//values           //具体的值，这里我只改一个寄存器，就写一个值
	Temp_buf[14] = value%256;
	Temp_buf[15] = value/256;//values           //具体的值，这里我只改一个寄存器，就写一个值
	Temp_buf[16] = value%256;
	SendMsg((char*)Temp_buf, 17);   //将报文发出，15为报文长度，这里是固定的
}

//写单个线圈
void Client::writeCoil(int Ref, int addr, int value)
{
	unsigned char Temp_buf[20];
	Temp_buf[0] = Ref;
	Temp_buf[1] = 1;
	Temp_buf[2] = 0;
	Temp_buf[3] = 0;
	Temp_buf[4] = 0;//从ID开始到最后的字节数
	Temp_buf[5] = 11;
	Temp_buf[6] = 1;//从机ID
	Temp_buf[7] = 16;//命令代码
	Temp_buf[8] = (addr-1)/256;//addr head //开始的地址
	Temp_buf[9] = (addr-1) % 256;
	Temp_buf[10] = 0;//number of addr   //地址的长度
	Temp_buf[11] = 2;
	Temp_buf[12] = 4;//# of Bytes for values    //一共多少byte的值
	Temp_buf[13] = value/256;//values           //具体的值，这里我只改一个寄存器，就写一个值
	Temp_buf[14] = value%256;
	Temp_buf[15] = value/256;//values           //具体的值，这里我只改一个寄存器，就写一个值
	Temp_buf[16] = value%256;
	SendMsg((char*)Temp_buf, 17);   //将报文发出，15为报文长度，这里是固定的
}

//写多个线圈
void Client::writeCoils(int Ref, int addr, int value)
{
	unsigned char Temp_buf[20];
	Temp_buf[0] = Ref;
	Temp_buf[1] = 1;
	Temp_buf[2] = 0;
	Temp_buf[3] = 0;
	Temp_buf[4] = 0;//从ID开始到最后的字节数
	Temp_buf[5] = 11;
	Temp_buf[6] = 1;//从机ID
	Temp_buf[7] = 16;//命令代码
	Temp_buf[8] = (addr-1)/256;//addr head //开始的地址
	Temp_buf[9] = (addr-1) % 256;
	Temp_buf[10] = 0;//number of addr   //地址的长度
	Temp_buf[11] = 2;
	Temp_buf[12] = 4;//# of Bytes for values    //一共多少byte的值
	Temp_buf[13] = value/256;//values           //具体的值，这里我只改一个寄存器，就写一个值
	Temp_buf[14] = value%256;
	Temp_buf[15] = value/256;//values           //具体的值，这里我只改一个寄存器，就写一个值
	Temp_buf[16] = value%256;
	SendMsg((char*)Temp_buf, 17);   //将报文发出，15为报文长度，这里是固定的
}

//读单个寄存器
void Client::readRegister(int Ref, int addr, int value)
{
	unsigned char Temp_buf[20];
	Temp_buf[0] = Ref;
	Temp_buf[1] = 1;
	Temp_buf[2] = 0;
	Temp_buf[3] = 0;
	Temp_buf[4] = 0;//从ID开始到最后的字节数
	Temp_buf[5] = 11;
	Temp_buf[6] = 1;//从机ID
	Temp_buf[7] = 16;//命令代码
	Temp_buf[8] = (addr-1)/256;//addr head //开始的地址
	Temp_buf[9] = (addr-1) % 256;
	Temp_buf[10] = 0;//number of addr   //地址的长度
	Temp_buf[11] = 2;
	Temp_buf[12] = 4;//# of Bytes for values    //一共多少byte的值
	Temp_buf[13] = value/256;//values           //具体的值，这里我只改一个寄存器，就写一个值
	Temp_buf[14] = value%256;
	Temp_buf[15] = value/256;//values           //具体的值，这里我只改一个寄存器，就写一个值
	Temp_buf[16] = value%256;
	SendMsg((char*)Temp_buf, 17);   //将报文发出，15为报文长度，这里是固定的
}

//读多个寄存器
void Client::readRegisters(int Ref, int addr, int value)
{
	unsigned char Temp_buf[20];
	Temp_buf[0] = Ref;
	Temp_buf[1] = 1;
	Temp_buf[2] = 0;
	Temp_buf[3] = 0;
	Temp_buf[4] = 0;//从ID开始到最后的字节数
	Temp_buf[5] = 11;
	Temp_buf[6] = 1;//从机ID
	Temp_buf[7] = 16;//命令代码
	Temp_buf[8] = (addr-1)/256;//addr head //开始的地址
	Temp_buf[9] = (addr-1) % 256;
	Temp_buf[10] = 0;//number of addr   //地址的长度
	Temp_buf[11] = 2;
	Temp_buf[12] = 4;//# of Bytes for values    //一共多少byte的值
	Temp_buf[13] = value/256;//values           //具体的值，这里我只改一个寄存器，就写一个值
	Temp_buf[14] = value%256;
	Temp_buf[15] = value/256;//values           //具体的值，这里我只改一个寄存器，就写一个值
	Temp_buf[16] = value%256;
	SendMsg((char*)Temp_buf, 17);   //将报文发出，15为报文长度，这里是固定的
}

//读单个线圈
void Client::readCoil(int Ref, int addr, int value)
{
	unsigned char Temp_buf[20];
	Temp_buf[0] = Ref;
	Temp_buf[1] = 1;
	Temp_buf[2] = 0;
	Temp_buf[3] = 0;
	Temp_buf[4] = 0;//从ID开始到最后的字节数
	Temp_buf[5] = 11;
	Temp_buf[6] = 1;//从机ID
	Temp_buf[7] = 16;//命令代码
	Temp_buf[8] = (addr-1)/256;//addr head //开始的地址
	Temp_buf[9] = (addr-1) % 256;
	Temp_buf[10] = 0;//number of addr   //地址的长度
	Temp_buf[11] = 2;
	Temp_buf[12] = 4;//# of Bytes for values    //一共多少byte的值
	Temp_buf[13] = value/256;//values           //具体的值，这里我只改一个寄存器，就写一个值
	Temp_buf[14] = value%256;
	Temp_buf[15] = value/256;//values           //具体的值，这里我只改一个寄存器，就写一个值
	Temp_buf[16] = value%256;
	SendMsg((char*)Temp_buf, 17);   //将报文发出，15为报文长度，这里是固定的
}

//读多个线圈
void Client::readCoils(int Ref, int addr, int value)
{
	unsigned char Temp_buf[20];
	Temp_buf[0] = Ref;
	Temp_buf[1] = 1;
	Temp_buf[2] = 0;
	Temp_buf[3] = 0;
	Temp_buf[4] = 0;//从ID开始到最后的字节数
	Temp_buf[5] = 11;
	Temp_buf[6] = 1;//从机ID
	Temp_buf[7] = 16;//命令代码
	Temp_buf[8] = (addr-1)/256;//addr head //开始的地址
	Temp_buf[9] = (addr-1) % 256;
	Temp_buf[10] = 0;//number of addr   //地址的长度
	Temp_buf[11] = 2;
	Temp_buf[12] = 4;//# of Bytes for values    //一共多少byte的值
	Temp_buf[13] = value/256;//values           //具体的值，这里我只改一个寄存器，就写一个值
	Temp_buf[14] = value%256;
	Temp_buf[15] = value/256;//values           //具体的值，这里我只改一个寄存器，就写一个值
	Temp_buf[16] = value%256;
	SendMsg((char*)Temp_buf, 17);   //将报文发出，15为报文长度，这里是固定的
}