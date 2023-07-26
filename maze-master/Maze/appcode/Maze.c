#include "graphics.h"
#include "extgraph.h"
#include "imgui.h"
#include "linkedlist.h"

#include <windows.h>
#include <winuser.h>

#include <time.h>
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>

#define true 1
#define false 0

#define MazeSize 40
#define CreateStartX 1
#define CreateStartY 13
#define MAXINT 2147483647

//方块类型 
#define BOUND 0
#define UNCHEACK 1
#define BARRIER 2
#define CHEACED 3
#define MYBOlOCK 5
#define STARTPOINT 6
#define TERMINAL 7 
#define GAMER 8
#define MACHINE 9


//计时器控件位置 
#define TimeShowX 1.0
#define TimeShowY 16.5
#define TimeShowH 1.0
#define TimeShowW 1.175

//种子控件位置 
#define SeedShowX 1.75
#define SeedShowY 0.25
#define SeedShowH 0.5
#define SeedShowW 1.75

//地图坐标 
typedef struct _sit_ {
	int x,y;		//在迷宫中的位置 

}*NODE,Node;

typedef struct pathnode {
	int dir;
	int isFind;
}*PATHNODE,pathNode; 

//标签结构体 
typedef struct _label_ {
	double x,y;
	char Txt[255]; 
}*LABEL,Label;

//计时器 
typedef enum {
	GameTimer,
	MouseTimer,
	MachineTimer,
	AllTimer,
	StepTimer,
} MyTimer;

//自动寻路方向 
typedef enum {
	dir_up,
	dir_down,
	dir_left,
	dir_right,
}DIR; 

//栈 
typedef struct stack {
	Node list[100];
	int top;
	int beg;
}*STACK,Stack; 

//地图参数
static char MapName[255] = "Map"; 
static char fileSuffix[255] = ".txt";
static char MapPath[255] = "../SavedMap/"; 
static char FileMapLoad[255] = "输入文件名"; 
static char SavedTip[255] = "";
static char saveName[255];
static char openName[255];
static int MapList = 1;

//标签参数
//int labelNums = 0;
static double SaveLabelX = 7.0;
static double SaveLabelY = 16.5;

//游戏流程参数
static int startingGame = 1; 
static int choicingBlocks = 0;

//方块参数
static double BlockWidth = 1.0;
static double BlockHight = 1.0; 


//按钮参数
static double buttonWidth = 1.6;
static double buttonHight = 0.75; 

//起终点位置
static int StartX = 1; 
static int StartY = 13;
static int FainalX = 13;
static int FainalY = 1;

//鼠标参数 
static int enable_mousing = 0;
static double mouse_x = 0;
static double mouse_y = 0;
static int mouseSet = 0;
static int mouseSetList[9] = {BARRIER,CHEACED,STARTPOINT,TERMINAL};
static char mouseColorList[9][256] = {{"Black"},{"White"},{"Bule"},{"Bule"},}; 

//游戏元素参数 
static int    enable_Gaming = 1;			//运行游戏开始 
static int    enable_indicating	= 1;		//游戏提示提示 
static int    show_more_buttons = 0;		//显示更多按钮 
static int 	  show_file_button = 0;			//显示文件系统按钮
static int    machine_step = 1;				//程序求解步数
static int    machine_all_step = 1;			//程序所有解步数 
static int    machine_onestep = 1;			//单步求解 
static int    machine_all_steping = 0;		//全部路径展示 
static int 	  enable_machine = 0;			//程序求解 
static int    machine_moving = 0;	 		//求最短路径 
static int    machine_steping = 0;			//程序|单步求解 
static int 	  show_load_label = 2; 			//显示文件打开状态 
static int 	  randmoSeed = 0;				//随机种子值 
static Node Mysit;							//玩家位置 
static Node begPos,endPos;					//起终点 
static int isFindEnd = 0;					//找到终点标志 
char * c[20] = {"Black","Dark Gray","Gray","Light Gray","White", "Cyan",
			"Brown", "Red", "Orange", "Yellow", "Green","Blue","Violet", "Magenta"}; //颜色 
 
int MazeWith = 14;		//迷宫宽度 
int MazeHight = 14;		//迷宫高度 
int SeedLists = 0;		//种子序列号
 
//int	startTime = 0;		

double winHeight = 17.5;		//画布宽度 
double winWidth = 17.0; 		//画布高度 

static char Myseed[10] = "随机种子值"; 
static char TimeTips[100] = "剩余时间:"; 
static char TimerStr[100] = "60";
static char LabelSeed[80] = "1";
static char SeedSet[80] = "1";
static char results[256] = "";
 
 //迷宫数组 
int maze[MazeSize][MazeSize] = {0,};
pathNode pathMaze[MazeSize][MazeSize] = {0,};
pathNode dirMap[MazeSize][MazeSize] = {0,};
STACK pathStack;
STACK allpath;

void initDisplay();		//初始化画布 
void reInitDisplay();	//重新初始化画布 
void redisplay();		//重新描绘画布 
STACK initPath();		//初始化路劲参数 
void display();			//画布 
void drawMaze();		//画迷宫 
bool cheackWin();		//检查游戏是否胜利 
void gameWin();			//游戏胜利函数
void gameLose();		//游戏失败 
void saveMap();			//保存地图
void loadMap();			//加载地图  
void aboutGame();		//关于游戏 
void setMyColor();		//设置颜色 
void introductionGame();//游戏说明
void machining(STACK,int,int);	//程序求解
int  isEmpty(STACK);			//判断栈空 
void popStack(STACK s); 		//出栈操作 
Node getTop(STACK s);   		//获取栈顶元素
int getDir(Node now,Node to);	//判断两点之间的走向 
void pushStack(STACK,int,int);				//入栈 
void drawMyMaze(int,int);					//画自己的迷宫 
int determine(int,int);						//判断是否是终点或者起点 
void setSeed(int seed);						//设置随机种子 
void drawSeedControler();					//画出随机种子控件 
void drawTimer();							//画出计时器控件
void drawMenu();							//画出菜单控件 
void drawMouse();							//画出鼠标控件 
void drawButton();							//画出按钮控件 
void drawLabels();							//画出标签组件 
void drawBolck(int,int,int);				//设置一个画块及其类型 
void drawPath(STACK);						//画出一个路径栈 
void setLabel(double,double,char*);			//传入一个Label的参数 
int cheackWay(int index_X,int index_Y);				//创建迷宫的过程中检查路径是否可行 
void createRandomMaze_01(int index_X,int index_Y);	//创建随机迷宫方式一 
void KeyboardEventProcess(int key, int event);				//键盘控件函数 
void CharEventProcess(char ch); 							//GUI字符输入 
void TimerProcess(int timerID);								//计时器函数
void MouseEventProcess(int x, int y, int button, int event);//鼠标控件函数 


