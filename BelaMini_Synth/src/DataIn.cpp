/***** dataIn.cpp *****/
#include "DataIn.hpp"
#include <stdio.h>

void InputData::write(int inBuf[3])
{
		buf_[inBuf[0]][inBuf[1]] = inBuf[2];
}

void InputData::print(int typeIn, int channelIn)
{
	

		rt_printf("%d %d %d\n", typeIn, channelIn, buf_[typeIn][channelIn]);

}

int InputData::getVal(int typeIn, int channelIn)
{
	
	int value = buf_[typeIn][channelIn];
	return value;
 

}