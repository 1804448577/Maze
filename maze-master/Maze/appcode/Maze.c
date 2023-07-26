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

//�������� 
#define BOUND 0
#define UNCHEACK 1
#define BARRIER 2
#define CHEACED 3
#define MYBOlOCK 5
#define STARTPOINT 6
#define TERMINAL 7 
#define GAMER 8
#define MACHINE 9


//��ʱ���ؼ�λ�� 
#define TimeShowX 1.0
#define TimeShowY 16.5
#define TimeShowH 1.0
#define TimeShowW 1.175

//���ӿؼ�λ�� 
#define SeedShowX 1.75
#define SeedShowY 0.25
#define SeedShowH 0.5
#define SeedShowW 1.75

//��ͼ���� 
typedef struct _sit_ {
	int x,y;		//���Թ��е�λ�� 

}*NODE,Node;

typedef struct pathnode {
	int dir;
	int isFind;
}*PATHNODE,pathNode; 

//��ǩ�ṹ�� 
typedef struct _label_ {
	double x,y;
	char Txt[255]; 
}*LABEL,Label;

//��ʱ�� 
typedef enum {
	GameTimer,
	MouseTimer,
	MachineTimer,
	AllTimer,
	StepTimer,
} MyTimer;

//�Զ�Ѱ·���� 
typedef enum {
	dir_up,
	dir_down,
	dir_left,
	dir_right,
}DIR; 

//ջ 
typedef struct stack {
	Node list[100];
	int top;
	int beg;
}*STACK,Stack; 

//��ͼ����
static char MapName[255] = "Map"; 
static char fileSuffix[255] = ".txt";
static char MapPath[255] = "../SavedMap/"; 
static char FileMapLoad[255] = "�����ļ���"; 
static char SavedTip[255] = "";
static char saveName[255];
static char openName[255];
static int MapList = 1;

//��ǩ����
//int labelNums = 0;
static double SaveLabelX = 7.0;
static double SaveLabelY = 16.5;

//��Ϸ���̲���
static int startingGame = 1; 
static int choicingBlocks = 0;

//�������
static double BlockWidth = 1.0;
static double BlockHight = 1.0; 


//��ť����
static double buttonWidth = 1.6;
static double buttonHight = 0.75; 

//���յ�λ��
static int StartX = 1; 
static int StartY = 13;
static int FainalX = 13;
static int FainalY = 1;

//������ 
static int enable_mousing = 0;
static double mouse_x = 0;
static double mouse_y = 0;
static int mouseSet = 0;
static int mouseSetList[9] = {BARRIER,CHEACED,STARTPOINT,TERMINAL};
static char mouseColorList[9][256] = {{"Black"},{"White"},{"Bule"},{"Bule"},}; 

//��ϷԪ�ز��� 
static int    enable_Gaming = 1;			//������Ϸ��ʼ 
static int    enable_indicating	= 1;		//��Ϸ��ʾ��ʾ 
static int    show_more_buttons = 0;		//��ʾ���ఴť 
static int 	  show_file_button = 0;			//��ʾ�ļ�ϵͳ��ť
static int    machine_step = 1;				//������ⲽ��
static int    machine_all_step = 1;			//�������нⲽ�� 
static int    machine_onestep = 1;			//������� 
static int    machine_all_steping = 0;		//ȫ��·��չʾ 
static int 	  enable_machine = 0;			//������� 
static int    machine_moving = 0;	 		//�����·�� 
static int    machine_steping = 0;			//����|������� 
static int 	  show_load_label = 2; 			//��ʾ�ļ���״̬ 
static int 	  randmoSeed = 0;				//�������ֵ 
static Node Mysit;							//���λ�� 
static Node begPos,endPos;					//���յ� 
static int isFindEnd = 0;					//�ҵ��յ��־ 
char * c[20] = {"Black","Dark Gray","Gray","Light Gray","White", "Cyan",
			"Brown", "Red", "Orange", "Yellow", "Green","Blue","Violet", "Magenta"}; //��ɫ 
 