void Main()
{		
	
	//控制板 
//	InitConsole(); 
	
	initDisplay(); 
	SetWindowTitle("迷宫");
	InitGraphics();
	
	
	registerCharEvent(CharEventProcess);        	// 字符
	registerTimerEvent(TimerProcess);				//定时器 
	registerKeyboardEvent(KeyboardEventProcess);	//键盘 
	registerMouseEvent(MouseEventProcess);			//鼠标 
	
	
//	startTimer(MouseTimer,10);
	startTimer(GameTimer,1000);
	startTimer(MachineTimer,25);
	startTimer(StepTimer,100);
	startTimer(AllTimer,100);
	
	
	display();
}

//初始化画布 
void initDisplay() {
	
	
	char buffer[100]; 
	int i = 0,j = 0,buf = 0; 
	//初始化画布宽高 
	SetWindowSize(winWidth,winHeight);
	
	//初始化种子参数 
	setSeed(randmoSeed);	
	
	//初始化迷宫 
	int tempH = MazeHight - 1;
	int tempW = MazeWith - 1;
	for( i = 1; i <= tempH; ++i) {
		for( j = 1; j <= tempW; ++j) {
			if(i%2 == 1 && j%2 == 1) {
				maze[i][j] = 1;
			}
			else {
				maze[i][j] = 2;
			}
		}
	}
	
	//随机化迷宫 
	createRandomMaze_01(CreateStartX,CreateStartY);
	
	//初始化我的位置 
	Mysit.x = 1;
	Mysit.y = 13; 
	//初始化起点位置
	StartX = 1;
	StartY = 13;
	//初始化终点位置
	FainalX = 13;
	FainalY = 1;

	maze[StartX][StartY] = STARTPOINT;
	maze[Mysit.x][Mysit.y] = GAMER;
	maze[FainalX][FainalY] = TERMINAL;
	
	//初始化计时器
	itoa(60,TimerStr,10);
	
	//初始化种子显示
	itoa(randmoSeed,SeedSet,10);
	
	//初始化种子序列号 
	FILE* fp = fopen("SeedList.bin","r");
	
	if(fp == NULL) {
		//若不存在则创建对应的种子序列文件 
		fp = fopen("SeedList.bin","w+");
		fwrite("0",1,1,fp);
	} else {
		fgets(buffer,10,fp);
		buffer[10] = '\0';
		SeedLists = atoi(buffer) % MAXINT ;
	}
	
	free(pathStack);
	pathStack = NULL;
	free(allpath);
	allpath = NULL;
	isFindEnd = 0;
	//初始化路径地图 
	pathStack = initPath();
	allpath = initPath();
	//程序求解路径 
	machining(pathStack,StartX,StartY);
	
	fclose(fp);
	free(fp);
	
	
}

//重新初始化画布 
void reInitDisplay() {
	
	int i = 0,j = 0,buf = 0; 
	//初始化画布宽高 
	SetWindowSize(winWidth,winHeight);
	
	//重新初始化迷宫 
	int tempH = MazeHight - 1;
	int tempW = MazeWith - 1;
	for( i = 1; i <= tempH; ++i) {
		for( j = 1; j <= tempW; ++j) {
				maze[i][j] = CHEACED;
		}
	}
	//初始化计时器
	itoa(60,TimerStr,10);
	
	//初始化种子显示
	itoa(randmoSeed,SeedSet,10);
	
	//隐藏所有非必要控件
	startingGame = 0;		//暂停游戏按钮
	
	maze[StartX][StartY] = STARTPOINT;
	maze[FainalX][FainalY] = TERMINAL;
	
}
void redisplay() {
	int i = 0,j = 0;
	//初始化计时器
	itoa(60,TimerStr,10);
	
	//初始化种子显示
	itoa(randmoSeed,SeedSet,10);
	
	 
	//初始化地图 
	for( i = 1; i < MazeWith; ++i) {
		for(j = 1; j < MazeHight; ++j) {
			maze[i][j] = ( maze[i][j] == BARRIER) ? BARRIER : CHEACED;
		}
	}
	
	//初始化位置 
	maze[Mysit.x ][Mysit.y ] = CHEACED;
	Mysit.x = StartX;
	Mysit.y = StartY;
	maze[StartX][StartY] = STARTPOINT;
	maze[FainalX][FainalY] = TERMINAL;
	
	isFindEnd = 0;
	free(pathStack);
	pathStack = NULL;
	free(allpath);
	allpath = NULL;
	//初始化路径地图 
	pathStack = initPath();
	allpath = initPath();
	//程序求解路径 
	machining(pathStack,StartX,StartY);
	
	
	display();
}
//显示函数 
void display() {
	//刷新画布 
	DisplayClear();
	//画出迷宫 
	drawMaze();
	//画出随机种子控件 
	drawSeedControler();
	//画出计时器控件 
	drawTimer(); 
	//画出菜单控件 
	drawMenu();
	//画出鼠标控件 
	drawMouse();
	//画出按钮控件 
	drawButton();
	//画出标签组件 
	drawLabels();
	return;
}

