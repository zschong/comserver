#ifndef __MODBUS_H__
#define __MODBUS_H__
#include "crc16.h"

class ModbusBase
{
public:
	unsigned char data[256];	

public:
	void SetChar(int i, unsigned char value)
	{
		data[i%sizeof(data)] = value;
	}
	void SetShortHL(int i, unsigned short value)
	{
		SetChar(i+0, value >> 8);
		SetChar(i+1, value >> 0);
	}
	void SetShortLH(int i, unsigned short value)
	{
		SetChar(i+1, value >> 8);
		SetChar(i+0, value >> 0);
	}
public:
	unsigned char GetChar(int i)const
	{
		return data[i%sizeof(data)];
	}
	unsigned short GetShortHL(int i)const
	{
		return (GetChar(i)<<8) | GetChar(i+1);
	}
	unsigned short GetShortLH(int i)const
	{
		return (GetChar(i+1)<<8) | GetChar(i);
	}
	const bool GetBit(int i)
	{
		return (GetChar(i/8) >> (i%8)) & 1;
	}
public:
	void SetSlave(unsigned char value){SetChar(0,value);}
	void SetFcode(unsigned char value){SetChar(1,value);}
public:
	unsigned char GetSlave(void){return GetChar(0);}
	unsigned char GetFcode(void){return GetChar(1);}
public:
	unsigned short CalcCRC(int l){return CRC16(data,l);}
};
class Requestx01 : public ModbusBase
{
public://[slave,fcode,offset,count,crc].length=[1,1,2,2,2]
	void SetOffset(unsigned short value){SetShortHL(2,value);}
	void SetCount(unsigned short value)	{SetShortHL(4,value);}
	void SetCRC(unsigned short value)	{SetShortLH(6,value);}
	unsigned short GetOffset(void)		{return GetShortHL(2);}
	unsigned short GetCount(void)		{return GetShortHL(4);}
	unsigned short GetCRC(void)			{return GetShortLH(6);}
	unsigned short CRCLen(void)			{return 1+1+2+2;}
	unsigned short CalcCRC(void)		{return CRC16(data, CRCLen());}
	const bool     Check(void)			{return CalcCRC()==GetCRC();}
};
#define Requestx02 Requestx01
#define Requestx03 Requestx01
#define Requestx04 Requestx01
class Requestx05 : public ModbusBase
{
public://[slave,fcode,offset,count,crc].length=[1,1,2,2,2]
	void SetOffset(unsigned short value){SetShortHL(2,value);}
	void SetValue(unsigned short value)	{SetShortHL(4,value);}
	void SetCRC(unsigned short value)	{SetShortLH(6,value);}
	unsigned short GetOffset(void)		{return GetShortLH(2);}
	unsigned short GetValue(void)		{return GetShortLH(4);}
	unsigned short GetCRC(void)			{return GetShortLH(6);}
	unsigned short CRCLen(void)			{return 1+1+2+2;}
	unsigned short CalcCRC(void)		{return CRC16(data, CRCLen());}
	const bool     Check(void)			{return CalcCRC()==GetCRC();}
};
#define Requestx06 Requestx05
class Requestx0f : public ModbusBase
{
public://[slave,fcode,offset,count,crc].length=[1,1,2,2,2]
	void SetOffset(unsigned short value)		{SetShortHL(2,value);}
	void SetCount(unsigned short value)			{SetShortHL(4,value);}
	void SetBcount(unsigned char value)			{SetChar(6, value);}
	void SetValue(int i, unsigned char value)	{SetChar(7+i, value);}
	void SetCRC(unsigned short value)			{SetShortLH(6,value);}
	unsigned short GetOffset(void)				{return GetShortHL(2);}
	unsigned short GetCount(void)				{return GetShortHL(4);}
	unsigned char  GetBcount(void)				{return GetChar(6);}
	unsigned short GetCRC(void)					{return GetShortLH(7+GetBcount());}
	unsigned short CRCLen(void)					{return 1+1+2+2;}
	unsigned short CalcCRC(void)				{return CRC16(data, CRCLen());}
	const bool     Check(void)					{return CalcCRC()==GetCRC();}
};
class Requestx10 : public ModbusBase
{
public://[slave,fcode,offset,count,crc].length=[1,1,2,2,2]
	void SetOffset(unsigned short value)		{SetShortHL(2,value);}
	void SetCount(unsigned short value)			{SetShortHL(4,value);}
	void SetBcount(unsigned char value)			{SetChar(6, value);}
	void SetCRC(unsigned short value)			{SetShortLH(6,value);}
	void SetValue(int i, unsigned char value)	{SetShortHL(7+i*2, value);}
	unsigned short GetOffset(void)				{return GetShortHL(2);}
	unsigned short GetCount(void)				{return GetShortHL(4);}
	unsigned char  GetBcount(void)				{return GetChar(6);}
	unsigned short GetCRC(void)					{return GetShortLH(7+GetBcount());}
	unsigned short CRCLen(void)					{return 1+1+2+2;}
	unsigned short CalcCRC(void)				{return CRC16(data, CRCLen());}
	const bool     Check(void)					{return CalcCRC()==GetCRC();}
};
class Responsex01 : public ModbusBase
{
public://[slave,fcode,bcount,data,crc].len=[1,1,1,bcount,2];
	unsigned char GetBcount(void){return GetChar(2);}
	unsigned char GetValue(int i){return GetChar(3+i/8)>>(i%8) & 1;}
	unsigned short GetCRC(void)	 {return GetShortLH(4+GetBcount());}
	unsigned short CRCLen(void)	 {return 1+1+1+GetBcount();}
	unsigned short CalcCRC(void) {return CRC16(data, CRCLen());}
	const bool     Check(void)	 {return CalcCRC()==GetCRC();}
};
#define Responsex02 Responsex01
class Responsex03 : public ModbusBase
{
public://[slave,fcode,bcount,data,crc].len=[1,1,1,bcount,2];
	void SetBcount(unsigned char value)		  {SetChar(2, value);}
	void SetValue(int i, unsigned short value){SetShortHL(3+i*2, value);}
	void SetCRC(unsigned short value)		  {SetShortHL(4+GetBcount(), value);}
	unsigned char  GetBcount(void){return GetChar(2);}
	unsigned short GetValue(int i){return GetShortHL(3+i*2);}
	unsigned short GetCRC(void)	  {return GetShortLH(4+GetBcount());}
	unsigned short CRCLen(void)	  {return 1+1+1+GetBcount();}
	unsigned short CalcCRC(void)  {return CRC16(data, CRCLen());}
	const bool     Check(void)	  {return CalcCRC()==GetCRC();}
};
#define Responsex04 Responsex03
class Responsex05 : public ModbusBase
{
public://[slave,fcode,offset,value,crc].len=[1,1,2,2,2];
	unsigned char  GetOffs(void)  {return GetChar(2);}
	unsigned short GetValue(void) {return GetShortHL(4);}
	unsigned short GetCRC(void)	  {return GetShortLH(6);}
	unsigned short CRCLen(void)	  {return 1+1+2+2;}
	unsigned short CalcCRC(void)  {return CRC16(data, CRCLen());}
	const bool     Check(void)	  {return CalcCRC()==GetCRC();}
};
#define Responsex06 Responsex05
class Responsex0f : public ModbusBase
{
public://[slave,fcode,offset,value,crc].len=[1,1,2,2,2];
	unsigned char  GetOffs(void)  {return GetChar(2);}
	unsigned short GetCount(void) {return GetShortHL(4);}
	unsigned short GetCRC(void)	  {return GetShortLH(6);}
	unsigned short CRCLen(void)	  {return 1+1+2+2;}
	unsigned short CalcCRC(void)  {return CRC16(data, CRCLen());}
	const bool     Check(void)	  {return CalcCRC()==GetCRC();}
};
#define Responsex10 Responsex0f

#endif//__MODBUS_H__
