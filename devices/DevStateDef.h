#pragma once

//关键等级
enum Priority
{
	CRITICAL = 0,
	IMPORTANT,
	UNIMPORTANT
};

// 运行状态
enum RUNSTATE
{
	TURNON,			//开机
	TURNOFF			//关机
};

// 工作模式
enum WORKMODE
{
	PQ=0,
	VF,
	VSG
};

// 运行模式
enum RUNMODE
{
	AUTO=0,
	MANUAL
};