//画出迷宫 
void drawMaze(){
	int i = 0,j = 0;
	int tempx = 0,tempy = 0;
	for( i = 0; i <= MazeHight; ++i) {
		for( j = 0; j <= MazeWith; ++j) {
			tempx = i + 1;
			tempy = j + 1;
			if(maze[i][j] == UNCHEACK){
				SetPenColor("Yellow");
				drawRectangle(tempx,tempy,BlockWidth,BlockHight,1); 
			} 
			else if(maze[i][j] == BARRIER){
				SetPenColor("Black");
				drawRectangle(tempx,tempy,BlockWidth,BlockHight,1); 
			}
			else if(maze[i][j] == GAMER) {
				if(dirMap[i][j].isFind && enable_indicating) {
					switch(dirMap[i][j].dir) {
						case dir_up:
							SetPenColor(c[3]);
							drawRectangle(tempx,tempy + 1.0,BlockWidth,BlockHight,1); 
							maze[i][j+1] = -1;
							break;
						case dir_down:
							SetPenColor(c[3]);
							drawRectangle(tempx,tempy - 1.0,BlockWidth,BlockHight,1); 
							break;
						case dir_left:
							SetPenColor(c[3]);
							drawRectangle(tempx - 1.0,tempy,BlockWidth,BlockHight,1); 
							break;
						case dir_right:
							SetPenColor(c[3]);
							drawRectangle(tempx + 1.0,tempy,BlockWidth,BlockHight,1); 
							maze[i+1][j]=-1;
							break;
						default:
							break;
					}
				}
				SetPenColor("Red");
				drawRectangle(tempx,tempy,BlockWidth,BlockHight,1); 
			}
			else if(maze[i][j] == CHEACED) {
				SetPenColor("White");
				drawRectangle(tempx,tempy,BlockWidth,BlockHight,1); 
			}
			else if(maze[i][j] == BOUND) {
				SetPenColor("Green");
				drawRectangle(tempx,tempy,BlockWidth,BlockHight,1); 
			}
			else if(maze[i][j] == MACHINE) {
				SetPenColor("Yellow"); 
				drawRectangle(tempx,tempy,BlockWidth,BlockHight,1); 
			}
			else if(maze[i][j] == TERMINAL ) {
				SetPenColor("Blue");
				drawBox(tempx,tempy,BlockWidth,BlockHight,1,"TPoint",'L',"White");
			}
			else if(maze[i][j] == STARTPOINT) {
				SetPenColor("Blue");
				drawBox(tempx,tempy,BlockWidth,BlockHight,1,"SPoint",'L',"White");
			}
			else if(maze[i][j] == -1) {
				maze[i][j] = (i == FainalX && j == FainalY) ? TERMINAL:CHEACED; 
			}
//			drawBox(i+1,j+1,1.0,1.0,0,"Test",'L',"Green");
		}
	}
	
	return;
}

//设置随机种子 
void setSeed(int seed) {
	randmoSeed = seed % MAXINT; 
	srand( randmoSeed );
	return; 
}

//获取创建路径 
int cheackWay(int index_X,int index_Y) {
	
	int ways = 0;
//	return ways; 
	//检查上侧是否可通 
	if(  (index_X - 2) > 0 &&
		 //(index_X <= MazeWith) &&
		 //(index_Y > 0 && index_Y <= MazeHight) &&
		 (maze[index_X - 2][index_Y] == 1) 
	) {
		++ways;
	}
	//检查下侧是否可通 
   if(  (index_X + 2) <= MazeWith &&
//		 (index_X  > 0) &&
//		 (index_Y > 0 && index_Y <= MazeHight) &&
		 (maze[index_X + 2][index_Y] == 1) 
	) {
		++ways;
	}
	
	//检查右侧是否可通 
	if(  ((index_Y + 2) <=  MazeWith )&&
		 //(index_Y  > 0) &&
		 //(index_X > 0 && index_X <=  MazeHight) &&
		 (maze[index_X][index_Y + 2] == 1) 
	) {
		++ways;
	}
	
	
	//检查左侧是否可通 
	if(  (index_Y - 2) > 0 &&
		 //(index_Y  <= MazeHight) &&
		 //(index_X > 0 && index_X <= MazeWith) &&
		 (maze[index_X][index_Y - 2] == 1) 
	) {
		++ways;
	}
	
	return ways;
}

//创建随机迷宫 
void createRandomMaze_01(int index_X,int index_Y) {
	if(maze[index_X][index_Y] == 1) {
		maze[index_X][index_Y] = 3;
	}
	
	//可用路劲数量 
	int random_index_Ways = 0;
	//选择的路径 
	int random_way = 0;
	//存储位置 
	int tempX = index_X,tempY = index_Y;
	while(1) {
		random_index_Ways =  cheackWay(index_X,index_Y);
		if(random_index_Ways == 0) {
			break;
		} 
		else {
			random_way = rand()%4;
			switch(random_way) {
				//向上 
				case 0:
					if(index_Y + 2 <= MazeHight && maze[index_X][index_Y+2] == 1){
						maze[index_X][index_Y+1] = 3;
						maze[index_X][index_Y+2] = 3;
						index_Y = index_Y + 2;
					}
					break;
				//向下 
				case 1:
					if(index_Y-2 > 0 && maze[index_X][index_Y-2] == 1){
						maze[index_X][index_Y-1] = 3;	
						maze[index_X][index_Y-2] = 3;
						index_Y = index_Y - 2;	
					}
					break;
				//向左 
				case 2:
					if(index_X+2 <= MazeWith &&  maze[index_X+2][index_Y] == 1){
						maze[index_X+1][index_Y] = 3;
						maze[index_X+2][index_Y] = 3;
						index_X = index_X + 2;
					}
					break;
				//向右 
				case 3:
					if(index_X-2 > 0 && maze[index_X-2][index_Y] == 1){
						maze[index_X-1][index_Y] = 3;
						maze[index_X-2][index_Y] = 3;
						index_X = index_X - 2;	
					}
					break;
				default:
					break;
			}
			createRandomMaze_01(index_X,index_Y);
		}
	}
	return;
}

//画出随机种子控件 
void drawSeedControler() {
	SetPenColor("Black");
	drawLabel(SeedShowX - 1.5, SeedShowY+0.15, Myseed);
	if(textbox(GenUIID(0), SeedShowX, SeedShowY, SeedShowW, SeedShowH, SeedSet, sizeof(SeedSet)))
		sprintf(LabelSeed,"%s", SeedSet);
	return;
}

