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
