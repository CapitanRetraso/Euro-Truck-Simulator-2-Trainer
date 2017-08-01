#include <iostream>
#include <Windows.h>
#include <string>
#include <ctime>

DWORD FindDmaAddy(int PointerLevel, HANDLE hProcHandle, DWORD Offsets[], DWORD BaseAddress);
void WriteToMemory(HANDLE hProcHandle);

void setColor(unsigned short color)
{
	HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hcon, color);
}
using namespace std;

std::string GameName = "eurotrucks2";
LPCSTR LGameWindow = "Euro Truck Simulator 2";
std::string GameStatus;

bool GameAvaliable;
bool UpdateNext;



//Money Hackz
bool MoneyStat;
BYTE MoneyValue[] = {0xA0, 0x1C, 0x2B, 0x0};
DWORD MoneyAddress = {0x0218DFB4};
DWORD MoneyOffsets[] = {0x28, 0x28};




int main()
{
	HWND hGameWindow = NULL;
	int TimeSinceLastUpdate = clock();
	int GameAvaliableTimer = clock();
	int OnePressTimer = clock();
	DWORD dwProcID = NULL;
	HANDLE hProcHandle = NULL;
	UpdateNext = true;
	std::string sMoneyStat = "OFF";


	while (!GetAsyncKeyState(VK_INSERT))
	{
		if (clock() - GameAvaliableTimer > 100)
		{
			GameAvaliableTimer = clock();
			GameAvaliable = false;
			hGameWindow = FindWindow(NULL, LGameWindow);
			if(hGameWindow)
			{
				GetWindowThreadProcessId(hGameWindow, &dwProcID);
				if (dwProcID != 0)
				{
					hProcHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcID);
					if (hProcHandle == INVALID_HANDLE_VALUE || hProcHandle == NULL)
					{
						GameStatus = "Failed to get handle!";
					}
					else
					{
						GameStatus = "ETS2 ready!";
						GameAvaliable = true;
					}
				}
				else
				{
					GameStatus = "Failed to get process ID!";
				}

			}
			else
			{
				GameStatus = "ETS2 not found!";
			}
			if (UpdateNext || clock() - TimeSinceLastUpdate > 5000)
			{
				system("cls");
				setColor(14); std::cout << "-----------------------------------------------------" << std::endl;
				std::cout << "|              EURO TRUCK SIMULATOR 2               |" << std::endl;
				std::cout << "-----------------------------------------------------" << std::endl << std::endl; setColor(7);
				std::cout << " GAME STATUS:  " << GameStatus << std::endl;
				setColor(12);  std::cout << " WARNING: Make sure you are running the 32 bit version of the game" << std::endl << std::endl << std::endl << std::endl;
				setColor(7); std::cout << " F1 - Unlimited Money -->"; setColor(11); cout << sMoneyStat; setColor(7); cout << "<--" << std::endl << std::endl;
				std::cout << "\n" << std::endl;
				std::cout << " Press INSERT to close" << std::endl;
				std::cout << "-----------------------------------------------------" << std::endl;
				setColor(13); std::cout << " Made by: Capitan Retraso" << std::endl; setColor(7);
				UpdateNext = false;
				TimeSinceLastUpdate = clock();
			}

			if (GameAvaliable)
			{
				WriteToMemory(hProcHandle);

			}
		}

		if (clock() - OnePressTimer > 500)
		{
			if (GameAvaliable)
			{
				if (GetAsyncKeyState(VK_F1))
				{
					OnePressTimer = clock();
					MoneyStat = !MoneyStat;
					UpdateNext = true;
					if (MoneyStat)sMoneyStat = "ON";
					else sMoneyStat = "OFF";
				}
			}
		}
	}
	CloseHandle(hProcHandle);
	DestroyWindow(hGameWindow);

	return ERROR_SUCCESS;
}


DWORD FindDmaAddy(int PointerLevel, HANDLE hProcHandle, DWORD Offsets[], DWORD BaseAddress)
{
	DWORD pointer = BaseAddress;
	DWORD pTemp;
	DWORD pointerAddr;
	for (int c = 0; c < PointerLevel; c++)
	{
		if (c == 0)
		{
			ReadProcessMemory(hProcHandle, (LPCVOID)pointer, &pTemp, sizeof(pTemp), NULL);
		}
		pointerAddr = pTemp + Offsets[c];
		ReadProcessMemory(hProcHandle, (LPCVOID)pointerAddr, &pTemp, sizeof(pTemp), NULL);
	}
	return pointerAddr;
}



void WriteToMemory(HANDLE hProcHandle)
{
	DWORD AddressToWrite;
	if(MoneyStat)
	{ 
		DWORD AddressToWrite = FindDmaAddy(2, hProcHandle, MoneyOffsets, MoneyAddress);  //First is number of pointers
		WriteProcessMemory(hProcHandle, (BYTE*)AddressToWrite, &MoneyValue, sizeof(MoneyValue), NULL);
	}
}