//键盘驱动程序 
void KeyboardEventProcess(int key, int event)
{	
	if(enable_machine)
		return;
	if(!enable_Gaming)
		return;
	int toPoint = 0;
	int nowPoint = 0; 
	
	uiGetKeyboard(key, event); // needed for using simpleGUI
	display();

	switch (event)
	{
	case KEY_DOWN:
		if (key == VK_LEFT){
			if(maze[Mysit.x - 1][Mysit.y] == BARRIER || maze[Mysit.x - 1][Mysit.y] == BOUND) {
				break;
			}
			else {
				toPoint = determine(Mysit.x - 1,Mysit.y);
				nowPoint = determine(Mysit.x,Mysit.y);
				if(toPoint == STARTPOINT || toPoint == TERMINAL) {
					maze[Mysit.x][Mysit.y] = CHEACED;
					maze[--Mysit.x][Mysit.y] = GAMER;
					break;
				}
				else if(nowPoint == STARTPOINT || nowPoint == TERMINAL) {
					maze[Mysit.x][Mysit.y] = nowPoint;
					maze[--Mysit.x][Mysit.y] = GAMER;
					break;
				}
				maze[Mysit.x][Mysit.y] = maze[--Mysit.x][Mysit.y];
				maze[Mysit.x][Mysit.y] = GAMER;
			}
		}
		else if(key == VK_RIGHT){
			if(maze[Mysit.x + 1][Mysit.y] == BARRIER || maze[Mysit.x + 1][Mysit.y] == BOUND) {
				break;
			}
			else {
				toPoint = determine(Mysit.x + 1,Mysit.y);
				nowPoint = determine(Mysit.x,Mysit.y);
				if(toPoint == STARTPOINT || toPoint == TERMINAL) {
					maze[Mysit.x][Mysit.y] = CHEACED;
					maze[++Mysit.x][Mysit.y] = GAMER;
					break;
				}
				else if(nowPoint == STARTPOINT || nowPoint == TERMINAL) {
					maze[Mysit.x][Mysit.y] = nowPoint;
					maze[++Mysit.x][Mysit.y] = GAMER;
					break;
				}
				maze[Mysit.x][Mysit.y] = maze[++Mysit.x][Mysit.y];
				maze[Mysit.x][Mysit.y] = GAMER;
			}
		}
		else if(key == VK_UP){
			if(maze[Mysit.x][Mysit.y + 1] == BARRIER || maze[Mysit.x][Mysit.y + 1] == BOUND) {
				break;
			}
			else {
				toPoint = determine(Mysit.x,Mysit.y + 1);
				nowPoint = determine(Mysit.x,Mysit.y);
				if(toPoint == STARTPOINT || toPoint == TERMINAL) {
					maze[Mysit.x][Mysit.y] = CHEACED;
					maze[Mysit.x][++Mysit.y] = GAMER;
					break;
				}
				else if(nowPoint == STARTPOINT || nowPoint == TERMINAL) {
					maze[Mysit.x][Mysit.y] = nowPoint;
					maze[Mysit.x][++Mysit.y] = GAMER;
					break;
				}
				maze[Mysit.x][Mysit.y] = maze[Mysit.x][++Mysit.y];
				maze[Mysit.x][Mysit.y] = GAMER;
			}
		}
		else if(key == VK_DOWN){
			if(maze[Mysit.x][Mysit.y- 1] == BARRIER || maze[Mysit.x][Mysit.y- 1] == BOUND) {
				break;
			}
			else {
				toPoint = determine(Mysit.x,Mysit.y- 1);
				nowPoint = determine(Mysit.x,Mysit.y);
				if(toPoint == STARTPOINT || toPoint == TERMINAL) {
					maze[Mysit.x][Mysit.y] = CHEACED;
					maze[Mysit.x][--Mysit.y] = GAMER;
					break;
				}
				else if(nowPoint == STARTPOINT || nowPoint == TERMINAL) {
					maze[Mysit.x][Mysit.y] = nowPoint;
					maze[Mysit.x][--Mysit.y] = GAMER;
					break;
				}
				maze[Mysit.x][Mysit.y] = maze[Mysit.x][--Mysit.y];
				maze[Mysit.x][Mysit.y] = GAMER;
			}
		}
		break;

	default:
		break;
	}
	display();
	cheackWin();
}

//计时器驱动程序 
void TimerProcess(int timerID) {
	
	int tTime = 0;
	switch(timerID) {
		case GameTimer:
			if(!enable_Gaming)
				break;
			tTime = atoi(TimerStr) - 1;
			if(tTime < 0) {
			itoa(60,TimerStr,10);
			}
			else {
				itoa(tTime,TimerStr,10);
			}
			display();
			break;	
		case MouseTimer:
			display();
			break;
		case MachineTimer:
			if(machine_moving) {
				printf("MachineTimer:%d",pathStack->top);
				if(machine_step == (pathStack->top - 1)) {
					machine_moving = 0;
					display();
					break;
				}
				if(machine_step < pathStack->top){
					
					drawBolck(pathStack->list[machine_step].x,pathStack->list[machine_step].y,MACHINE);
				} 
				machine_step++; 
			}
			display();
			break;
		case AllTimer: 
			if(machine_all_steping) {
				printf("AllTimer:%d",allpath->top);
				if(machine_all_step == (allpath->top - 1)) {
					machine_all_steping = 0;
					machine_all_step = 1;
					break;
				}
				if(machine_all_step < allpath->top){
					printf("go on!\n");
					maze[allpath->list[machine_all_step].x][allpath->list[machine_all_step].y] = MACHINE;
					machine_all_step++;
				} 
			}
			display();
			break;
		case StepTimer:
			if(machine_steping) {
				printf("StepTimer:%d",pathStack->top);
				if(machine_step == (pathStack->top - 1) ){
					machine_steping = 0;
					machine_step = 1;
				}
				if(machine_step < pathStack->top && machine_step < machine_onestep) {
					if((pathStack->list[machine_step-1].x != StartX ||
					   pathStack->list[machine_step-1].y != StartY)) {
						maze[pathStack->list[machine_step - 1].x][pathStack->list[machine_step -1].y] = CHEACED;
					}
					maze[pathStack->list[machine_step].x][pathStack->list[machine_step].y] = MACHINE;
					machine_step++; 
				}
			}
			display();
			break;
		default:
			break;
	}
	
	return;
}