int MazeWith = 14;		//�Թ���� 
int MazeHight = 14;		//�Թ��߶� 
int SeedLists = 0;		//�������к�
 
//int	startTime = 0;		

double winHeight = 17.5;		//������� 
double winWidth = 17.0; 		//�����߶� 

static char Myseed[10] = "�������ֵ"; 
static char TimeTips[100] = "ʣ��ʱ��:"; 
static char TimerStr[100] = "60";
static char LabelSeed[80] = "1";
static char SeedSet[80] = "1";
static char results[256] = "";
 
 //�Թ����� 
int maze[MazeSize][MazeSize] = {0,};
pathNode pathMaze[MazeSize][MazeSize] = {0,};
pathNode dirMap[MazeSize][MazeSize] = {0,};
STACK pathStack;
STACK allpath;

void initDisplay();		//��ʼ������ 
void reInitDisplay();	//���³�ʼ������ 
void redisplay();		//������滭�� 
STACK initPath();		//��ʼ��·������ 
void display();			//���� 
void drawMaze();		//���Թ� 
bool cheackWin();		//�����Ϸ�Ƿ�ʤ�� 
void gameWin();			//��Ϸʤ������
void gameLose();		//��Ϸʧ�� 
void saveMap();			//�����ͼ
void loadMap();			//���ص�ͼ  
void aboutGame();		//������Ϸ 
void setMyColor();		//������ɫ 
void introductionGame();//��Ϸ˵��
void machining(STACK,int,int);	//�������
int  isEmpty(STACK);			//�ж�ջ�� 
void popStack(STACK s); 		//��ջ���� 
Node getTop(STACK s);   		//��ȡջ��Ԫ��
int getDir(Node now,Node to);	//�ж�����֮������� 
void pushStack(STACK,int,int);				//��ջ 
void drawMyMaze(int,int);					//���Լ����Թ� 
int determine(int,int);						//�ж��Ƿ����յ������� 
void setSeed(int seed);						//����������� 
void drawSeedControler();					//����������ӿؼ� 
void drawTimer();							//������ʱ���ؼ�
void drawMenu();							//�����˵��ؼ� 
void drawMouse();							//�������ؼ� 
void drawButton();							//������ť�ؼ� 
void drawLabels();							//������ǩ��� 
void drawBolck(int,int,int);				//����һ�����鼰������ 
void drawPath(STACK);						//����һ��·��ջ 
void setLabel(double,double,char*);			//����һ��Label�Ĳ��� 
int cheackWay(int index_X,int index_Y);				//�����Թ��Ĺ����м��·���Ƿ���� 
void createRandomMaze_01(int index_X,int index_Y);	//��������Թ���ʽһ 
void KeyboardEventProcess(int key, int event);				//���̿ؼ����� 
void CharEventProcess(char ch); 							//GUI�ַ����� 
void TimerProcess(int timerID);								//��ʱ������
void MouseEventProcess(int x, int y, int button, int event);//���ؼ����� 


void Main()
{		
	
	//���ư� 
//	InitConsole(); 
	
	initDisplay(); 
	SetWindowTitle("�Թ�");
	InitGraphics();
	
	
	registerCharEvent(CharEventProcess);        	// �ַ�
	registerTimerEvent(TimerProcess);				//��ʱ�� 
	registerKeyboardEvent(KeyboardEventProcess);	//���� 
	registerMouseEvent(MouseEventProcess);			//��� 
	
	
//	startTimer(MouseTimer,10);
	startTimer(GameTimer,1000);
	startTimer(MachineTimer,25);
	startTimer(StepTimer,100);
	startTimer(AllTimer,100);
	
	
	display();
}

