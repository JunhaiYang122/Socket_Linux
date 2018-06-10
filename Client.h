#pragma once
#include <stdio.h>
#include<windows.h>
#pragma comment(lib, "Ws2_32.lib")

class Client
{
public:
	//Constructor
	Client(const char* Addr, int Port, int Id); 
	//连接
	int Connect();
	//发送TCP包
	int SendMsg(const char* msg, int len);
	//关闭
	void Close();
	
	//写单个寄存器
	void writeRegister(int Ref, int addr, int value);
	//写多个寄存器
	void writeRegisters(int Ref, int addr, int value);
	//写单个线圈
	void writeCoil(int Ref, int addr, int value);
	//写多个线圈
	void writeCoils(int Ref, int addr, int value);
	
	//读单个寄存器
	void readRegister(int Ref, int addr, int value);
	//读多个寄存器
	void readRegisters(int Ref, int addr, int value);
	//读单个线圈
	void readCoil(int Ref, int addr, int value);
	//读多个线圈
	void readCoils(int Ref, int addr, int value);
private:
	SOCKET m_sock;
	int port;
	const char* address;
	int id;
};



 


