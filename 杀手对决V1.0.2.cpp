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
const char* Pic="�t�T�T�[�T�s�q�T�T�j�T�[�X�T�[�����[�������T�[\n�q�T�a�^�T�r�������U�����U���U�T�T�g�^�r�T�T�d�[\n�X�T�T�d�T�[�X�T�T�d�T�[�U���U�[���U���������U�U\n�����X�U�[���������U�����U���U�U���U�q�a�����U�U\n�X�T�s�U�t�[�X�T�T�d�T�[�^�X�s�t�T�U�U���T�T�d�m\n�^�T�t�a�T�a�����^�s�����^�s�a�^�T�s�^���^�T�s�a\n                  �֣���������\n";
const string SkillName[5]={"��׼","����","����","�ػ�","����"}; 
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
	SetConsoleTitle("ɱ�ֶԾ�");
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
	puts("��ļ���:");
	for(int i=0;i<5;i++)cout<<"  "<<SkillName[i]<<":"<<Skill[i]<<"��"<<endl;
	puts("\n��ѡ��˳λ");
	vector<string>Number={"�˳�","1��","2��","3��","4��","5��"};
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
		cout<<"��"<<Round<<"��:"<<endl;
		puts("�����:");
		for(int i=1;i<6;i++)if(Alive[i])cout<<i<<" ";
		cout<<endl<<endl<<"�ֵ�"<<Turn<<"��ɱ����"<<endl;
		if(Turn==Choice){
			puts("��Ҫɱ˭?");
			char Input=getch();
			while(!(isdigit(Input)&&Input>'0'&&Input<'6'&&Input!='0'+Turn&&Alive[Input-'0']))Input=getch();
			Goal=Input-'0';
		}else{
			Sleep(1000);
			Goal=rand()%5+1;
			while(Goal==Turn||!Alive[Goal])Goal=rand()%5+1;
		}
		cout<<Turn<<"��ѡ����"<<Goal<<"��"<<endl;
		Sleep(1000);
		Get=rand()%6+1;
		cout<<Turn<<"��������"<<Get<<"��"<<endl;
		Sleep(1000);
		if(Get<=PlayerSkill[Turn][0]){
			cout<<Turn<<"��ɱ����"<<Goal<<"��"<<endl;
			Sleep(1000);
			if(PlayerSkill[Goal][1]){
				cout<<Goal<<"��ʹ�ø���Ȩ"<<endl;
				PlayerSkill[Goal][1]--; 
				Sleep(1000);
				if(PlayerSkill[Turn][4]){
					cout<<Turn<<"��������"<<Goal<<"�ŵĸ���Ȩ,ֱ�ӽ���ɱ��"<<endl;
					Sleep(1000);
					Alive[Goal]=0;
				}else if(PlayerSkill[Goal][2]){
					Goal2=rand()%5+1;
					while(Goal2==Goal||!Alive[Goal2])Goal2=rand()%5+1;
					cout<<Goal<<"��������"<<Goal2<<"��"<<endl;
					Sleep(1000);
					if(rand()%4<=PlayerSkill[Goal][2]){
						cout<<Goal2<<"������"<<endl;
						Alive[Goal2]=0;
					}else cout<<"��"<<Goal2<<"��û��"<<endl;
					Sleep(1000);
				}
			}else Alive[Goal]=0;
		}else{
			cout<<Turn<<"��û��ɱ��"<<Goal<<"��"<<endl;
			Sleep(1000); 
		}
		AliveNumber=0;
		for(int i=1;i<6;i++)if(Alive[i]){
			AliveNumber++;
			Liver=i;
		}
		if(!AliveNumber){
			puts("����ͬ���ھ���");
			break;
		}
		if(AliveNumber==1){
			cout<<Liver<<"�Ż�ʤ!"<<endl;
			break;
		}
		if(!Alive[Choice]){
			puts("������!");
			break;
		}
		if(Alive[Turn]&&Chance<=PlayerSkill[Turn][3]){
			cout<<Turn<<"��ʹ���ٴι���Ȩ"<<endl;
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
		puts("���ʤ��!\n��ý��:1000");
		getch();
		return;
	}else{
		long long GotCoin=1000>>AliveNumber;
		Coin+=GotCoin;
		cout<<"��ʧ����!\n��ý��:"<<GotCoin;
		getch();
		return; 
	}
	return;
}
int main(){
	HideCursor();
	SetWindow(48,100);
	Color(0,15);
	puts("������......");
	ifstream in("data.sgr");
	if(!in.is_open()){
		cout<<"�Բ���,����ʱ��������,��ȷ����Ĵ浵����";
		getch();
		in.close();
		return 0;
	}
	getline(in,FilePlayerName);
	system("cls");
	puts(Pic);
	cout<<"ɱ���������½\nɱ����:";
	getline(cin,PlayerName);
	if(PlayerName!=FilePlayerName){
		puts("�뿪ʼ���ɱ������");
		Coin=0;
		for(int i=0;i<5;i++)Skill[i]=0;
		ofstream kout("data.sgr");
		kout<<PlayerName<<endl<<Coin<<' ';
	}else{
		puts("����,��ӭ����");
		in>>Coin;
		for(int i=0;i<5;i++)in>>Skill[i];
	}
	in.close();
	getch();
	while(1){
		system("cls");
		puts(Pic);
		cout<<"��Ľ��:"<<Coin<<endl;
		vector<string>Action={"��ʼ��Ϸ","�����̵�","�˳���Ϸ"};
		switch(Choose(Action)){
			case 0:{
				Gaming();
				break;
			}
			case 1:{
				system("cls"); 
				long long UpgradeCoin[5];
				for(int i=0;i<5;i++)UpgradeCoin[i]=100<<Skill[i];
				cout<<"��Ľ��:"<<Coin<<endl;
				puts("��ļ���:");
				for(int i=0;i<5;i++)cout<<"  "<<SkillName[i]<<":"<<Skill[i]<<"��"<<endl;
				puts("��Ҫ�����ĸ�����?");
				vector<string>Upgrade;
				for(int i=0;i<5;i++)Upgrade.push_back(SkillName[i]+":��Ҫ"+ll2str(UpgradeCoin[i])+"���");
				Upgrade.push_back("�˳�");
				int Choice=Choose(Upgrade);
				if(Choice==5)break;
				if(UpgradeCoin[Choice]<=Coin){
					puts("�����ɹ�!");
					Coin-=UpgradeCoin[Choice];
					Skill[Choice]++;
				}else puts("��Ҳ���!");
				getch();
				break;
			}
			case 2:{
				system("cls"); 
				ofstream kout("data.sgr");
				kout<<PlayerName<<endl<<Coin<<' ';
				for(int i=0;i<5;i++)kout<<Skill[i]<<' ';
				puts("�������߲���");
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