//鼠标驱动程序 
void MouseEventProcess(int x, int y, int button, int event){
	
	uiGetMouse(x, y, button, event); 
	
	mouse_x = ScaleXInches(x);
	mouse_y = ScaleYInches(y);
	
	display();
	
	switch(event)  {
		case BUTTON_DOWN:
			if(button == LEFT_BUTTON && enable_mousing) {
				drawMyMaze(mouse_x,mouse_y);
			}
			break;
		case MOUSEMOVE:
			display(); 
			break;
		default:
			
			break;
	}

}

//画出计时器控件 
void drawTimer() 
{
	SetPenColor("Black");
	drawLabel(TimeShowX + 0.35, TimeShowY, TimerStr);
	SetPenColor("Red");
	drawLabel(TimeShowX - 1.0, TimeShowY + 0.005, TimeTips);
	
	
}

//画出菜单控件 
void drawMenu() {
	static char * menuListFile[] = {"文件",  
		"打开地图  | Ctrl-O", // 快捷键必须采用[Ctrl-X]格式，放在字符串的结尾
		"保存地图  | Ctrl-X",
		"退出游戏  | Ctrl-Q",};
		
	static char * menuListTool[] = {"地图编辑",
		"生成随机地图 | Ctrl-L",
		"重新开始     | Ctrl-R",
		"元素选择 ",};
	
	static char * menuListHelp[] = {"帮助",
		"游戏说明 ",
		"关于游戏 "};
		
		
	static char * menuListFigureMap[] = {"地图求解",
		"手动求解 | Ctrl-T",
		"显示所有可行解 "};
		
	static char * selectedLabel = NULL;

	double fH = GetFontHeight();
	double x = 0; //fH/8;
	double y = winHeight;
	double h = fH*1.5; // 控件高度
	double w = TextStringWidth(menuListHelp[0])*2 + 1.0;		// 控件宽度
	double wlist = TextStringWidth(menuListTool[3])*1.2 + 0.6;	//列表宽度 
	double xindent = winHeight/20; // 缩进
	int    selection;
	
	drawMenuBar(0,y-h,winWidth,h);
	
	//菜单 
	selection = menuList(GenUIID(0), x, y-h, w, w, h, menuListFile, sizeof(menuListFile)/sizeof(menuListFile[0]));
	if( selection > 0 ) selectedLabel = menuListFile[selection];
	switch(selection) {
		case 1:		//打开地图 
			loadMap(); 
			break;
		case 2:		//保存地图 
			saveMap();
			break;
		case 3:		//退出游戏 
			exit(-1); 
			break;
		default:
			break;
	}
		
	selection = menuList(GenUIID(0),x+w,  y-h, w, w,h, menuListTool,sizeof(menuListTool)/sizeof(menuListTool[0]));
	if( selection > 0 ) selectedLabel = menuListTool[selection];
	switch(selection) {
		case 1:		//生成随机种子
			randmoSeed = rand();
			initDisplay();
			display();
			break;
		case 2:		//重新开始 
			redisplay();
			display();
			enable_Gaming = 1;
			break;
		case 3:		//元素选择 
			SavedTip[0] = '\0';
			if(enable_mousing == 1) {  
				initDisplay();
				display();
			} 
			else {
				enable_Gaming = 0;
				reInitDisplay();
				display(); 
			}
			enable_mousing = !enable_mousing;
			break;
		default:
			break;
	}
	
	menuListFigureMap[1] = enable_Gaming ? "手动求解 | Ctrl-T" : "自动求解 | Ctrl-T";
	selection = menuList(GenUIID(0),x+2*w,y-h, w, w, h, menuListFigureMap,sizeof(menuListFigureMap)/sizeof(menuListFigureMap[0]));
	if(selection > 0) selectedLabel = menuListFigureMap[selection];
	switch(selection) {
		case 1:		//手动求解 | 程序求解 
			SavedTip[0] = '\0';
			if(!enable_Gaming) {
				//画出游戏开始提示  
				startingGame  = 1;
				enable_machine = 0; 
				redisplay();
				display();
			}
			else {
				//画出程序求解提示 
				startingGame  = 0;
				enable_machine = 1;
				redisplay();
				display();
			 } 
			enable_Gaming = !enable_Gaming;
			break;
		case 2:		//显示所有可行解  
			SavedTip[0] = '\0';
			redisplay();
			machine_all_steping = 1;
			break;
		default:
			break;
	}
	
	selection = menuList(GenUIID(0),x+3*w,y-h, w, w, h, menuListHelp,sizeof(menuListHelp)/sizeof(menuListHelp[0]));
	if( selection > 0 ) selectedLabel = menuListHelp[selection];
	switch(selection) {
		case 1:		//游戏说明
			introductionGame();
			break;
		case 2:		//关于游戏 
			aboutGame(); 
			break;
		default:
			break;
	}
		
}

//判断一个位置是否是终点或起点 
int determine(int x ,int y) {
	
	if( x == StartX && y == StartY) {
		return STARTPOINT;
	}
	else if( x == FainalX && y == FainalY) {
		return TERMINAL;
	}
	
	return 0;
}


//GUI字符输入 
void CharEventProcess(char ch)
{
	uiGetChar(ch); // GUI字符输入
	display(); //刷新显示
}

