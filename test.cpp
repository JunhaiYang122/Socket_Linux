#include "Client.h"
using namespace std;

int main()
{
	//Client client("127.0.0.1",502,0); //连接本地回路，502端口，10号从机
	Client client("192.168.1.101",502,0); //连接本地回路，502端口，10号从机
	client.Connect();
	system("pause");
	client.writeRegister(0, 10, 323);// 将145号寄存器写为24323（十进制）
	//client.writeRegisters(1, 117, 9999);// 将145号寄存器写为24323（十进制）
	client.Close();
	system("pause");
	return 0;
}