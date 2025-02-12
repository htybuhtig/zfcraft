#include<bits/stdc++.h>
#include<graphics.h>
#include<easyx.h>
#include<fstream>
#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
#include<thread>
#include<windows.h>
#include<winuser.h>
#include<cmath>
#include"cppgraphics.cpp"
using namespace std;
//zfcraft hty
IMAGE block[15];
IMAGE blockbag[15];
map<string,double> zfini;
int keydown(int c){
	return GetAsyncKeyState(c)&0x8000;
}
struct PEIFANG{
	int a[9];
	int ans,f;
	void setup(int a1,int b,int c,int d,int e,int f1){
		a[0]=a1;a[1]=b;a[2]=c;a[3]=d;ans=e;f=f1;
	}
	void print(){
		printf("peifang %d %d %d %d -> %d x %d\n",a[0],a[1],a[2],a[3],ans,f);
	}
	bool operator== (PEIFANG b){
		return (a[0]==b.a[0])&&(a[1]==b.a[1])&&(a[2]==b.a[2])&&(a[3]==b.a[3]);
	}
};
vector<PEIFANG> peifang;
int readini(){
	ifstream ini("zfcraft.ini");
	if(!ini.is_open()){
		cout<<"readini/ error cannot open file zfcraft.ini\n";
		return 1;
	}
	string line;
	string str = "";
	while(getline(ini,line)){
		//cout<<"readline:"<<line<<endl;
		if(line[0]=='#') continue;
		else if(str==""){
			str = line;
			continue;
		}
		else if(str!=""){
			try{
				zfini[str] = stod(line.c_str());
				cout<<"setini "<<str<<" to "<<line<<endl;
			}
			catch(const char* e){
				cout<<"readini/ error:"<<e<<endl;
			}
			str = "";
		}
	}
	ini.close();
	
	ifstream ws("work.ini");
	if(!ws.is_open()){
		cout<<"readini/ error cannot open file work.ini\n";
		return 1;
	}
	int a,b,c,d,e,f;
	while(ws>>a&&a!=-1){
		ws>>b>>c>>d>>e>>f;
		PEIFANG t;
		t.setup(a,b,c,d,e,f);
		t.print();
		peifang.push_back(t);
	}
	ws.close();
}
int getini(string s){
	cout<<"get zfcraftini "<<s<<endl;
	return zfini[s];
}
int imgcount = -1,imgloadsize = -1,tree_kfb;
int loadimg(){
	imgcount = getini("imgcount");
	imgloadsize = getini("imgloadsize");
	
	cout<<"loadimg/ loading block\n";
	for(int i = 0;i<=imgcount;i++){//!! <=
		string fn = "pic\\b"+to_string(i)+".png";
		loadimage(&block[i],fn.c_str(),imgloadsize,imgloadsize);
		cout<<"loadpic:"<<fn<<endl;
	}
	
	cout<<"loadimg/ loading blockbag\n";
	for(int i = 0;i<=imgcount;i++){//!! <=
		string fn = "pic\\w"+to_string(i)+".jpg";
		loadimage(&blockbag[i],fn.c_str(),imgloadsize,imgloadsize);
		cout<<"loadpic:"<<fn<<endl;
	}
}
const int WORLDSIZE=1000,WORLDHEIGHT=384;
struct BLOCK{
	int id;
	int zt;
	int dd;//仅用于背包 堆叠num
	void clear(){
		dd=0;
		zt=0;
		id=0;
	}
	void set(int id_c){
		dd=0;
		zt=0;
		id=id_c;
	}
	void checkbug(){
		if(dd==0){
			clear();
		}
		if(id==0){
			clear();
		}
	}
};
BLOCK world[WORLDSIZE+170][WORLDHEIGHT+3];
BLOCK bag[47];
vector<int> genheight(vector<int> jian={-1}){
	int size = WORLDSIZE;
	vector<int> v;
	int rang = getini("gentimes"),qm = getini("rand_qm"),
	ht = getini("lower_ht"),pj = getini("pingjun_num");
	for(int i = 0;i<size+150;i++) v.push_back(rand()%qm+ht);
	for(int i = 0;i<rang;i++){
		for(int j = 0;j<size;j++){
			int cnt = 0;
			for(int k = 0;k<pj;k++){
				cnt+=v[i+k];
			}
			v[i] = cnt/pj;
		}
	}
	if(jian[0] = -1){
		return v;
	}
	for(int i = 0;i<size;i++){
		v[i] = jian[i]-v[i];
	}
	return v;
}
int saveworld(string s){//ret is cntcheck
	cout<<"saveworld/ running\n";
	string fn = "save\\"+s+"\\world.mcw";
	ofstream ofs(fn.c_str(),ios::binary);
	/*
	--BIN--
	*/
	//ofs<<WORLDSIZE<<" "<<WORLDHEIGHT<<" "<<time(0)<<endl;
	int cnt = 0;
	for(int i = 0;i<WORLDSIZE;i++){
		for(int j = 0;j<WORLDHEIGHT;j++){
			ofs<<(char)world[i][j].id<<(char)world[i][j].zt;
			cnt += world[i][j].id + world[i][j].zt;
			cnt%=65536;
		}
	}
	return cnt;
}
int savechk(string s,int a){
	cout<<"savechk/ running\n";
	string fn = "save\\"+s+"\\chk.txt";
	ofstream ofs(fn.c_str());
	ofs<<WORLDSIZE<<" "<<WORLDHEIGHT<<" "<<time(0)<<endl;
	ofs<<"chk1\n"<<a<<endl;//chk 校验和
	return 0;
}
int saveall(){
	string s;
	cout<<"请输入存档名\n";
	cin>>s;
	string c = "md save\\"+s;
	system(c.c_str());
	int chk1 = saveworld(s);
	savechk(s,chk1);
}
int swan_kw(int x,int y,int id,int tk){
	if(x<=10||x>=990||y<5) return 1;
	int cnt = 0;
	for(int i = -2;i<=2;i++){
		for(int j = -2;j<=2;j++){
			if(cnt<tk){
				if(rand()%(3+cnt)==0){
					world[x+i][y+j].id=id;
				}
			}
			else{
				return 0;
			}
		}
	}
}
int swan_tree(int x,int y){
	world[x][y].set(4);
	world[x][y+1].set(4);
	world[x][y+2].set(4);
	world[x][y+3].set(4);
	world[x][y+4].set(4);
	world[x][y+5].set(4);
	world[x-1][y+4].set(6);
	world[x+1][y+4].set(6);
	world[x-2][y+4].set(6);
	world[x+2][y+4].set(6);
	world[x-1][y+5].set(6);
	world[x+1][y+5].set(6);
	world[x][y+6].set(6);
	cout<<"swan tree on "<<x<<","<<y<<endl;
}
int genmap(){
	vector<int> dirt = genheight();
	cout<<"genheight/ prt ht dirt\n";
	for(int i = 0;i<WORLDSIZE;i++){
		cout<<dirt[i]<<" ";
	}
	cout<<endl;
	int iron_kfb = getini("iron_kfb"),
	iron_h = getini("iron_h"),
	iron_l = getini("iron_l"),
	iron_tk_m = getini("iron_tk_m"),
	tree_kfb = getini("tree_kfb");
	int t = rand()%1000;
	for(int i = 0;i<WORLDSIZE;i++){
		world[i][dirt[i]].id = 1;
		world[i][dirt[i]-1].id = 1;
		world[i][dirt[i]-2].id = 1;
		if(t<=tree_kfb){
			swan_tree(i,dirt[i]+1);
		}
		for(int j = 0;j<dirt[i]-2;j++){
			world[i][j].id = 2;
			t = rand()%1000;
			if(t<=iron_kfb&&j<=iron_h&&j>=iron_l){
				swan_kw(i,j,3,iron_tk_m);
			}
		}
	}
}
int window_x,window_y;
IMAGE player;
int time_of_tick,arm_len;
int start1(){
	cout<<"zfcraft by hty\n";
	readini();
	window_x = getini("window_x");
	window_y = getini("window_y");
	time_of_tick = getini("time_of_tick");
	arm_len = getini("arm_len");
	cout<<"window is "<<window_x<<" "<<window_y<<endl;
	initgraph(window_x,window_y,SHOWCONSOLE);
	//setbkmode(TRANSPARENT);
	IMAGE img;
    loadimage(&img,"lib\\start.png");
    loadimage(&player,"lib\\player.png");
    putimage1(0,0,&img);
	srand(time(0));
	initcppg();
	loadimg();
	genmap();
	//saveall();
	//Sleep(1000);
	cls();
}
int play_x=500,play_y=135;
queue<string> keyq;
int drawworld(){
	//cout<<"draw world\n";
	for(int i = -12;i<12;i++){
		for(int j = -9;j<9;j++){
			int sx = (i+12)*imgloadsize,ey = window_y-(j+9)*imgloadsize;
			int ex = sx+imgloadsize,sy = ey-imgloadsize;
			putimage1(sx,sy,&block[world[play_x+i][play_y+j].id]);
			//cout<<world[play_x+i][play_y+j].id<<" ";
			if(i==0 && j==1){
				transparentimage(sx,sy,player);
			}
		}
		//cout<<endl;
	}
}
int keyqrun(){//keyq:按键队列
	while(!keyq.empty()){
		string s = keyq.front();
		keyq.pop();
		if(s=="x-") play_x--;
		if(s=="y-") play_y--;
		if(s=="x+") play_x++;
		if(s=="y+") play_y++;
	}
}
int mspt,_mspt_t;
int showpos(){
	settextstyle(24,12,"Cascadia Code");
	string s = "x:"+to_string(play_x)+" y:"+to_string(play_y)+"    ";
	outtextxy(0,0,s.c_str());
	string s1 = "MSPT:"+to_string(mspt)+"   ";
	outtextxy(180,0,s1.c_str());
}
int drawwpl(){
	settextstyle(24,12,"Cascadia Code");
	setbkmode(TRANSPARENT);
	setfillcolor(BLACK);
	setlinecolor(WHITE);
	for(int i = 0;i<9;i++){
		fillrectangle(i*64,window_y-64,i*64+64,window_y);
		if(bag[i].id==0) continue;
		transparentimage(i*64+16,window_y-48,blockbag[bag[i].id]);
		if(bag[i].dd==1) continue;
		string s = to_string(bag[i].dd);
		outtextxy(i*64+34,window_y-25,s.c_str());
	}
	setbkmode(OPAQUE);
}
int duidie(int id){
	return 64;//to do 堆叠数判断
}
int dig_block(int x,int y){
	int wid = world[x][y].id;
	if(wid==0) return 0;
	for(int i = 0;i<36;i++){
		if(bag[i].id==wid&&duidie(wid)>bag[i].dd){
			bag[i].dd++;
			cout<<"digblock add block "<<wid<<" to "<<
			i<<endl;
			world[x][y].clear();
			return 0;
		}
		else if(bag[i].id==0){
			bag[i].id=wid;
			bag[i].dd=1;
			cout<<"digblock add block "<<wid<<" to "<<
			i<<endl;
			world[x][y].clear();
			return 0;
		}
	}
}
int mouse_ld(int x,int y){//mouse left down
	int sy = window_y-32*18;
	if(y<window_y-32 && y>sy && x<32*24){
		int dy = (y-sy)/32;
		int dx = x/32;
		int rx = play_x-12+dx;
		int ry = play_y+8-dy;
		float len = sqrt(pow(abs(dx-12),2)+pow(abs(dy-8),2));
		cout<<"mouse_ld dx:"<<dx<<" dy:"<<dy
		<<" rx:"<<rx<<" ry:"<<ry<<" len:"<<len<<endl;
		if(len<=arm_len){
			dig_block(rx,ry);
		}
	}
}
int mouserun(){
	MOUSEMSG msg;
	if(PeekMouseMsg(&msg,0)==0) return 0;
	msg = GetMouseMsg();
	if(msg.uMsg == WM_LBUTTONDOWN){
		//cout<<"msg: WM_LBUTTONDOWN\n";
		mouse_ld(msg.x,msg.y);
	}
	else if(msg.uMsg == WM_RBUTTONDOWN){
		cout<<"msg: WM_RBUTTONDOWN\n";
		//to do 放置
	}
	else if(msg.uMsg == WM_MBUTTONDOWN){
		cout<<"msg: WM_MBUTTONDOWN\n";
	}
	else if(msg.uMsg == WM_MOUSEWHEEL){
		cout<<"msg: WM_MOUSEWHEEL wheel:"<<msg.wheel<<endl;//up or down
		//+-120
	}
}
int drawbag(){
	//line 3 - no.9-17
	settextstyle(24,12,"Cascadia Code");
	setbkmode(TRANSPARENT);
	setfillcolor(BLACK);
	setlinecolor(WHITE);
	for(int k = 1;k<=3;k++){
		int l = k;
		for(int i = 9*l;i<9+9*l;i++){
			fillrectangle((i-9*l)*64,window_y-64-64*l,(i-9*l)*64+64,window_y-64*l);
			if(bag[i].id==0) continue;
			transparentimage((i-9*l)*64+16,window_y-48-64*l,blockbag[bag[i].id]);
			if(bag[i].dd==1) continue;
			string s = to_string(bag[i].dd);
			outtextxy((i-9*l)*64+34,window_y-25-64*l,s.c_str());
		}
	}
	/*
	1 2
	3 4  5
	*/
	//4 block gz
	fillrectangle(200,100,264,164);//zuoshang
	fillrectangle(264,100,328,164);//youshang
	fillrectangle(200,164,264,228);//zuoxia
	fillrectangle(264,164,328,228);//youxia
	fillrectangle(400,132,464,196);//->[ok]
	line(332,164,396,164);
	line(376,144,396,164);
	line(376,184,396,164);
	
	transparentimage(200+16,100+16,blockbag[bag[36].id]);
	if(bag[36].dd>1){
		string s = to_string(bag[36].dd);
		outtextxy(200+34,100+39,s.c_str());
	}
	
	transparentimage(264+16,100+16,blockbag[bag[37].id]);
	if(bag[37].dd>1){
		string s = to_string(bag[37].dd);
		outtextxy(264+34,100+39,s.c_str());
	}
	
	transparentimage(200+16,164+16,blockbag[bag[38].id]);
	if(bag[38].dd>1){
		string s = to_string(bag[38].dd);
		outtextxy(200+34,164+39,s.c_str());
	}

	transparentimage(264+16,164+16,blockbag[bag[39].id]);
	if(bag[39].dd>1){
		string s = to_string(bag[39].dd);
		outtextxy(264+34,164+39,s.c_str());
	}
	
	transparentimage(400+16,132+16,blockbag[bag[40].id]);
	if(bag[40].dd>1){
		string s = to_string(bag[40].dd);
		outtextxy(400+34,132+39,s.c_str());
	}
	
	setbkmode(OPAQUE);
}
int getxuanbag(int x,int y1){
	if(x>=200&&x<264&&y1>=100&&y1<164) return 36;
	if(x>=264&&x<328&&y1>=100&&y1<164) return 37;
	if(x>=200&&x<264&&y1>=164&&y1<228) return 38;
	if(x>=264&&x<328&&y1>=164&&y1<228) return 39;
	if(x>=400&&x<464&&y1>=132&&y1<196) return 40;
	int y = window_y-y1;
	/*
	(200,100,264,164);36
	(264,100,328,164);37
	(200,164,264,228);38
	(264,164,328,228);39
	(400,132,464,196);40
	*/
	if(x>=9*64 || y>=4*64) return 31;
	int dx = x/64;
	int dy = y/64;
	int p = dy*9+dx;
	printf("getxuanbag/ x%3d y%3d dx%2d dy%2d p%2d\n",x,y,dx,dy,p);
	return p;
}
BLOCK _bagxuan;//选择物品
int _bagid;//选择的格子号
int tryhecheng(MOUSEMSG msg){
	PEIFANG t;
	t.a[0] = bag[36].id;
	t.a[1] = bag[37].id;
	t.a[2] = bag[38].id;
	t.a[3] = bag[39].id;
	for(PEIFANG i : peifang){
		if(i==t){
			int canhct1 = (i.a[0]==0?64:bag[36].dd);
			int canhct2 = (i.a[1]==0?64:bag[37].dd);
			int canhct3 = (i.a[2]==0?64:bag[38].dd);
			int canhct4 = (i.a[3]==0?64:bag[39].dd);
			int canhct = min(canhct1,min(canhct2,min(canhct3,canhct4)));
			int canhc = canhct*i.f;
			BLOCK bag40;
			bag40.id=i.ans;
			bag40.dd=canhc;
			bag[40] = bag40;
			if(getxuanbag(msg.x,msg.y)==40&&msg.uMsg==WM_LBUTTONDOWN){
				bool f = 0;
				for(int j = 0;j<36;j++){
					bag[j].checkbug();
					if(bag[j].id==0||(bag[j].id==bag[40].id&&bag[j].dd+bag[40].dd<=duidie(bag[40].id))){
						bag[j].dd+=bag[40].dd;
						bag[j].id=bag[40].id;
						f = 1;
						break;
					}
				}
				if(f==1){
					bag[40].clear();
					bag[36].dd-=canhct;
					bag[37].dd-=canhct;
					bag[38].dd-=canhct;
					bag[39].dd-=canhct;
				}
				
				bag[36].checkbug();
				bag[37].checkbug();
				bag[38].checkbug();
				bag[39].checkbug();
				bag[40].checkbug();
			}
			return 0;
		}
	}
	return 1;
}
int inbagclick_run(){
	MOUSEMSG msg;
	if(PeekMouseMsg(&msg,0)==0) return 0;
	msg = GetMouseMsg();
	if(msg.uMsg == WM_LBUTTONDOWN){
		int t = getxuanbag(msg.x,msg.y);
		if(_bagxuan.id==0){
			cout<<"_bagxuan is blank\n";
			_bagid = t;
			_bagxuan = bag[t];
		}
		else if(bag[t].id==0||(bag[t].id==_bagxuan.id && bag[t].dd+_bagxuan.dd<=duidie(_bagxuan.id))){
			cout<<"_bagid is "<<_bagid<<endl;
			bag[_bagid].clear();
			int tm = bag[t].dd;
			bag[t] = _bagxuan;
			bag[t].dd+=tm;
			_bagxuan.clear();
			_bagid = 31;
		}
		//cout<<"inbagclick_run/ click "<<t<<" x"<<msg.x<<" y"<<msg.y<<endl;
	}
	else if(msg.uMsg == WM_RBUTTONDOWN){
		int t = getxuanbag(msg.x,msg.y);
		if(_bagxuan.id==0){
			cout<<"_bagxuan is blank\n";
			_bagid = t;
			_bagxuan = bag[t];
		}
		else if(_bagxuan.dd==0){
			cout<<"_bagxuan.dd is 0\n";
			_bagxuan.clear();
			_bagid = 31;
		}
		else if(bag[t].id==0||(bag[t].id==_bagxuan.id && bag[t].dd+_bagxuan.dd<=duidie(_bagxuan.id))){
			cout<<"_bagid is "<<_bagid<<endl;
			int l = _bagxuan.dd/2.0;
			if(l==0){
				cout<<"l is 0\n";
			}
			else{
				bag[_bagid].dd -= l;
				int tm = bag[t].dd;
				bag[t] = _bagxuan;
				bag[t].dd = l+tm;
				printf("t:%d l:%d tm:%d\n",t,l,tm);
				_bagxuan.clear();
				_bagid = 31;
			}
		}
	}
	if(_bagxuan.dd==0){
		_bagxuan.clear();
		_bagid = 31;
	}
	tryhecheng(msg);
}
int boardjtrun(){
	if(keydown('E')){
		_bagxuan.clear();_bagid=31;
		while(keydown('E')) Sleep(10);
		while(!(keydown(VK_ESCAPE)||keydown('E'))){
			drawwpl();
			drawbag();
			inbagclick_run();
		}
		while(keydown('E')) Sleep(10);
		_bagxuan.clear();_bagid=31;
	}
}
int cntmspt(int a){
	if(a==1){
		_mspt_t = GetTickCount();
	}
	else if(a==2){
		mspt = GetTickCount()-_mspt_t;
	}
	else{
		if(mspt<time_of_tick){
			while(GetTickCount()<(_mspt_t+time_of_tick)){
				mouserun();//mouse run
				boardjtrun();//长时间监听任务
			}
		}
		//Sleep(time_of_tick-mspt);
	}
}
int main(){
	start1();
	while(1){
		cntmspt(1);
		if(keydown('A')){
			keyq.push("x-");
		}
		if(keydown('D')){
			keyq.push("x+");
		}
		if(keydown('W')){
			keyq.push("y+");
		}
		if(keydown('S')){
			keyq.push("y-");
		}
		keyqrun();
		drawworld();
		drawwpl();//绘制物品栏
		showpos();
		cntmspt(2);
		cntmspt(3);
	}
	return 0;
}