//画出鼠标控件 
void drawMouse() {
	
	if(!enable_mousing)
		return; 
	
	switch(mouseSetList[mouseSet]) {
		case BARRIER:
			SetPenColor("Black");
			drawRectangle(mouse_x, mouse_y, BlockWidth, BlockHight, 1);
			break;
		case CHEACED:
			SetPenColor("White");
			drawRectangle(mouse_x, mouse_y, BlockWidth,BlockHight, 1);
			break;
		case STARTPOINT:
			SetPenColor("Blue");
			drawBox(mouse_x,mouse_y,BlockWidth,BlockHight,1,"SPoint",'L',"White");
			break;
		case TERMINAL:
			SetPenColor("Blue");
			drawBox(mouse_x,mouse_y,BlockWidth,BlockHight,1,"TPoint",'L',"White");
			break;
		default:
			break;
	}
	
	return;
}

 
void drawMyMaze(int mymouse_x,int mymouse_y){
	
	int tempx = mymouse_x - 1,tempy = mymouse_y - 1;
	
	if((tempx <  MazeWith && tempy < MazeHight) &&
	   (tempx > 0 && tempy > 0) );
	else 
		return;
	
	
	if(determine(tempx,tempy) && (mouseSet == 0 || mouseSet == 1))
		return ;
		
	if(mouseSetList[mouseSet] == STARTPOINT ) {
		maze[StartX][StartY] = CHEACED;
		StartX = tempx;
		StartY= tempy;
		maze[tempx][tempy] = mouseSetList[mouseSet];
		return;
	}
	if(mouseSetList[mouseSet] == TERMINAL) {
		maze[FainalX][FainalY] = CHEACED;
		FainalX = tempx;
		FainalY= tempy;
		maze[tempx][tempy] = mouseSetList[mouseSet];
		return;
	}
	
	
	maze[tempx][tempy] = mouseSetList[mouseSet]; 
	return;
}

//画出鼠标控件 
void drawButton() {
	
	//控件参数
	double buttonstartX =   winWidth - 1.75;
	double buttonstartY =   winHeight - 1.25;
	double buttonListX = 2.2; 
	double buttonListY = winHeight - 1.27;
	double buttonListW = buttonWidth;
	double buttonListH = buttonHight;
	double buttonRetraction = 0.5;
	int num;
	char tempFile[255] = "";
	char LoadFileName[255] = ""; 
	int i = 0,j = 0;
	
	//画出游戏暂停/开始按钮 
	if (startingGame) {
		if(button(GenUIID(0), buttonstartX, buttonstartY, buttonListW, buttonListH, enable_Gaming ? "暂停游戏" : "继续游戏")){
			enable_Gaming = ! enable_Gaming;
		}
		if(button(GenUIID(0), buttonstartX - 2.0, buttonstartY, buttonListW, buttonListH, enable_indicating ? "关闭提示" : "开启提示")){
			enable_indicating = !enable_indicating; 
		} 
	}
	
	if( !startingGame && enable_mousing ) {
		//画出选择元素必要控件 
		if(button(GenUIID(0), buttonListX, buttonListY, buttonListW + 0.5, buttonListH,"障碍物(Black)")){
			mouseSet = 0;
		}
		if(button(GenUIID(0), 2*buttonListX + buttonRetraction, buttonListY , buttonListW + 0.5, buttonListH,"路径(White)")){
			mouseSet = 1;
		}
		if(button(GenUIID(0), 3*buttonListX + buttonRetraction, buttonListY, buttonListW + 0.5, buttonListH,"起点(Blue)")){
			mouseSet = 2;
		}
		if(button(GenUIID(0), 4*buttonListX + buttonRetraction, buttonListY, buttonListW + 0.5, buttonListH,"终点(Blue)")){
			mouseSet = 3;
		}
		if(button(GenUIID(0), 6*buttonListX + buttonRetraction, buttonListY, buttonListW + 0.5, buttonListH,"创建该地图")){
			Mysit.x = StartX;
			Mysit.y = StartY;
			maze[Mysit.x][Mysit.y] = GAMER;
			enable_mousing = 0; 
			enable_Gaming = 1;
			startingGame = 1;
			free(pathStack);
			pathStack = NULL;
			isFindEnd = 0;
			//初始化路径地图 
			pathStack = initPath();
			allpath = initPath();
			//程序求解路径 
			machining(pathStack,StartX,StartY);
		}
	}
	if(show_file_button) {
		FILE* fp;
		if(textbox(GenUIID(0), 14.5, 0.1 , buttonListW + 0.5, buttonListH, FileMapLoad, sizeof(FileMapLoad))){
			sprintf(tempFile,"../SavedMap/%s", FileMapLoad);
			printf("%s\n",tempFile);
		}
		if(button(GenUIID(0),  12.0 , 0.1, buttonListW + 0.5, buttonListH,"载入地图文件")) {
			SavedTip[0] = '\0';
			sprintf(tempFile,"../SavedMap/%s", FileMapLoad);
			printf("%s\n",tempFile);
			//打开地图文件，并将其中内容赋值给迷宫数组
			if(NULL == (fp = fopen(tempFile,"r") )) {
				show_load_label = 0;
			} 
			else {
				show_load_label = 1;
				for( i = 0; i <= MazeWith; ++i ) {
					for( j = 0; j <= MazeHight; ++j) {
						fscanf(fp,"%d ",&num);
						maze[i][j] = num;
					}//for 
				}//for
				fscanf(fp,"%d ",&num);Mysit.x = num;fscanf(fp,"%d ",&num);Mysit.y = num;
				fscanf(fp,"%d ",&num);StartX = num;fscanf(fp,"%d ",&num);StartY = num;
				fscanf(fp,"%d ",&num);FainalX = num;fscanf(fp,"%d ",&num);FainalY = num;
				fclose(fp);
				isFindEnd = 0;
				//初始化路径地图 
				free(pathStack);
				free(allpath);
				pathStack = initPath();
				allpath = initPath();
				//程序求解路径 
				machining(pathStack,StartX,StartY);
			}//else 
			
		}//if
		 
	}//if
	
	if(enable_machine) {
		
		if(button(GenUIID(0), 2*buttonListX + buttonRetraction, buttonListY , buttonListW + 0.5, buttonListH,"程序求解")){
			//初始化地图 
			redisplay(); 
			machine_steping = 1;
			machine_onestep = pathStack->top; 
			machine_step = 1;
		}
		if(button(GenUIID(0), 3*buttonListX + buttonRetraction, buttonListY, buttonListW + 0.5, buttonListH,"单步执行")){
			if(( (pathStack->top-1) <= machine_onestep) || machine_moving){
				redisplay(); 
				machine_moving = 0;
				machine_step = machine_onestep = 1;
			}
			machine_steping = 1;
			machine_step = machine_onestep;
			 machine_onestep++;
			 if(machine_onestep == (pathStack->top - 1)) {
			 	machine_onestep = 1;
			 } 
		}
		if(button(GenUIID(0), 4*buttonListX + buttonRetraction, buttonListY, buttonListW + 0.5, buttonListH,"显示最短解")){
			redisplay(); 
			machine_steping = 0;
			machine_moving = 1;
			machine_step = 1;
		}
		if(button(GenUIID(0), 5*buttonListX + buttonRetraction, buttonListY, buttonListW + 0.5, buttonListH,"退出")){
			enable_machine = 0; 
			machine_steping = 0;
			machine_onestep = 1;
			machine_moving = 0;
			machine_step = 1;
			enable_Gaming = 1; 
			startingGame = 1; 
			//初始化地图 A
			redisplay(); 
			//初始化玩家位置 
			Mysit.x = StartX; 
			Mysit.y = StartY;
			maze[StartX][StartY] = GAMER;
		}
	}
	
}

