#include <stdio.h>
#include <Windows.h>
//#include <tchar.h>
//#include <conio.h>

#define OBJECT_NAME("LocalINTERPRO")
#define PAGE_SIZE 0x1000

int main(void)
{
	HANDLE hMapping;
	char * buf;
	
	hMapping = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, PAGE_SIZE, OBJECT_NAME);
	
	buf = (char*)MapViewOfFile(hMapping, PAGE_READONLY, 0, 0, 0) ;
	
	strcopy(buf, "h4ppyyy") ;
	getch();
	
	UnmapViewOfFile(buf);
	CloseHandle(hMapping);
	
	return 0;
}
