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