//检查游戏是否胜利 
bool cheackWin() {
	if(Mysit.x == FainalX && Mysit.y == FainalY) {
		redisplay();
	}
}

//保存地图
void saveMap() {
	FILE* fp;
	int strLen = 0,i = 0,j = 0;
	char buf[255],temp[255];
	char fileName[255];
	strcpy(SavedTip,"地图保存为:");
	buf[0] = '\0';
	strcat(buf,MapPath);
//	printf("%s\n",buf);
	strcat(buf,MapName);
//	printf("%s\n",buf);
	strLen = strlen(buf);	
	do {
		itoa(MapList,temp,10);
		strcat(temp,fileSuffix);
		strcat(buf,temp);
//		printf("%s\n",buf);
//		printf("%s\n",temp);
		
		fp = fopen(buf,"r");
		
		if(fp == NULL){
			strcpy(fileName,buf);
		}
		else {
			buf[strLen] = '\0';
			temp[0] = '\0';
		}
		MapList++;
	}while(fp != NULL);
	
	fp = fopen(fileName,"w+");
//	printf("%s\n",fileName);
	for( i = 0; i <= MazeHight; ++i){
		for( j = 0; j <= MazeWith; ++j) {
			fprintf(fp,"%2d",maze[i][j]);
		}//for	
	}//for 
	
	fprintf(fp,"%3d%3d%3d%3d%3d%3d",Mysit.x,Mysit.y,StartX,StartY,FainalX,FainalY);
	
//	printf("%s\n",temp);
	strcat(SavedTip,MapName);
	strcat(SavedTip,temp);
	
	fclose(fp);
	
	
	return; 
}

//导入地图
void loadMap() {
	
	//
	show_file_button = !show_file_button; 
	
	return;
}

void drawLabels() {
	int i = 0; 
	SetPenColor("Black");
	if(strlen(SavedTip) > 0)
		drawLabel(SaveLabelX-2.0,SaveLabelY,SavedTip);
	
	if(show_load_label == 0 && show_file_button) {
		drawLabel(SaveLabelX + 2.0,SaveLabelY,"打开文件失败");
	} 
	else if(show_load_label == 1 && show_file_button) {
		drawLabel(SaveLabelX + 2.0,SaveLabelY,"导入地图成功");
	}
	else if(!show_file_button) {
		show_load_label = 2;
	}  
	
	return; 
}

STACK initPath() {
	STACK path = (STACK)malloc(sizeof(Stack));
	
	int i = 0,j = 0;
	
	for( i = 0; i <= MazeWith; ++i) {
		for( j = 0; j <= MazeHight; ++j) {
			pathMaze[i][j].dir = dir_up;
			pathMaze[i][j].isFind = 0;
		}
	}
	begPos.x = StartX;begPos.y = StartY;
	endPos.x = FainalX;endPos.y = FainalY; 
	pathMaze[StartX][StartY].isFind = 1;
	path->top = path->beg = 0;
	pushStack(path,StartX,StartY);
	
	return path;
}

