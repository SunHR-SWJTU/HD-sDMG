#ifndef __CD4051_H
#define __CD4051_H

#include <Arduino.h>
#include <string.h>
#include <stdio.h>
#include "OLED091.h"

#define PACKAGE_FRAMENUM 7   //希望读到多少帧后发送

/*------------------------------class CD4051_Chip------------------------------*/
class CD4051_Chip
{
private:
  uint8_t _ssPin = 0;       //片选引脚
  uint8_t _ioPin = 0;
  uint8_t _lineNum = 0; //该芯片上的通道数，<=8
  int **_data = NULL;   //二维数组指针，存储每个通道的缓存

public:
  CD4051_Chip(uint8_t ssPin, uint8_t ioPin, uint8_t lineNum);
  CD4051_Chip(uint8_t ioPin, uint8_t lineNum);

  uint8_t GetLineNum(void);           //返回通道数
  void ChooseChip(bool ch);           //片选
  void ReadLine(uint8_t line, uint8_t index);    //读取某一通道上的值，编号从1开始
  int GetData(uint8_t line, uint8_t index);      //获取某一通道的数据
  void debug(void);

  ~CD4051_Chip();

};


/*------------------------------class CD4051_Manager------------------------------*/
class CD4051_Manager
{
private:
  uint8_t _addrAPin;
  uint8_t _addrBPin;
  uint8_t _addrCPin;
  uint8_t _chipNum = 0;       //芯片数
  uint8_t _groupNum = 0;      //通道组数
  uint8_t _group = 1;         //当前正在读的通道组，编号从1开始
  uint8_t *_index = NULL;     //每个通道组的缓存的索引，每一个索引值 0~BUFLENGTH
  uint32_t _frameNum = 0;     //帧序号

  unsigned long _remember = 0;
  uint8_t _freq = 130;         //采样频率，1~130，单位Hz
  uint32_t _Tc = 0;            //采样周期，= 1000000/freq，单位us

  uint32_t _packageTc = 0;       // 数据包周期，= _Tc * PACKAGE_FRAMENUM
  uint16_t _packageLen = PACKAGE_FRAMENUM * 46;   // sendPackage数组的总长度
  uint8_t _sendPackage[PACKAGE_FRAMENUM * 46];    // 一帧46字节
  uint16_t _sendPackageIndex = 0;                 // sendPackage数组的索引

  CD4051_Chip *_chip = NULL;

public:
  CD4051_Manager(uint8_t addrAPin, uint8_t addrBPin, uint8_t addrCPin, uint8_t chipNum, CD4051_Chip chip[]);

  void ChooseLine(uint8_t line);  //选中某个通道
  void ReadGroup(void);           //读取某一组通道数据，编号从1开始
  void SendGroup(void);           //发送某一组通道数据，编号从1开始
  void Loop(void);                //读取并发送某一组通道数据，编号从1开始
  void debug(void);
  void setFreq(uint8_t freq);     //设置采样频率，1~130，单位Hz

  ~CD4051_Manager();
};

#endif
