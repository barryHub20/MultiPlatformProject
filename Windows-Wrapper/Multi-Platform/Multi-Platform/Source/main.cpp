/*************************************************************
Framework version 2.0

Author: Tan Yie Cher Test

Date: 25/12/2016
Date: 23/6/2016 (ver. 1.0)
/*************************************************************/
#include "ControllerPC.h"

int main(void)
{
	/* Memory leak checker */
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);	//call this if program does not exit at same place everytime

	Controller_PC::Instance()->Init();
	Controller_PC::Instance()->Run();
	Controller_PC::Instance()->Exit();
}