void machining(STACK p,int x,int y) {
	if(isFindEnd) {
		return;
	}
	int i,j,tempx = 0,tempy = 0;
	STACK  path = p;
	Node mypos;
	mypos.x = x;
	mypos.y = y;
	Node currentPos = mypos;
//	printf("%d %d\n",currentPos.x,currentPos.y);
//	printf("%d %d %d %d\n",dir_up,dir_down,dir_left,dir_right);
//	printf("%d \n",path->top);
	Node searchPos; 
	
	//循环寻路 
	while(1) {
		//找到这一次的试探点
		searchPos.x = currentPos.x;
		searchPos.y = currentPos.y;
		//判断是否能走
		switch(pathMaze[currentPos.x][currentPos.y].dir) {
			case dir_up:
				searchPos.y++;
				//改试探方向
				pathMaze[currentPos.x][currentPos.y].dir++;
				if(pathMaze[searchPos.x][searchPos.y].isFind != 1 &&
				   (maze[searchPos.x][searchPos.y] != BOUND && maze[searchPos.x][searchPos.y] != BARRIER)){
					dirMap[currentPos.x][currentPos.y].isFind = 1;
					dirMap[currentPos.x][currentPos.y].dir = dir_up;
					//能走 
//					printf("PUSH x:%d y:%d\n",searchPos.x,searchPos.y);
					//标记走过 
					pathMaze[searchPos.x][searchPos.y].isFind = 1;
					currentPos = searchPos;
					//入栈 
					pushStack(path,searchPos.x,searchPos.y);
					pushStack(allpath,searchPos.x,searchPos.y);
				}
				break;
			case dir_down:
				searchPos.y--;
				pathMaze[currentPos.x][currentPos.y].dir++;
				if(pathMaze[searchPos.x][searchPos.y].isFind != 1 &&
				   (maze[searchPos.x][searchPos.y] != BOUND && maze[searchPos.x][searchPos.y] != BARRIER)){
					dirMap[currentPos.x][currentPos.y].isFind = 1;
					dirMap[currentPos.x][currentPos.y].dir = dir_down;
					//能走 
//					printf("PUSH x:%d y:%d\n",searchPos.x,searchPos.y);
					//标记走过 
					pathMaze[searchPos.x][searchPos.y].isFind = 1;
					currentPos = searchPos;
					//入栈 
					pushStack(path,searchPos.x,searchPos.y);
					pushStack(allpath,searchPos.x,searchPos.y);
				}
				break;
			case dir_left:
				searchPos.x--;
				pathMaze[currentPos.x][currentPos.y].dir++;
				if(pathMaze[searchPos.x][searchPos.y].isFind != 1 &&
				   (maze[searchPos.x][searchPos.y] != BOUND && maze[searchPos.x][searchPos.y] != BARRIER)){
					dirMap[currentPos.x][currentPos.y].isFind = 1;
					dirMap[currentPos.x][currentPos.y].dir = dir_left;
					//能走 
//					printf("PUSH x:%d y:%d\n",searchPos.x,searchPos.y);
					currentPos = searchPos;
					//标记走过 
					pathMaze[currentPos.x][currentPos.y].isFind = 1;
					//入栈 
					pushStack(path,searchPos.x,searchPos.y);
					pushStack(allpath,searchPos.x,searchPos.y);
				}
				break;
			case dir_right:
				searchPos.x++;
				if(pathMaze[searchPos.x][searchPos.y].isFind != 1 &&
				   (maze[searchPos.x][searchPos.y] != BOUND && maze[searchPos.x][searchPos.y] != BARRIER)){
					dirMap[currentPos.x][currentPos.y].isFind = 1;
					dirMap[currentPos.x][currentPos.y].dir = dir_right;
					//能走 
					currentPos = searchPos;
					//标记走过 
					pathMaze[currentPos.x][currentPos.y].isFind = 1;
					//入栈 
//					printf("PUSH x:%d y:%d\n",searchPos.x,searchPos.y);
					pushStack(path,searchPos.x,searchPos.y);
					pushStack(allpath,searchPos.x,searchPos.y);
				}
				else {
					Node top = currentPos;
//					printf("x:%d y:%d POP! \n",top.x,top.y);
					//不能走，且是死胡同 回退 
					popStack(path);
					currentPos = getTop(path);
					dirMap[top.x][top.y].dir =  getDir(top,currentPos);
				} 
				break;
			default:
				
				break;
		}
		
//		printf("x:%d y:%d\n",currentPos.x,currentPos.y);
		
		//是否找到终点
		if(currentPos.x == FainalX && currentPos.y == FainalY) {
//			printf("getEnd\n");
			for( i = 0; i < path->top; ++i) {
//				printf("(%d,%d) ",path->list[i].x,path->list[i].y);
			}
//			printf("\n");
			isFindEnd = 1; 
			break;
		}
		//每个角落都找遍了，回到起点了，还是没找到终点， 循环结束 
		if(isEmpty(path)) {
//			printf("getStart\n");
			break;
		}
	}
	
	for( i = 0; (i+1) < path->top; ++i) {
		dirMap[path->list[i].x][path->list[i].y].isFind = 1;
		tempx = path->list[i+1].x - path->list[i].x;
		tempy = path->list[i+1].y - path->list[i].y;
		if(tempx == 1) {
			dirMap[path->list[i].x][path->list[i].y].dir = dir_right;
		} 
		else if(tempx == -1) {
			dirMap[path->list[i].x][path->list[i].y].dir = dir_left;
		}
		else if(tempy == 1){
			dirMap[path->list[i].x][path->list[i].y].dir = dir_up;
		}
		else if(tempy == -1){
			dirMap[path->list[i].x][path->list[i].y].dir = dir_down;
		}
	}
	
	for( i = 0; i < (path->top -1); ++i) {
		printf("%d\n",dirMap[path->list[i].x][path->list[i].y].dir);
	}
	
}
//入栈 
void pushStack(STACK s,int x,int y){
	
	Node tempNode;
	tempNode.x = x;
	tempNode.y = y;
	s->list[s->top] = tempNode;
	s->top++;
	return;
}
//出栈 
void popStack(STACK s) {
	--s->top; 
}
//获取栈顶元素 
Node getTop(STACK s) {
	Node temp = s->list[s->top-1];
	return temp;
}
//判断栈空 
int isEmpty(STACK s) {
	return (s->top == s->beg);
}
//画出一个路径栈 
void drawPath(STACK path) {
	
 	int i = 0,j = 0, len = path->top - 1;
	
//	for( i = 1; i < len; ++i) {
//		printf("x:%d y:%d\n",path->list[i].x,path->list[i].y); 
//	}
	
	if(isEmpty(path)) {
		return;
	}
	maze[Mysit.x][Mysit.y] = CHEACED;
 	maze[StartX][StartY] = STARTPOINT;
 	maze[FainalX][FainalY] = TERMINAL;
 	
 	
 	for( i = 1 ; i < len; ++i) {
 		maze[path->list[i].x][path->list[i].y] = MACHINE;
 	}
 	
}			
//设置一个画块及其类型 	
void drawBolck(int x,int y,int type) {
	maze[x][y] = type;
	return;
}		

//设置自定义颜色 
void setMyColor(){

	return; 
}	

//游戏说明 
void introductionGame() {
	
	system("cd ../doc && Gameintroduction.txt ");
	system("exit");
	return; 
}

//游戏介绍
void aboutGame() {
	
	system("cd ../doc && AboutGame.txt");
	system("exit");
	return;
} 	

//判断两点之间的走向 
int getDir(Node now,Node to) {
	int x,y;
	x = to.x - now.x;
	y = to.y - now.y;
	if(x == 1) {
		return dir_right;
	}
	else if( x == -1) {
		return dir_left;
	}
	else if( y == 1) {
		return dir_up;
	}
	else if( y == -1) {
		return dir_down;
	}
}
	
