#include<iostream>
#include <stdlib.h>     /* atof */
#include<stdio.h>
#include<string>
#include<vector>
#include<bitset>
#include<fstream>
using namespace std;
#define ADDU 1
#define SUBU 3
#define AND 4
#define OR  5
#define NOR 7
#define MemSize 65536 // memory size, in reality, the memory size should be 2^32, but for this lab, for the firstspace resaon, we keep it as this large number, but the memory is still 32-bit addressable.
#define R 0
#define lw 35
#define sw 43

int todecimal(int num)
{
	int dec = 0, base = 1, rem = 1;
	while (num > 0)//把地址变化为十进制
	{
		rem = num % 10;
		dec = dec + rem * base;
		base = base *2;
		num = num / 10;
	}

	return dec;
}


class RF
{
public:
	bitset<32> ReadData1, ReadData2; //定义两个bitset数组
	RF()
	{
		Registers.resize(32); //定义一个register
		Registers[0] = bitset<32>(0); // r[0]=0；
	}

	void ReadWrite(bitset<5> RdReg1, bitset<5> RdReg2, bitset<5> WrtReg, bitset<32> WrtData, bitset<1> WrtEnable)//定义操作对象
	{
		// implement the funciton by you.
		if (WrtEnable[0] == 1)
		{
			string temp = WrtReg.to_string();
			int num = atoi(const_cast<const char *>(temp.c_str()));
			Registers[todecimal(num)] = WrtData;
		}
		else {
			string temp1 = RdReg1.to_string();
			int num1 = atoi(const_cast<const char *>(temp1.c_str()));
			ReadData1 = Registers[todecimal(num1)];

			string temp2 = RdReg2.to_string();
			int num2 = atoi(const_cast<const char *>(temp2.c_str()));
			ReadData2 = Registers[todecimal(num2)];
		}
	}

	void OutputRF()
	{
		ofstream rfout;//定义一个of stream类
		rfout.open("RFresult.txt", std::ios_base::app);//打开结果RFresult
		if (rfout.is_open())
		{
			rfout << "A state of RF:" << endl;
			for (int j = 0; j<32; j++)
			{
				rfout << Registers[j] << endl;
			}

		}
		else cout << "Unable to open file";
		rfout.close();

	}
private:
	vector<bitset<32> >Registers;//定义register，里面每个元素都是32bit的2进制数

};

class ALU
{
public:
	bitset<32> ALUresult;
	bitset<32> ALUOperation(bitset<3> ALUOP, bitset<32> oprand1, bitset<32> oprand2)//成员函数，参数分别是两个操作数以及操作代码
	{
		// implement the ALU operations by you. 
		int aluop = 0; int j = 1;
		for (int i = 0; i < 3; i++)
		{
			aluop = aluop + ALUOP[i] * j;
			j = j *2;
		}

		unsigned long op1_num = oprand1.to_ulong();
		unsigned long op2_num = oprand2.to_ulong();

		unsigned long aluresult = 0;

		switch (aluop)
		{
		case(ADDU): aluresult = op1_num + op2_num; break;
		case(SUBU): aluresult = op1_num - op2_num; break;
		}

		//整数aluresult变回32位数组

		bitset<32> ALUresult(aluresult);

		return ALUresult;
	}
};

class INSMem
{
public:
	bitset<32> Instruction;
	INSMem()
	{
		IMem.resize(MemSize);
		ifstream imem;//定义一个ifstream类
		string line;
		int i = 0;
		imem.open("imem.txt");
		if (imem.is_open())//取指令
		{
			while (getline(imem, line))
			{
				IMem[i] = bitset<8>(line);
				i++;
			}

		}
		else cout << "Unable to open file";
		imem.close();

	}

	bitset<32> ReadMemory(bitset<32> ReadAddress) //成员函数READMEMORY,参数是32位的地址
	{
		// implement by you. (Read the byte at the ReadAddress and the following three byte)；
		unsigned long readaddress = ReadAddress.to_ulong();//读取readaddress地址中的指令放入Instruction中
		int i = 0;
		for (int j = 0; j < 4; j++)
		{
			bitset<8> a(IMem[readaddress + j]);
			for (int k = 7; k >= 0; k--)
			{
				Instruction[i] = a[k];
				i++;
			}
		}
		return Instruction;
	}

private:
	vector<bitset<8> > IMem;//定义一个指令内存数组，其中每个元素都是一个8bit的二进制数，刚好4个是一条指令

};

