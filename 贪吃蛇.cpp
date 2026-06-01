#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <windows.h>

//设定地图的长和宽
#define WIDTH 20
#define HEIGHT 10

//定义蛇的结构体
typedef struct 
{
	int x;
	int y;
}Snake;

Snake snake[100];//蛇身数组
int snakeLen;//蛇长度数组

//食物坐标
int foodX;
int foodY;

char direction;//蛇移动的方向
int score ;//得分
int gameOver;//游戏结束标记（0：继续，1：结束）

void InitGame();//初始化游戏
void DrawGame();//绘制游戏界面
void ControlSnake();//控制蛇的方向
void MoveSnake();//移动蛇
void GenerateFood();//生成食物
void SetConsolePos(int x, int y); // 设置光标位置（解决闪烁核心）
void HideCursor();     // 隐藏控制台光标

int main()
{
	HideCursor();
	InitGame();
	
	while(!gameOver)
	{
		DrawGame();//绘制界面
		ControlSnake();//检测键盘输入
		MoveSnake();//移动蛇
		Sleep(200);//windows平台专用延时函数，意为让程序暂停200ms，1秒=1000毫秒
		//此处用于控制移动速度
	}
	
	//游戏结束提示
	system("cls");//对控制台进行清屏的命令
	printf("=====游戏结束=====\n");
	printf("你的得分：%d\n",score);
	printf("感谢游玩！\n");
	return 0;
}

//初始化游戏
void InitGame()
{
	gameOver=0;
	direction='d';//初始化蛇移动方向
	snakeLen=3;//初始化蛇长
	//初始化蛇身
	snake[0].x =WIDTH/2;
	snake[0].y =HEIGHT/2;
	for(int i=1;i<snakeLen;i++)
	{
		snake[i].x =snake[0].x-i;
		snake[i].y =HEIGHT/2; 
	}
	
	score=0;
    GenerateFood();
}

//设置控制台光标位置
void SetConsolePos(int x, int y)
{
    COORD pos = {x, y};
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hOut, pos);
}

//隐藏光标位置
void HideCursor()
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cci;
    GetConsoleCursorInfo(hOut, &cci);
    cci.bVisible = FALSE; // 隐藏光标
    SetConsoleCursorInfo(hOut, &cci);
}

void DrawGame()
{
	//system("cls");
	//绘制上边界
	SetConsolePos(0,0);
	for(int i=0;i<WIDTH+2;i++)
	{
		printf("#");
	}
	//printf("\n");
	
	for(int y=0;y<HEIGHT;y++)
	{
		SetConsolePos(0,y+1);
		for(int x=0;x<WIDTH;x++)
		{
			//绘制左边界
			if(x==0)
			{
				printf("#");
			}
			
			//绘制蛇头
			if(x==snake[0].x &&y==snake[0].y )
			{
				printf("O");
			}
			
			//绘制蛇身
			else
			{
				int isSnakeBody=0;
				for(int i=1;i<snakeLen;i++)
				{
					if(x==snake[i].x &&y==snake[i].y )
					{
						printf("o");
						isSnakeBody=1;
						break;
					}
				}
				//绘制食物
				if(!isSnakeBody&&x==foodX&&y==foodY)
				{
					printf("F");
				}
				//绘制空白区域
				else if(!isSnakeBody)
				{
					printf(" ");
				}
			}
			
			//绘制右边界
			if(x==WIDTH-1)
			{
				printf("#");
			}
		}
	}
	//绘制下边界
	SetConsolePos(0,HEIGHT+1);
	for(int i=0;i<WIDTH+2;i++)
	{
		printf("#");
	}
	
	//绘制得分信息
	SetConsolePos(0,HEIGHT+2);
	printf("得分:%d              ",score);
	SetConsolePos(0,HEIGHT+3);
	printf("操作：W（上）S（下）A（左）D（右）ESC（退出）\n");
}

//控制蛇的方向（无阻塞输入）
void ControlSnake()
{
	//_kbhit:检测是否有键盘输入，有则返回非0，无则返回0
	if(_kbhit())
	{
		switch(_getch())
		//—_getch:作用：读取你按下的任意一个按键（包括方向键、字母键、ESC 键），且不在控制台显示这个按键（无回显）
		/*对比普通的 scanf()：scanf() 需要按回车才确认输入，还会显示你按的键
		_getch() 按下去就立刻读取，屏幕上看不到，特别适合做游戏的 “即时按键响应”。
		头文件：<conio.h>*/
		{
			case 'w':
			case 'W':
				if(direction!='s')
				{
					direction='w';
				}
				break;
			case 's':
			case 'S':
				if(direction!='w')
				{
					direction='s';
				}
				break;
			case 'a':
			case 'A':
				if(direction!='d')
				{
					direction='a';
				}
				break;
			case 'd':
			case 'D':
				if(direction!='a')
				{
					direction='d';
				}
				break;
			case 27://ESC键的ASCII是27
				gameOver=1;
				break;
		}
	}
}

void MoveSnake()
{
	//1.保留蛇尾坐标（用于判断是否吃到东西）
	int lastX=snake[snakeLen-1].x ;
	int lastY=snake[snakeLen-1].y ;
	
	//蛇身移动方式
	for(int i=snakeLen-1;i>0;i--)
	{
		snake[i].x =snake[i-1].x ;
		snake[i].y =snake[i-1].y ;
	}
	
	//蛇头移动
	switch(direction)
	{
		case 'w':
			snake[0].y--;
			break;
		case 's':
			snake[0].y++;
		    break;
		case 'a':
			snake[0].x--;
			break;
		case 'd':
			snake[0].x++;
			break;
	}
	//检测撞墙
	if(snake[0].x <0||snake[0].x >=WIDTH||snake[0].y <0||snake[0].y >=HEIGHT)
	{
		gameOver=1;
		return;
	}
	
	//检测吃自己
	for(int i=1;i<snakeLen;i++)
	{
		if(snake[0].x ==snake[i].x &&snake[0].y ==snake[i].y )
		{
			gameOver=1;
			return;
		}
	}
	
	//检测是否吃到食物
	if(snake[0].x ==foodX&&snake[0].y ==foodY)
	{
		score+=10;
		snakeLen++;
		snake[snakeLen-1].x =lastX;
		snake[snakeLen-1].y =lastY;
		GenerateFood();//再次生成食物
	}
}

//生成食物
void GenerateFood()
{
	srand((unsigned int)time(NULL));//设置随机数种子
	
	//循环生成，直到找到不与蛇身重合的位置
	while(1)
	{
		foodX=rand()%WIDTH;
		foodY=rand()%HEIGHT;
		
		int isOverlap=0;
		for(int i=0;i<snakeLen;i++)
		{
			if(foodX==snake[i].x &&foodY==snake[i].y )
			{
				isOverlap=1;
				break;
			}
		}
		if(!isOverlap)
		{
			break;
		}
	}
}
