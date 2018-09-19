#include <stdio.h>
#include <Windows.h>
//#include <tchar.h>
//#include <conio.h>

#define OBJECT_NAME("LocalINTERPRO")

int main(void)
{
	HANDLE hMapping ;
	char* buf ;
	
	hMapping = OpenFileMapping(FILE_MAP_REA | FILE_MAP_WRITE, FALSE, OBJECT_NAME) ;
	
	buf = (char*)MapViewOfFile(hMapping, PAGE_READONLY, 0, 0, 0) ;
	
	printf("buf: %s\n", buf) ;
	getch();
	
	UnmapViewOfFile(buf) ;
	CloseHandle(hMapping) ;
	
	return 0 ;
}