//��ʼ������ 
void initDisplay() {
	
	
	char buffer[100]; 
	int i = 0,j = 0,buf = 0; 
	//��ʼ��������� 
	SetWindowSize(winWidth,winHeight);
	
	//��ʼ�����Ӳ��� 
	setSeed(randmoSeed);	
	
	//��ʼ���Թ� 
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
	
	//������Թ� 
	createRandomMaze_01(CreateStartX,CreateStartY);
	
	//��ʼ���ҵ�λ�� 
	Mysit.x = 1;
	Mysit.y = 13; 
	//��ʼ�����λ��
	StartX = 1;
	StartY = 13;
	//��ʼ���յ�λ��
	FainalX = 13;
	FainalY = 1;

	maze[StartX][StartY] = STARTPOINT;
	maze[Mysit.x][Mysit.y] = GAMER;
	maze[FainalX][FainalY] = TERMINAL;
	
	//��ʼ����ʱ��
	itoa(60,TimerStr,10);
	
	//��ʼ��������ʾ
	itoa(randmoSeed,SeedSet,10);
	
	//��ʼ���������к� 
	FILE* fp = fopen("SeedList.bin","r");
	
	if(fp == NULL) {
		//���������򴴽���Ӧ�����������ļ� 
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
	//��ʼ��·����ͼ 
	pathStack = initPath();
	allpath = initPath();
	//�������·�� 
	machining(pathStack,StartX,StartY);
	
	fclose(fp);
	free(fp);
	
	
}

//���³�ʼ������ 
void reInitDisplay() {
	
	int i = 0,j = 0,buf = 0; 
	//��ʼ��������� 
	SetWindowSize(winWidth,winHeight);
	
	//���³�ʼ���Թ� 
	int tempH = MazeHight - 1;
	int tempW = MazeWith - 1;
	for( i = 1; i <= tempH; ++i) {
		for( j = 1; j <= tempW; ++j) {
				maze[i][j] = CHEACED;
		}
	}
	//��ʼ����ʱ��
	itoa(60,TimerStr,10);
	
	//��ʼ��������ʾ
	itoa(randmoSeed,SeedSet,10);
	
	//�������зǱ�Ҫ�ؼ�
	startingGame = 0;		//��ͣ��Ϸ��ť
	
	maze[StartX][StartY] = STARTPOINT;
	maze[FainalX][FainalY] = TERMINAL;
	
}
void redisplay() {
	int i = 0,j = 0;
	//��ʼ����ʱ��
	itoa(60,TimerStr,10);
	
	//��ʼ��������ʾ
	itoa(randmoSeed,SeedSet,10);
	
	 
	//��ʼ����ͼ 
	for( i = 1; i < MazeWith; ++i) {
		for(j = 1; j < MazeHight; ++j) {
			maze[i][j] = ( maze[i][j] == BARRIER) ? BARRIER : CHEACED;
		}
	}
	
	//��ʼ��λ�� 
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
	//��ʼ��·����ͼ 
	pathStack = initPath();
	allpath = initPath();
	//�������·�� 
	machining(pathStack,StartX,StartY);
	
	
	display();
}
//��ʾ���� 
void display() {
	//ˢ�»��� 
	DisplayClear();
	//�����Թ� 
	drawMaze();
	//����������ӿؼ� 
	drawSeedControler();
	//������ʱ���ؼ� 
	drawTimer(); 
	//�����˵��ؼ� 
	drawMenu();
	//�������ؼ� 
	drawMouse();
	//������ť�ؼ� 
	drawButton();
	//������ǩ��� 
	drawLabels();
	return;
}

//�����Թ� 
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

//����������� 
void setSeed(int seed) {
	randmoSeed = seed % MAXINT; 
	srand( randmoSeed );
	return; 
}

//��ȡ����·�� 
int cheackWay(int index_X,int index_Y) {
	
	int ways = 0;
//	return ways; 
	//����ϲ��Ƿ��ͨ 
	if(  (index_X - 2) > 0 &&
		 //(index_X <= MazeWith) &&
		 //(index_Y > 0 && index_Y <= MazeHight) &&
		 (maze[index_X - 2][index_Y] == 1) 
	) {
		++ways;
	}
	//����²��Ƿ��ͨ 
   if(  (index_X + 2) <= MazeWith &&
//		 (index_X  > 0) &&
//		 (index_Y > 0 && index_Y <= MazeHight) &&
		 (maze[index_X + 2][index_Y] == 1) 
	) {
		++ways;
	}
	
	//����Ҳ��Ƿ��ͨ 
	if(  ((index_Y + 2) <=  MazeWith )&&
		 //(index_Y  > 0) &&
		 //(index_X > 0 && index_X <=  MazeHight) &&
		 (maze[index_X][index_Y + 2] == 1) 
	) {
		++ways;
	}
	
	
	//�������Ƿ��ͨ 
	if(  (index_Y - 2) > 0 &&
		 //(index_Y  <= MazeHight) &&
		 //(index_X > 0 && index_X <= MazeWith) &&
		 (maze[index_X][index_Y - 2] == 1) 
	) {
		++ways;
	}
	
	return ways;
}

//��������Թ� 
void createRandomMaze_01(int index_X,int index_Y) {
	if(maze[index_X][index_Y] == 1) {
		maze[index_X][index_Y] = 3;
	}
	
	//����·������ 
	int random_index_Ways = 0;
	//ѡ���·�� 
	int random_way = 0;
	//�洢λ�� 
	int tempX = index_X,tempY = index_Y;
	while(1) {
		random_index_Ways =  cheackWay(index_X,index_Y);
		if(random_index_Ways == 0) {
			break;
		} 
		else {
			random_way = rand()%4;
			switch(random_way) {
				//���� 
				case 0:
					if(index_Y + 2 <= MazeHight && maze[index_X][index_Y+2] == 1){
						maze[index_X][index_Y+1] = 3;
						maze[index_X][index_Y+2] = 3;
						index_Y = index_Y + 2;
					}
					break;
				//���� 
				case 1:
					if(index_Y-2 > 0 && maze[index_X][index_Y-2] == 1){
						maze[index_X][index_Y-1] = 3;	
						maze[index_X][index_Y-2] = 3;
						index_Y = index_Y - 2;	
					}
					break;
				//���� 
				case 2:
					if(index_X+2 <= MazeWith &&  maze[index_X+2][index_Y] == 1){
						maze[index_X+1][index_Y] = 3;
						maze[index_X+2][index_Y] = 3;
						index_X = index_X + 2;
					}
					break;
				//���� 
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

//����������ӿؼ� 
void drawSeedControler() {
	SetPenColor("Black");
	drawLabel(SeedShowX - 1.5, SeedShowY+0.15, Myseed);
	if(textbox(GenUIID(0), SeedShowX, SeedShowY, SeedShowW, SeedShowH, SeedSet, sizeof(SeedSet)))
		sprintf(LabelSeed,"%s", SeedSet);
	return;
}

//������������ 
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

//��ʱ���������� 
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

//����������� 
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

//������ʱ���ؼ� 
void drawTimer() 
{
	SetPenColor("Black");
	drawLabel(TimeShowX + 0.35, TimeShowY, TimerStr);
	SetPenColor("Red");
	drawLabel(TimeShowX - 1.0, TimeShowY + 0.005, TimeTips);
	
	
}

//�����˵��ؼ� 
void drawMenu() {
	static char * menuListFile[] = {"�ļ�",  
		"�򿪵�ͼ  | Ctrl-O", // ��ݼ��������[Ctrl-X]��ʽ�������ַ����Ľ�β
		"�����ͼ  | Ctrl-X",
		"�˳���Ϸ  | Ctrl-Q",};
		
	static char * menuListTool[] = {"��ͼ�༭",
		"���������ͼ | Ctrl-L",
		"���¿�ʼ     | Ctrl-R",
		"Ԫ��ѡ�� ",};
	
	static char * menuListHelp[] = {"����",
		"��Ϸ˵�� ",
		"������Ϸ "};
		
		
	static char * menuListFigureMap[] = {"��ͼ���",
		"�ֶ���� | Ctrl-T",
		"��ʾ���п��н� "};
		
	static char * selectedLabel = NULL;

	double fH = GetFontHeight();
	double x = 0; //fH/8;
	double y = winHeight;
	double h = fH*1.5; // �ؼ��߶�
	double w = TextStringWidth(menuListHelp[0])*2 + 1.0;		// �ؼ����
	double wlist = TextStringWidth(menuListTool[3])*1.2 + 0.6;	//�б��� 
	double xindent = winHeight/20; // ����
	int    selection;
	
	drawMenuBar(0,y-h,winWidth,h);
	
	//�˵� 
	selection = menuList(GenUIID(0), x, y-h, w, w, h, menuListFile, sizeof(menuListFile)/sizeof(menuListFile[0]));
	if( selection > 0 ) selectedLabel = menuListFile[selection];
	switch(selection) {
		case 1:		//�򿪵�ͼ 
			loadMap(); 
			break;
		case 2:		//�����ͼ 
			saveMap();
			break;
		case 3:		//�˳���Ϸ 
			exit(-1); 
			break;
		default:
			break;
	}
		
	selection = menuList(GenUIID(0),x+w,  y-h, w, w,h, menuListTool,sizeof(menuListTool)/sizeof(menuListTool[0]));
	if( selection > 0 ) selectedLabel = menuListTool[selection];
	switch(selection) {
		case 1:		//�����������
			randmoSeed = rand();
			initDisplay();
			display();
			break;
		case 2:		//���¿�ʼ 
			redisplay();
			display();
			enable_Gaming = 1;
			break;
		case 3:		//Ԫ��ѡ�� 
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
	
	menuListFigureMap[1] = enable_Gaming ? "�ֶ���� | Ctrl-T" : "�Զ���� | Ctrl-T";
	selection = menuList(GenUIID(0),x+2*w,y-h, w, w, h, menuListFigureMap,sizeof(menuListFigureMap)/sizeof(menuListFigureMap[0]));
	if(selection > 0) selectedLabel = menuListFigureMap[selection];
	switch(selection) {
		case 1:		//�ֶ���� | ������� 
			SavedTip[0] = '\0';
			if(!enable_Gaming) {
				//������Ϸ��ʼ��ʾ  
				startingGame  = 1;
				enable_machine = 0; 
				redisplay();
				display();
			}
			else {
				//�������������ʾ 
				startingGame  = 0;
				enable_machine = 1;
				redisplay();
				display();
			 } 
			enable_Gaming = !enable_Gaming;
			break;
		case 2:		//��ʾ���п��н�  
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
		case 1:		//��Ϸ˵��
			introductionGame();
			break;
		case 2:		//������Ϸ 
			aboutGame(); 
			break;
		default:
			break;
	}
		
}

//�ж�һ��λ���Ƿ����յ����� 
int determine(int x ,int y) {
	
	if( x == StartX && y == StartY) {
		return STARTPOINT;
	}
	else if( x == FainalX && y == FainalY) {
		return TERMINAL;
	}
	
	return 0;
}


//GUI�ַ����� 
void CharEventProcess(char ch)
{
	uiGetChar(ch); // GUI�ַ�����
	display(); //ˢ����ʾ
}

//�������ؼ� 
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

//�������ؼ� 
void drawButton() {
	
	//�ؼ�����
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
	
	//������Ϸ��ͣ/��ʼ��ť 
	if (startingGame) {
		if(button(GenUIID(0), buttonstartX, buttonstartY, buttonListW, buttonListH, enable_Gaming ? "��ͣ��Ϸ" : "������Ϸ")){
			enable_Gaming = ! enable_Gaming;
		}
		if(button(GenUIID(0), buttonstartX - 2.0, buttonstartY, buttonListW, buttonListH, enable_indicating ? "�ر���ʾ" : "������ʾ")){
			enable_indicating = !enable_indicating; 
		} 
	}
	
	if( !startingGame && enable_mousing ) {
		//����ѡ��Ԫ�ر�Ҫ�ؼ� 
		if(button(GenUIID(0), buttonListX, buttonListY, buttonListW + 0.5, buttonListH,"�ϰ���(Black)")){
			mouseSet = 0;
		}
		if(button(GenUIID(0), 2*buttonListX + buttonRetraction, buttonListY , buttonListW + 0.5, buttonListH,"·��(White)")){
			mouseSet = 1;
		}
		if(button(GenUIID(0), 3*buttonListX + buttonRetraction, buttonListY, buttonListW + 0.5, buttonListH,"���(Blue)")){
			mouseSet = 2;
		}
		if(button(GenUIID(0), 4*buttonListX + buttonRetraction, buttonListY, buttonListW + 0.5, buttonListH,"�յ�(Blue)")){
			mouseSet = 3;
		}
		if(button(GenUIID(0), 6*buttonListX + buttonRetraction, buttonListY, buttonListW + 0.5, buttonListH,"�����õ�ͼ")){
			Mysit.x = StartX;
			Mysit.y = StartY;
			maze[Mysit.x][Mysit.y] = GAMER;
			enable_mousing = 0; 
			enable_Gaming = 1;
			startingGame = 1;
			free(pathStack);
			pathStack = NULL;
			isFindEnd = 0;
			//��ʼ��·����ͼ 
			pathStack = initPath();
			allpath = initPath();
			//�������·�� 
			machining(pathStack,StartX,StartY);
		}
	}
	if(show_file_button) {
		FILE* fp;
		if(textbox(GenUIID(0), 14.5, 0.1 , buttonListW + 0.5, buttonListH, FileMapLoad, sizeof(FileMapLoad))){
			sprintf(tempFile,"../SavedMap/%s", FileMapLoad);
			printf("%s\n",tempFile);
		}
		if(button(GenUIID(0),  12.0 , 0.1, buttonListW + 0.5, buttonListH,"�����ͼ�ļ�")) {
			SavedTip[0] = '\0';
			sprintf(tempFile,"../SavedMap/%s", FileMapLoad);
			printf("%s\n",tempFile);
			//�򿪵�ͼ�ļ��������������ݸ�ֵ���Թ�����
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
				//��ʼ��·����ͼ 
				free(pathStack);
				free(allpath);
				pathStack = initPath();
				allpath = initPath();
				//�������·�� 
				machining(pathStack,StartX,StartY);
			}//else 
			
		}//if
		 
	}//if
	
	if(enable_machine) {
		
		if(button(GenUIID(0), 2*buttonListX + buttonRetraction, buttonListY , buttonListW + 0.5, buttonListH,"�������")){
			//��ʼ����ͼ 
			redisplay(); 
			machine_steping = 1;
			machine_onestep = pathStack->top; 
			machine_step = 1;
		}
		if(button(GenUIID(0), 3*buttonListX + buttonRetraction, buttonListY, buttonListW + 0.5, buttonListH,"����ִ��")){
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
		if(button(GenUIID(0), 4*buttonListX + buttonRetraction, buttonListY, buttonListW + 0.5, buttonListH,"��ʾ��̽�")){
			redisplay(); 
			machine_steping = 0;
			machine_moving = 1;
			machine_step = 1;
		}
		if(button(GenUIID(0), 5*buttonListX + buttonRetraction, buttonListY, buttonListW + 0.5, buttonListH,"�˳�")){
			enable_machine = 0; 
			machine_steping = 0;
			machine_onestep = 1;
			machine_moving = 0;
			machine_step = 1;
			enable_Gaming = 1; 
			startingGame = 1; 
			//��ʼ����ͼ A
			redisplay(); 
			//��ʼ�����λ�� 
			Mysit.x = StartX; 
			Mysit.y = StartY;
			maze[StartX][StartY] = GAMER;
		}
	}
	
}

//�����Ϸ�Ƿ�ʤ�� 
bool cheackWin() {
	if(Mysit.x == FainalX && Mysit.y == FainalY) {
		redisplay();
	}
}

//�����ͼ
void saveMap() {
	FILE* fp;
	int strLen = 0,i = 0,j = 0;
	char buf[255],temp[255];
	char fileName[255];
	strcpy(SavedTip,"��ͼ����Ϊ:");
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

//�����ͼ
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
		drawLabel(SaveLabelX + 2.0,SaveLabelY,"���ļ�ʧ��");
	} 
	else if(show_load_label == 1 && show_file_button) {
		drawLabel(SaveLabelX + 2.0,SaveLabelY,"�����ͼ�ɹ�");
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
	
	//ѭ��Ѱ· 
	while(1) {
		//�ҵ���һ�ε���̽��
		searchPos.x = currentPos.x;
		searchPos.y = currentPos.y;
		//�ж��Ƿ�����
		switch(pathMaze[currentPos.x][currentPos.y].dir) {
			case dir_up:
				searchPos.y++;
				//����̽����
				pathMaze[currentPos.x][currentPos.y].dir++;
				if(pathMaze[searchPos.x][searchPos.y].isFind != 1 &&
				   (maze[searchPos.x][searchPos.y] != BOUND && maze[searchPos.x][searchPos.y] != BARRIER)){
					dirMap[currentPos.x][currentPos.y].isFind = 1;
					dirMap[currentPos.x][currentPos.y].dir = dir_up;
					//���� 
//					printf("PUSH x:%d y:%d\n",searchPos.x,searchPos.y);
					//����߹� 
					pathMaze[searchPos.x][searchPos.y].isFind = 1;
					currentPos = searchPos;
					//��ջ 
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
					//���� 
//					printf("PUSH x:%d y:%d\n",searchPos.x,searchPos.y);
					//����߹� 
					pathMaze[searchPos.x][searchPos.y].isFind = 1;
					currentPos = searchPos;
					//��ջ 
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
					//���� 
//					printf("PUSH x:%d y:%d\n",searchPos.x,searchPos.y);
					currentPos = searchPos;
					//����߹� 
					pathMaze[currentPos.x][currentPos.y].isFind = 1;
					//��ջ 
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
					//���� 
					currentPos = searchPos;
					//����߹� 
					pathMaze[currentPos.x][currentPos.y].isFind = 1;
					//��ջ 
//					printf("PUSH x:%d y:%d\n",searchPos.x,searchPos.y);
					pushStack(path,searchPos.x,searchPos.y);
					pushStack(allpath,searchPos.x,searchPos.y);
				}
				else {
					Node top = currentPos;
//					printf("x:%d y:%d POP! \n",top.x,top.y);
					//�����ߣ���������ͬ ���� 
					popStack(path);
					currentPos = getTop(path);
					dirMap[top.x][top.y].dir =  getDir(top,currentPos);
				} 
				break;
			default:
				
				break;
		}
		
//		printf("x:%d y:%d\n",currentPos.x,currentPos.y);
		
		//�Ƿ��ҵ��յ�
		if(currentPos.x == FainalX && currentPos.y == FainalY) {
//			printf("getEnd\n");
			for( i = 0; i < path->top; ++i) {
//				printf("(%d,%d) ",path->list[i].x,path->list[i].y);
			}
//			printf("\n");
			isFindEnd = 1; 
			break;
		}
		//ÿ�����䶼�ұ��ˣ��ص�����ˣ�����û�ҵ��յ㣬 ѭ������ 
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
//��ջ 
void pushStack(STACK s,int x,int y){
	
	Node tempNode;
	tempNode.x = x;
	tempNode.y = y;
	s->list[s->top] = tempNode;
	s->top++;
	return;
}
//��ջ 
void popStack(STACK s) {
	--s->top; 
}
//��ȡջ��Ԫ�� 
Node getTop(STACK s) {
	Node temp = s->list[s->top-1];
	return temp;
}
//�ж�ջ�� 
int isEmpty(STACK s) {
	return (s->top == s->beg);
}
//����һ��·��ջ 
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
//����һ�����鼰������ 	
void drawBolck(int x,int y,int type) {
	maze[x][y] = type;
	return;
}		

//�����Զ�����ɫ 
void setMyColor(){

	return; 
}	

//��Ϸ˵�� 
void introductionGame() {
	
	system("cd ../doc && Gameintroduction.txt ");
	system("exit");
	return; 
}

//��Ϸ����
void aboutGame() {
	
	system("cd ../doc && AboutGame.txt");
	system("exit");
	return;
} 	

//�ж�����֮������� 
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
	