class DataMem
{
public:
	bitset<32> readdata;
	DataMem()
	{
		DMem.resize(MemSize);
		ifstream dmem;
		string line;
		int i = 0;
		dmem.open("dmem.txt");
		if (dmem.is_open())
		{
			while (getline(dmem, line))
			{
				DMem[i] = bitset<8>(line);
				i++;
			}
		}
		else cout << "Unable to open file";
		dmem.close();

	}
	bitset<32> MemoryAccess(bitset<32> Address, bitset<32> WriteData, bitset<1> readmem, bitset<1> writemem)
	{

		// implement by you.
		string Addr_temp = Address.to_string();//把Address变为十进制address
		int Addr_num = atoi(const_cast<const char *>(Addr_temp.c_str()));
		int address = 0, base = 1, rem = 1;
		while (Addr_num > 0)
		{
			rem = (Addr_num % 10);
			address = address + (int)rem * base;
			base = base * 2;
			Addr_num = Addr_num / 10;
		}

		if (readmem == 1)//读取内存中的数据
		{
			//读取address地址中的指令放入readdata中
			int i = 0;
			for (int j = 0; j < 4; j++)
			{
				bitset<8> a(DMem[address + j]);
				for (int k = 7; k >=0; k--)
				{
					readdata[i] = a[k];
					i++;
				}
			}

		}

		else if (writemem == 1) //把内存写入寄存器中
		{
			//读取WriteData里的数据放入内存DMem中
			int i = 0;
			for (int j = 3; j >=0; j--)
			{
				bitset<8> a;
				for (int k = 0; k <8; k++)
				{
					a[k] = WriteData[i];
					i++;
				}
				DMem[address + j] = a;
			}
		}
		return readdata;
	}
	void OutputDataMem()
	{
		ofstream dmemout;
		dmemout.open("dmemresult.txt");
		if (dmemout.is_open())
		{
			for (int j = 0; j< 1000; j++)
			{
				dmemout << DMem[j] << endl;
			}

		}
		else cout << "Unable to open file";
		dmemout.close();

	}
private:
	vector<bitset<8> > DMem;
};



int main()
{
	RF myRF;
	ALU myALU;
	INSMem myInsMem;
	DataMem myDataMem;
	bitset<32> INS(0);

	while (1)
	{
		bitset<32> Instruction = myInsMem.ReadMemory(INS);// Fetch
		unsigned long j = INS.to_ulong();
		j += 4;
		bitset<32> newad(j);

		bitset<32> out = 0xffffffff;
		if (Instruction == out)
			break;	// If current insturciton is "11111111111111111111111111111111", then break;
		else
		{
			bitset<6> op(0);
			// decode(Read RF)
			for (int i = 0; i < 6; i++) {
				op[i] = Instruction[5-i];
			}//得到op code
			bitset<3> func(0);
			for (int i = 0; i < 3; i++) {
				func[i] = Instruction[31-i];
			}//得到function code
			unsigned long ops = op.to_ulong();
			//unsigned long funcs = func.to_ulong();
			switch (ops)
			{
			case R:
			{//addu,subu,and,or,nor
				bitset<5> Read1;
				bitset<5> Read2;
				bitset<5> Write;
				for (int i = 0; i < 5; i++) {
					Read1[i] = Instruction[10-i];
					Read2[i] = Instruction[15 - i];
					Write[i] = Instruction[20 - i];//因为是load，所以只有2个寄存器
				}
				myRF.ReadWrite(Read1, Read2, 0, 0, 0);
				bitset<32> ALUResult = myALU.ALUOperation(func, myRF.ReadData1, myRF.ReadData2);
				myRF.ReadWrite(Read1, Read1, Write, ALUResult, 1);
				break;
			}
			case lw:
			{//是load, 要从25-21，20-16拿到2个寄存器
				bitset<5> Read1(0);
				bitset<5> Write(0);
				bitset<32> writedata(0);
				bitset<3> alu(string("001"));
				for (int i = 0; i < 5; i++) {
					Read1[i] = Instruction[10-i];
					Write[i] = Instruction[15-i];//因为是load，所以只有2个寄存器
				}
				myRF.ReadWrite(Read1, Write, 0, 0, 0);//得到一个寄存器里的值,放在了ReadData1里，现在要去将立即数取出来
				bitset<32> imm(0);//根据立即数首位去进行符号扩展
				if (Instruction[16] == 0) imm = 0;
				else imm = 1;
				for (int i = 0; i < 16; i++)
				{
					imm[i] = Instruction[31-i];
				}
				for (int i = 16; i < 32; i++)
				{
					imm[i] = 0;
				}
				bitset<32> ALUResult = myALU.ALUOperation(alu, myRF.ReadData1, imm);//返回一个32位的二进制数，去这个数对应的内存地址里去找，然后写回RF
				writedata = myDataMem.MemoryAccess(ALUResult, 0, 1, 0);
				myRF.ReadWrite(Read1, Read1, Write, writedata, 1);
				break;
			}
			case sw:
			{//是store, 要从25-21，20-16拿到2个寄存器
				bitset<5> Read1(0);
				bitset<5> Read2(0);
				bitset<32> writedata(0);
				bitset<3> alu (string("001"));
				for (int i = 0; i < 5; i++) {
					Read1[i] = Instruction[10-i];
					Read2[i] = Instruction[15-i];//因为是load，所以只有2个寄存器
				}
				myRF.ReadWrite(Read1, Read2, 0, 0, 0);//得到一个寄存器里的值,放在了ReadData1里，现在要去将立即数取出来
				bitset<32> imm(0);//根据立即数首位去进行符号扩展
				if (Instruction[16] == 0) imm = 0;
				else imm = 1;
				for (int i = 0; i < 16; i++)
				{
					imm[i] = Instruction[31 - i];
				}
				for (int i = 16; i < 32; i++)
				{
					imm[i] = 0;
				}
				bitset<32> ALUResult = myALU.ALUOperation(alu, myRF.ReadData1, imm);//返回一个32位的二进制数，去这个数对应的内存地址里去找，找到后讲READ DATA 2写入
				writedata = myDataMem.MemoryAccess(ALUResult, myRF.ReadData2, 0, 1);
				break;
			}

			}

			// Execute

			// Read/Write Mem

			// Write back to RF
			INS = newad;
			myRF.OutputRF(); // dump RF;
		}
	}

	myDataMem.OutputDataMem(); // dump data mem

	return 0;

}
