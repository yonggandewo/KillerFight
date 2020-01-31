#include<windows.h>
#include<iostream>
#include<conio.h>
#include<fstream>
#include<random>
#include<vector>
#include<ctime>
using namespace std;
HANDLE hOut=GetStdHandle(STD_OUTPUT_HANDLE);
HWND hWnd=GetConsoleWindow();
string PlayerName,FilePlayerName;
const char* Pic="╰══╗═╯╭══╦═╗╔═╗　　╗　　　═╗\n╭═╝╚═╮　　　║　　║　║══╣╚╮══╠╗\n╔══╠═╗╔══╠═╗║　║╗　║　　　　║║\n　　╔║╗　　　　║　　║　║║　║╭╝　　║║\n╔═╯║╰╗╔══╠═╗╚╔╯╰═║║　══╠╩\n╚═╰╝═╝　　╚╯　　╚╯╝╚═╯╚　╚═╯╝\n                  Ｖ１．０．２\n";
const string SkillName[5]={"瞄准","复活","诅咒","重击","看破"}; 
long long Coin,Skill[5];
string ll2str(long long x){
	string s;
	while(x){
		s=(char)(x%10+'0')+s;
		x/=10; 
	}
	return s;
}
void Color(int f,int b){
	SetConsoleTextAttribute(hOut,f*16+b);
}
void SetWindow(short r,short c){
	COORD size={c,100};
	SMALL_RECT rc;
	RECT rc2;
	rc.Right=c-1;
	rc.Bottom=r-1;
	rc.Left=rc.Top=0;
	SetConsoleTitle("杀手对决");
	SetConsoleWindowInfo(hOut,true,&rc);
	SetConsoleScreenBufferSize(hOut,size);
	GetWindowRect(hWnd, &rc2);
	SetWindowLongPtr(hWnd,GWL_STYLE,GetWindowLong(hWnd,GWL_STYLE)&~WS_THICKFRAME&~WS_MAXIMIZEBOX&~WS_MINIMIZEBOX);
	SetWindowPos(hWnd,NULL,rc2.left,rc2.top,rc2.right-rc2.left,rc2.bottom-rc2.top,NULL);
}
void HideCursor(){
	CONSOLE_CURSOR_INFO cci;
	GetConsoleCursorInfo(hOut,&cci);
	cci.bVisible=false;
	SetConsoleCursorInfo(hOut,&cci);
}
bool SetCursorPosition(short x,short y){
    return SetConsoleCursorPosition(hOut,(COORD){y,x});
}
COORD GetCursorPosition(){
    CONSOLE_SCREEN_BUFFER_INFO sbi;
    GetConsoleScreenBufferInfo(hOut,&sbi);
    return sbi.dwCursorPosition;
}
void _set(int id,int line,int col1,int col2,string st){
    SetCursorPosition(line,0);
    Color(col1/16,col1%16);
    cout<<id<<". "<<st<<endl;
    Color(col2/16,col2%16);
}
int Choose(const vector<string>&op,int co=240,int uc=15,int sel=0){
	COORD cpos=GetCursorPosition();
	int len=op.size(),s1=cpos.Y,s2=s1,cur=sel;
	vector<int>pos;
	char ch;
	for(int i=0;i<len;i++){
		pos.push_back(s1);
		if(i==sel)SetConsoleTextAttribute(hOut,co);
		cout<<i<<". "<<op[i]<<endl;
		if(i==sel)SetConsoleTextAttribute(hOut,uc);
		s1++;
	}
	while((ch=_getch())!='\r'){
		if(ch==72||ch=='W'||ch=='w'){
			_set(cur,s2+cur,uc,co,op[cur]);
			if(!cur)cur=len-1;
			else cur--;
			_set(cur,s2+cur,co,uc,op[cur]);
		}else if(ch==80||ch=='S'||ch=='s'){
			_set(cur,s2+cur,uc,co,op[cur]);
			if(cur==len-1)cur=0;
			else cur++;
			_set(cur,s2+cur,co,uc,op[cur]);
		}else if(isdigit(ch)){
			int x=ch-'0';
			if(x>=len)continue;
			if(x!=cur){
				_set(cur,s2+cur,uc,co,op[cur]);
				cur=x;
				_set(cur,s2+cur,co,uc,op[cur]);
			}
		}
	}
	SetCursorPosition(cpos.Y+len,0);
	return cur;
}
void Gaming(){
	srand(time(NULL));
	system("cls");
	puts("你的技能:");
	for(int i=0;i<5;i++)cout<<"  "<<SkillName[i]<<":"<<Skill[i]<<"级"<<endl;
	puts("\n请选择顺位");
	vector<string>Number={"退出","1号","2号","3号","4号","5号"};
	int Choice=Choose(Number);
	if(!Choice)return;
	int PlayerSkill[6][5];
	for(int i=1;i<6;i++){
		PlayerSkill[i][0]=i;
		for(int j=1;j<5;j++)PlayerSkill[i][j]=0;
	}
	PlayerSkill[1][1]=3;
	PlayerSkill[1][2]=4;
	PlayerSkill[2][1]=1;
	PlayerSkill[2][2]=2;
	PlayerSkill[3][1]=1;
	PlayerSkill[4][3]=1;
	PlayerSkill[5][4]=1;
	for(int i=0;i<5;i++)PlayerSkill[Choice][i]+=Skill[i];
	if(PlayerSkill[Choice][0]>5)PlayerSkill[Choice][0]=5;
	if(PlayerSkill[Choice][1]>5)PlayerSkill[Choice][1]=5;
	if(PlayerSkill[Choice][2]>4)PlayerSkill[Choice][2]=4;
	if(PlayerSkill[Choice][3]>3)PlayerSkill[Choice][3]=3;
	if(PlayerSkill[Choice][4]>1)PlayerSkill[Choice][4]=1;
	bool Alive[6]={0,1,1,1,1,1};
	int Turn=1,Round=1,Chance=1,Goal,Get,Goal2,AliveNumber,Liver;
	while(1){
		system("cls");
		cout<<"第"<<Round<<"轮:"<<endl;
		puts("存活者:");
		for(int i=1;i<6;i++)if(Alive[i])cout<<i<<" ";
		cout<<endl<<endl<<"轮到"<<Turn<<"号杀手了"<<endl;
		if(Turn==Choice){
			puts("你要杀谁?");
			char Input=getch();
			while(!(isdigit(Input)&&Input>'0'&&Input<'6'&&Input!='0'+Turn&&Alive[Input-'0']))Input=getch();
			Goal=Input-'0';
		}else{
			Sleep(1000);
			Goal=rand()%5+1;
			while(Goal==Turn||!Alive[Goal])Goal=rand()%5+1;
		}
		cout<<Turn<<"号选择了"<<Goal<<"号"<<endl;
		Sleep(1000);
		Get=rand()%6+1;
		cout<<Turn<<"号掷出了"<<Get<<"点"<<endl;
		Sleep(1000);
		if(Get<=PlayerSkill[Turn][0]){
			cout<<Turn<<"号杀死了"<<Goal<<"号"<<endl;
			Sleep(1000);
			if(PlayerSkill[Goal][1]){
				cout<<Goal<<"号使用复活权"<<endl;
				PlayerSkill[Goal][1]--; 
				Sleep(1000);
				if(PlayerSkill[Turn][4]){
					cout<<Turn<<"号无视了"<<Goal<<"号的复活权,直接将其杀死"<<endl;
					Sleep(1000);
					Alive[Goal]=0;
				}else if(PlayerSkill[Goal][2]){
					Goal2=rand()%5+1;
					while(Goal2==Goal||!Alive[Goal2])Goal2=rand()%5+1;
					cout<<Goal<<"号诅咒了"<<Goal2<<"号"<<endl;
					Sleep(1000);
					if(rand()%4<=PlayerSkill[Goal][2]){
						cout<<Goal2<<"号死了"<<endl;
						Alive[Goal2]=0;
					}else cout<<"但"<<Goal2<<"号没死"<<endl;
					Sleep(1000);
				}
			}else Alive[Goal]=0;
		}else{
			cout<<Turn<<"号没有杀死"<<Goal<<"号"<<endl;
			Sleep(1000); 
		}
		AliveNumber=0;
		for(int i=1;i<6;i++)if(Alive[i]){
			AliveNumber++;
			Liver=i;
		}
		if(!AliveNumber){
			puts("你们同归于尽了");
			break;
		}
		if(AliveNumber==1){
			cout<<Liver<<"号获胜!"<<endl;
			break;
		}
		if(!Alive[Choice]){
			puts("你死了!");
			break;
		}
		if(Alive[Turn]&&Chance<=PlayerSkill[Turn][3]){
			cout<<Turn<<"号使用再次攻击权"<<endl;
			Sleep(1000);
			Chance++;
		}else{
			Turn=Turn%5+1;
			while(!Alive[Turn])Turn=Turn%5+1;
			if(Turn==1)Round++;
			Chance=1; 
		}
	}
	if(Alive[Choice]){
		Coin+=1000;
		puts("你获胜了!\n获得金币:1000");
		getch();
		return;
	}else{
		long long GotCoin=1000>>AliveNumber;
		Coin+=GotCoin;
		cout<<"你失败了!\n获得金币:"<<GotCoin;
		getch();
		return; 
	}
	return;
}
int main(){
	HideCursor();
	SetWindow(48,100);
	Color(0,15);
	puts("读档中......");
	ifstream in("data.sgr");
	if(!in.is_open()){
		cout<<"对不起,读档时发生错误,请确认你的存档正常";
		getch();
		in.close();
		return 0;
	}
	getline(in,FilePlayerName);
	system("cls");
	puts(Pic);
	cout<<"杀手先生请登陆\n杀手名:";
	getline(cin,PlayerName);
	if(PlayerName!=FilePlayerName){
		puts("请开始你的杀手生涯");
		Coin=0;
		for(int i=0;i<5;i++)Skill[i]=0;
		ofstream kout("data.sgr");
		kout<<PlayerName<<endl<<Coin<<' ';
	}else{
		puts("先生,欢迎回来");
		in>>Coin;
		for(int i=0;i<5;i++)in>>Skill[i];
	}
	in.close();
	getch();
	while(1){
		system("cls");
		puts(Pic);
		cout<<"你的金币:"<<Coin<<endl;
		vector<string>Action={"开始游戏","技能商店","退出游戏"};
		switch(Choose(Action)){
			case 0:{
				Gaming();
				break;
			}
			case 1:{
				system("cls"); 
				long long UpgradeCoin[5];
				for(int i=0;i<5;i++)UpgradeCoin[i]=100<<Skill[i];
				cout<<"你的金币:"<<Coin<<endl;
				puts("你的技能:");
				for(int i=0;i<5;i++)cout<<"  "<<SkillName[i]<<":"<<Skill[i]<<"级"<<endl;
				puts("你要升级哪个技能?");
				vector<string>Upgrade;
				for(int i=0;i<5;i++)Upgrade.push_back(SkillName[i]+":需要"+ll2str(UpgradeCoin[i])+"金币");
				Upgrade.push_back("退出");
				int Choice=Choose(Upgrade);
				if(Choice==5)break;
				if(UpgradeCoin[Choice]<=Coin){
					puts("升级成功!");
					Coin-=UpgradeCoin[Choice];
					Skill[Choice]++;
				}else puts("金币不足!");
				getch();
				break;
			}
			case 2:{
				system("cls"); 
				ofstream kout("data.sgr");
				kout<<PlayerName<<endl<<Coin<<' ';
				for(int i=0;i<5;i++)kout<<Skill[i]<<' ';
				puts("先生慢走不送");
				getch();
				return 0;
			}
		}
		ofstream kout("data.sgr");
		kout<<PlayerName<<endl<<Coin<<' ';
		for(int i=0;i<5;i++)kout<<Skill[i]<<' ';
	}
	return 0;
}
