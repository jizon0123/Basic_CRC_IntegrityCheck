#include <iostream>
#include <windows.h>
#include "CRC.h"

int main();
int calcAddr(void*);
void* checkcrc(int*);

int main()
{
	BOOL IntegrityMain;
	int TableMain = (int)CRC::Calculate((char*)calcAddr(main), 0x5000, CRC::CRC_32());
	int ParamMainCRC[4] = { (int)calcAddr(main), 0x5000, TableMain,(int)&IntegrityMain};
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)(checkcrc), ParamMainCRC, NULL, NULL);

	while (TRUE)
	{
		if (IntegrityMain)
			std::cout << "Hello World!\n";
		else
		{
			std::cout << "Integrity Check Failed" << std::endl;
		}
		Sleep(1000);
	}
}


/*
	params[0] : Check Base
	params[1] : Check Size
	params[2] : Original CRC TABLE
	params[3] : Integrity Pointer
*/

void* checkcrc(int* params)
{
#ifdef  _DEBUG
	printf("CRC_BASE = 0x%X\n", params[0]);
	printf("CRC_SIZE = 0x%X\n", params[1]);
#endif 
	BOOL* Integrity = (BOOL*)params[3];
	while (TRUE)
	{
		if (params[2] != CRC::Calculate((char*)params[0], params[1], CRC::CRC_32()))
			*Integrity = FALSE;
		else
			*Integrity = TRUE;
	}
	return NULL;
}

int calcAddr(void* func)
{
	auto FuncOffset = *(int*)((char*)func + 1) + 5;
	return (int)func + FuncOffset;
}
