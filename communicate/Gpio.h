#pragma once

#define BUFFER_MAX 100
#define DIRECTION_MAX 100
#define IN 0
#define OUT 1
#define LOW 0
#define HIGH 1
#define POUT 21

class GPIO
{
public:
	static int Export(int pin);
	static int Unexport(int pin);
	static int Direction(int pin, int dir);
	static int Write(int pin, int value);
	static int Read(int pin);
	static int Edge(int pin, char * mode);
};