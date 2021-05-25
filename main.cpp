//need easyx file and loaddown graphic repository
/*
* 添加头文件
*/
#include "cstdio"
#include "cstdlib"
#include "conio.h"
//加载图形库文件
#include "graphics.h"//先下载easyx
//加载音乐
#include "mmsystem.h"
#pragma comment(lib,"winmm.lib")
#include "windows.h"
//C++ 输入输出流头文件
#include "iostream"
//使用时间函数的头文件
#include "ctime"
/*
* 控制窗口宽高
*/
const int WIDe = 820;
const int HIGh = 422;
const int WIDE1 = 750;
const int HIGH1 = 530;
const int WIDE2 = 700;
const int HIGH2 = 700;
const int WIDE3 = 780;
const int HIGH3 = 390;
const int WIDE4 = 700;
const int HIGH4 = 350;
/*
* 定义
*/
//定义系统运行时间变量
clock_t repro[6],jp;
//定义 一些图片的变量
IMAGE bk0,bk1,bk2,bk3,bk4,wall;
IMAGE apple, apple0, eye;
IMAGE die,money;
//定义 分数,输入关卡及输入答案
int count0=0,num,answer;
//通过枚举 定义方向
enum DIRECT { up, down, left, right };
/*
* 定义蛇和食物的属性
*/
struct SNAKE
{
	int size;//长度
	int direction;//方向
	int speed;//速度
	POINT axis[1000];//每一节的坐标
}snake;
struct FOOD
{
	POINT coor[6];//食物的坐标
	bool flag[6] = { 0 };//判断是否有食物的标志 0=无 1=有
}food;
/*
* 播放音乐
*/
void start_music()//打开游戏音乐
{
	mciSendString("open ./timi.mp3 alias music0", 0, 0, 0);//打开音乐文件,并且重命名
	mciSendString("play music0", 0, 0, 0);//播放音乐
	Sleep(1.13*1000);//进行音乐播放时间控制
	mciSendString("close music0", 0, 0, 0);//关闭音乐
}
void eat_music()//吃掉食物的声音
{
	mciSendString("open ./faded.mp3 alias music1", 0, 0, 0);//打开音乐文件,并且重命名
	mciSendString("play music1", 0, 0, 0);//播放音乐
	Sleep(1000*0.23);//进行音乐播放时间控制
	mciSendString("close music1", 0, 0, 0);//关闭音乐
}
void back_music_bigfish()//背景音乐
{
	mciSendString("open ./bigfish.mp3 alias music2", 0, 0, 0);
	mciSendString("play music2 repeat", 0, 0, 0);//重复播放音乐
}
void back_music_cherrytree()//背景音乐
{
	mciSendString("open ./cherrytree.mp3 alias music3", 0, 0, 0);
	mciSendString("play music3 repeat", 0, 0, 0);
}
void back_music_cherryblossom()//再次玩游戏背景音乐
{
	mciSendString("close music2", 0, 0, 0);
	mciSendString("close music3", 0, 0, 0);
	mciSendString("close music5", 0, 0, 0);
	mciSendString("close music6", 0, 0, 0);
	mciSendString("open ./cherryblossom.mp3 alias music4", 0, 0, 0);
	mciSendString("play music4 repeat", 0, 0, 0);
}
void back_music_moonglow()//背景音乐
{
	mciSendString("open ./moonglow.mp3 alias music5", 0, 0, 0);
	mciSendString("play music5 repeat", 0, 0, 0);
}
void back_music_sun()//背景音乐
{
	mciSendString("open ./sun.mp3 alias music6", 0, 0, 0);
	mciSendString("play music6 repeat", 0, 0, 0);
}
void die_music()//死亡音乐
{
	mciSendString("close music2", 0, 0, 0);
	mciSendString("close music3", 0, 0, 0);
	mciSendString("close music4", 0, 0, 0);
	mciSendString("close music5", 0, 0, 0);
	mciSendString("close music6", 0, 0, 0);
	mciSendString("open ./die.mp3 alias music8", 0, 0, 0);
	mciSendString("play music8", 0, 0, 0);
}
/*
* 打开游戏的最初界面
*/
void init_game0()
{
	system("mode con lines=12  cols=36");//控制"console"窗口大小
	initgraph(WIDe,HIGh,SHOWCONSOLE);//控制图形窗口大小,并且同时显示"console"窗口
	loadimage(&bk0,"./snake.jpg");//加载图形窗口中的图片
	putimage(0,0,&bk0);//在图形窗口中显示图片
	system("color 75");//控制"console"窗口的前景色和背景色
	//在图形窗口上输出文字
    RECT r = {0,HIGh/2,WIDe,HIGh};
	drawtext(_T("please finish SETTING first"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);//输出文字
	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = 48;//控制文字高度
	_tcscpy_s(f.lfFaceName, _T("黑体"));//选择字体
	f.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&f);
	outtextxy(0, 50, _T("Snake Game"));//输出文字
	for (clock_t &x:repro)//对所有时间变量进行获取当前程序运行系统时间
	{
		x = clock();
	}
}
/*
* 对每局游戏中的元素进行初始化
*/
void init_game1()
{
	initgraph(WIDE1, HIGH1, SHOWCONSOLE);//控制图形窗口的大小,同时显示"console"窗口
	loadimage(&bk1, "./background(1).jpg");
	putimage(0, 0, &bk1);
	//加载无背景的苹果图片
	loadimage(&apple, "./apple.jpg");
	loadimage(&apple0, "./apple0.jpg");
	loadimage(&eye, "./eye.jpg");//加载蛇的眼睛
	//初始化蛇
	snake.size = 4;//4节的长度
	snake.direction = down;//方向向下
	for (int i=0;i<snake.size;i++)//显示初始的蛇
	{
		snake.axis[i].x = 10 ;
		snake.axis[i].y = 50-10*i ;
	}
}
void init_game2()
{
	initgraph(WIDE2, HIGH2, SHOWCONSOLE);
	loadimage(&bk2, "./background(2).jpg");
	putimage(0, 0, &bk2);
	loadimage(&apple, "./apple.jpg");
	loadimage(&apple0, "./apple0.jpg");
	loadimage(&eye, "./eye.jpg");
	snake.size = 4;
	snake.direction = down;
	for (int i = 0; i < snake.size; i++)
	{
		snake.axis[i].x = 10;
		snake.axis[i].y = 50 - 10 * i;
	}
}
void init_game3()
{
	initgraph(WIDE3, HIGH3, SHOWCONSOLE);
	loadimage(&bk3, "./background(3).jpg");
	putimage(0, 0, &bk3);
	loadimage(&apple, "./apple.jpg");
	loadimage(&apple0, "./apple0.jpg");
	loadimage(&eye, "./eye.jpg");
	snake.size = 4;
	snake.direction = down;
	for (int i = 0; i < snake.size; i++)
	{
		snake.axis[i].x = 10;
		snake.axis[i].y = 50 - 10 * i;
	}
}
void init_game4()
{
	initgraph(WIDE4, HIGH4, SHOWCONSOLE);
	loadimage(&bk4, "./background(4).jpg");
	putimage(0, 0, &bk4);
	loadimage(&apple, "./apple.jpg");
	loadimage(&apple0, "./apple0.jpg");
	loadimage(&eye, "./eye.jpg");
	snake.size = 4;
	snake.direction = down;
	for (int i = 0; i < snake.size; i++)
	{
		snake.axis[i].x = 10;
		snake.axis[i].y = 50 - 10 * i;
	}
}
/*
* 对每局游戏进行初始化界面
*/
void show_wall1()
{
	BeginBatchDraw();//进行图形界面的一起显示,防止一卡一卡的
	loadimage(&wall, "./wall.jpg");//加载墙的图片
	//限定墙的坐标,并显示墙
	for (int i=1;i<=15;i++)
	{
		putimage(60, 0+10*(i-1), &wall);
	}
	for (int i = 1; i <= 40; i++)
	{
		putimage(550, 0 + 10 * (i - 1), &wall);
	}
	for (int i = 1; i <= 35; i++)
	{
		putimage(0+10*(i-1),190, &wall);
	}
	for (int i = 1; i <= 25; i++)
	{
		putimage(100, 530-10*i, &wall);
	}
	for (int i = 1; i <= 50; i++)
	{
		putimage(750-10*i, 400, &wall);
	}
	EndBatchDraw();//结束函数效果
}
void show_wall2()
{
	BeginBatchDraw();
	loadimage(&wall, "./wall.jpg");
	for (int i = 1; i <= 30; i++)
	{
		putimage(0+10 * (i-1), 120, &wall);
	}
	for (int i = 1; i <= 20; i++)
	{
		putimage(500, 10*(i-1), &wall);
	}
	for (int i = 1; i <= 20; i++)
	{
		putimage(320+10*(i-1), 300, &wall);
	}
	for (int i = 1; i <= 10; i++)
	{
		putimage(0+10 * (i-1), 650, &wall);
	}
	for (int i = 1; i <= 10; i++)
	{
		putimage(700, 160+10*(i-1), &wall);
	}
	EndBatchDraw();
}
void show_wall3()
{
	BeginBatchDraw();
	loadimage(&wall, "./wall.jpg");
	for (int i = 1; i < 10; i++)
	{
		putimage(0+10*(i-1), 200, &wall);
	}
	for (int i = 1; i < 15; i++)
	{
		putimage(250, 200+10*(i-1), &wall);
	}
	for (int i = 1; i < 15; i++)
	{
		putimage(250+10*(i-1), 100, &wall);
	}
	for (int i = 1; i < 20; i++)
	{
		putimage(200+10*(i-1), 240, &wall);
	}
	for (int i = 1; i < 25; i++)
	{
		putimage(400, 0+10*(i-1), &wall);
	}
	for (int i = 1; i < 15; i++)
	{
		putimage(460, 100+10*(i-1), &wall);
	}
	for (int i = 1; i < 10; i++)
	{
		putimage(680+10*(i-1), 280, &wall);
	}
	for (int i = 1; i < 5; i++)
	{
		putimage(770,0+10*(i-1), &wall);
	}
	EndBatchDraw();
}
void show_wall4()
{
	BeginBatchDraw();
	loadimage(&wall, "./wall.jpg");
	for (int i = 1; i < 30; i++)
	{
		putimage(0+10*(i-1), 160, &wall);
	}
	for (int i = 1; i < 20; i++)
	{
		putimage(150, 150+10*(i-1), &wall);
	}
	for (int i = 1; i < 10; i++)
	{
		putimage(290, 170+10*(i-1), &wall);
	}
	for (int i = 1; i < 30; i++)
	{
		putimage(400, 0+10*(i-1), &wall);
	}
	for (int i = 1; i < 30; i++)
	{
		putimage(410+10*(i-1), 290, &wall);
	}
	for (int i = 1; i < 10; i++)
	{
		putimage(600 + 10 * (i-1), 140, &wall);
	}
	EndBatchDraw();
}
/*
* 产生食物,并计分
*/
void show_food()
{
	BeginBatchDraw();
	//产生6个食物,为不同坐标
	for (int i=0;i<6;i++)
	{
		//显示无背景的苹果图片
		putimage(food.coor[i].x, food.coor[i].y, &apple0, NOTSRCERASE);
		putimage(food.coor[i].x, food.coor[i].y, &apple,SRCINVERT);
	}
	EndBatchDraw();
}
void eat_food()
{
	if (food.flag[0]&& 
		snake.axis[0].x >= food.coor[0].x-5 && snake.axis[0].x <= food.coor[0].x + 15 && 
		snake.axis[0].y >= food.coor[0].y-5 && snake.axis[0].y <= food.coor[0].y + 15 )//判断蛇的头是否碰到食物
	{
		food.flag[0] = false;//将食物的存在标志设置为0
		repro[0] = clock();//对食物时间进行调整
		snake.size++;//增加蛇的长度
		eat_music();//播放吃掉食物后的音乐
		count0++;//进行计分
		system("cls");//清除"console"窗口的内容
		printf("Your score: %d",count0);//显示分数
	}
	if (food.flag[1] &&
		snake.axis[0].x >= food.coor[1].x - 5 && snake.axis[0].x <= food.coor[1].x + 15 &&
		snake.axis[0].y >= food.coor[1].y - 5 && snake.axis[0].y <= food.coor[1].y + 15)
	{
		food.flag[1] = false;
		repro[1] = clock();
		snake.size++;
		eat_music();
		count0++;
		system("cls");
		printf("Your score: %d", count0);
	}
	if (food.flag[2] &&
		snake.axis[0].x >= food.coor[2].x - 5 && snake.axis[0].x <= food.coor[2].x + 15 &&
		snake.axis[0].y >= food.coor[2].y - 5 && snake.axis[0].y <= food.coor[2].y + 15)
	{
		food.flag[2] = false;
		repro[2] = clock();
		snake.size++;
		eat_music();
		count0++;
		system("cls");
		printf("Your score: %d", count0);
	}
	if (food.flag[3] &&
		snake.axis[0].x >= food.coor[3].x - 5 && snake.axis[0].x <= food.coor[3].x + 15 &&
		snake.axis[0].y >= food.coor[3].y - 5 && snake.axis[0].y <= food.coor[3].y + 15)
	{
		food.flag[3] = false;
		repro[3] = clock();
		snake.size++;
		eat_music();
		count0++;
		system("cls");
		printf("Your score: %d", count0);
	}
	if (food.flag[4] &&
		snake.axis[0].x >= food.coor[4].x - 5 && snake.axis[0].x <= food.coor[4].x + 15 &&
		snake.axis[0].y >= food.coor[4].y - 5 && snake.axis[0].y <= food.coor[4].y + 15)
	{
		food.flag[4] = false;
		repro[4] = clock();
		snake.size++;
		eat_music();
		count0++;
		system("cls");
		printf("Your score: %d", count0);
	}
	if (food.flag[5] &&
		snake.axis[0].x >= food.coor[5].x - 5 && snake.axis[0].x <= food.coor[5].x + 15 &&
		snake.axis[0].y >= food.coor[5].y - 5 && snake.axis[0].y <= food.coor[5].y + 15)
	{
		food.flag[5] = false;
		repro[5] = clock();
		snake.size++;
		eat_music();
		count0++;
		system("cls");
		printf("Your score: %d", count0);
	}
}
void product_food1()
{
	//如果没有食物就进入,随机产生食物
	if (!food.flag[0])
	{
		//随机产生食物
		food.coor[0].x = rand() % (WIDE1 - 4) + 5;
		food.coor[0].y = rand() % (HIGH1 - 4) + 5;
		food.flag[0] = true;
		repro[0] = clock();//对食物时间进行调整
	}
	if (!food.flag[1])
	{
		food.coor[1].x = rand() % (WIDE1 - 4) + 5;
		food.coor[1].y = rand() % (HIGH1 - 4) + 5;
		food.flag[1] = true;
		repro[1] = clock();
	}
	if (!food.flag[2])
	{
		food.coor[2].x = rand() % (WIDE1 - 4) + 5;
		food.coor[2].y = rand() % (HIGH1 - 4) + 5;
		food.flag[2] = true;
		repro[2] = clock();
	}
	if (!food.flag[3])
	{
		food.coor[3].x = rand() % (WIDE1 - 4) + 5;
		food.coor[3].y = rand() % (HIGH1 - 4) + 5;
		food.flag[3] = true;
		repro[3] = clock();
	}
	if (!food.flag[4])
	{
		food.coor[4].x = rand() % (WIDE1 - 4) + 5;
		food.coor[4].y = rand() % (HIGH1 - 4) + 5;
		food.flag[4] = true;
		repro[4] = clock();
	}
	if (!food.flag[5])
	{
		food.coor[5].x = rand() % (WIDE1 - 4) + 5;
		food.coor[5].y = rand() % (HIGH1 - 4) + 5;
		food.flag[5] = true;
		repro[5] = clock();
	}
}
void product_food2()
{
	if (!food.flag[0])
	{
		food.coor[0].x = rand() % (WIDE2 - 4) + 5;
		food.coor[0].y = rand() % (HIGH2 - 4) + 5;
		food.flag[0] = true;
		repro[0] = clock();
	}
	if (!food.flag[1])
	{
		food.coor[1].x = rand() % (WIDE2 - 4) + 5;
		food.coor[1].y = rand() % (HIGH2 - 4) + 5;
		food.flag[1] = true;
		repro[1] = clock();
	}
	if (!food.flag[2])
	{
		food.coor[2].x = rand() % (WIDE2 - 4) + 5;
		food.coor[2].y = rand() % (HIGH2 - 4) + 5;
		food.flag[2] = true;
		repro[2] = clock();
	}
	if (!food.flag[3])
	{
		food.coor[3].x = rand() % (WIDE2 - 4) + 5;
		food.coor[3].y = rand() % (HIGH2 - 4) + 5;
		food.flag[3] = true;
		repro[3] = clock();
	}
	if (!food.flag[4])
	{
		food.coor[4].x = rand() % (WIDE2 - 4) + 5;
		food.coor[4].y = rand() % (HIGH2 - 4) + 5;
		food.flag[4] = true;
		repro[4] = clock();
	}
	if (!food.flag[5])
	{
		food.coor[5].x = rand() % (WIDE2 - 4) + 5;
		food.coor[5].y = rand() % (HIGH2 - 4) + 5;
		food.flag[5] = true;
		repro[5] = clock();
	}
}
void product_food3()
{
	if (!food.flag[0])
	{
		food.coor[0].x = rand() % (WIDE3 - 4) + 5;
		food.coor[0].y = rand() % (HIGH3 - 4) + 5;
		food.flag[0] = true;
		repro[0] = clock();
	}
	if (!food.flag[1])
	{
		food.coor[1].x = rand() % (WIDE3 - 4) + 5;
		food.coor[1].y = rand() % (HIGH3 - 4) + 5;
		food.flag[1] = true;
		repro[1] = clock();
	}
	if (!food.flag[2])
	{
		food.coor[2].x = rand() % (WIDE3 - 4) + 5;
		food.coor[2].y = rand() % (HIGH3 - 4) + 5;
		food.flag[2] = true;
		repro[2] = clock();
	}
	if (!food.flag[3])
	{
		food.coor[3].x = rand() % (WIDE3 - 4) + 5;
		food.coor[3].y = rand() % (HIGH3 - 4) + 5;
		food.flag[3] = true;
		repro[3] = clock();
	}
	if (!food.flag[4])
	{
		food.coor[4].x = rand() % (WIDE3 - 4) + 5;
		food.coor[4].y = rand() % (HIGH3 - 4) + 5;
		food.flag[4] = true;
		repro[4] = clock();
	}
	if (!food.flag[5])
	{
		food.coor[5].x = rand() % (WIDE3 - 4) + 5;
		food.coor[5].y = rand() % (HIGH3 - 4) + 5;
		food.flag[5] = true;
		repro[5] = clock();
	}
}
void product_food4()
{
	if (!food.flag[0])
	{
		food.coor[0].x = rand() % (WIDE4 - 4) + 5;
		food.coor[0].y = rand() % (HIGH4 - 4) + 5;
		food.flag[0] = true;
		repro[0] = clock();
	}
	if (!food.flag[1])
	{
		food.coor[1].x = rand() % (WIDE4 - 4) + 5;
		food.coor[1].y = rand() % (HIGH4 - 4) + 5;
		food.flag[1] = true;
		repro[1] = clock();
	}
	if (!food.flag[2])
	{
		food.coor[2].x = rand() % (WIDE4 - 4) + 5;
		food.coor[2].y = rand() % (HIGH4 - 4) + 5;
		food.flag[2] = true;
		repro[2] = clock();
	}
	if (!food.flag[3])
	{
		food.coor[3].x = rand() % (WIDE4 - 4) + 5;
		food.coor[3].y = rand() % (HIGH4 - 4) + 5;
		food.flag[3] = true;
		repro[3] = clock();
	}
	if (!food.flag[4])
	{
		food.coor[4].x = rand() % (WIDE4 - 4) + 5;
		food.coor[4].y = rand() % (HIGH4 - 4) + 5;
		food.flag[4] = true;
		repro[4] = clock();
	}
	if (!food.flag[5])
	{
		food.coor[5].x = rand() % (WIDE4 - 4) + 5;
		food.coor[5].y = rand() % (HIGH4 - 4) + 5;
		food.flag[5] = true;
		repro[5] = clock();
	}
}
/*
* 展示蛇的眼睛
*/
void show_eye()
{
	switch (snake.direction)
	{
	case left://如果是向左,就对蛇的眼睛进行向左的调整
		putimage(snake.axis[0].x - 1, snake.axis[0].y - 7, &eye);
		putimage(snake.axis[0].x - 1, snake.axis[0].y + 3, &eye);
		break;
	case right:
		putimage(snake.axis[0].x + 1, snake.axis[0].y - 7, &eye);
		putimage(snake.axis[0].x + 1, snake.axis[0].y + 3, &eye);
		break;
	case up:
		putimage(snake.axis[0].x - 7, snake.axis[0].y - 1, &eye);
		putimage(snake.axis[0].x + 3, snake.axis[0].y - 1, &eye);
		break;
	case down:
		putimage(snake.axis[0].x - 7, snake.axis[0].y + 1, &eye);
		putimage(snake.axis[0].x + 3, snake.axis[0].y + 1, &eye);
		break;
	}
}
/*
* 进行死亡判断
*/
void judge_die()
{
	switch (num)
	{
	case 1:
		if ((snake.axis[0].x >= 60 && snake.axis[0].x <= 70 && snake.axis[0].y >= 0 && snake.axis[0].y <= 150)
			|| (snake.axis[0].x >= 550 && snake.axis[0].x <= 560 && snake.axis[0].y >= 0 && snake.axis[0].y <= 400)
			|| (snake.axis[0].x >= 0 && snake.axis[0].x <= 350 && snake.axis[0].y >= 190 && snake.axis[0].y <= 200)
			|| (snake.axis[0].x >= 100 && snake.axis[0].x <= 110 && snake.axis[0].y >= 280 && snake.axis[0].y <= 530)
			|| (snake.axis[0].x >= 250 && snake.axis[0].x <= 750 && snake.axis[0].y >= 400 && snake.axis[0].y <= 410)
			)//进行蛇是否触碰到墙的判断
		{
			mciSendString("close music5", 0, 0, 0);//死亡后,关闭该关卡的背景音乐
			system("cls");
			printf("You have die!!!");//进行说明
			putchar('\n');
			while (1)//对不输入"Y","y","N","n" 进行无效输入
			{
				printf("Do you want to start new game???(Y / N)");//进行询问
			end1:
				answer = _getch();//得到使用者的键盘输入
				if (answer != 'y' && answer != 'Y' && answer != 'n' && answer != 'N')//对不输入"Y","y","N","n" 进行无效输入
					goto end1;
				if (answer == 'y' || answer == 'n' || answer == 'Y' || answer == 'N')//对输入正确的符号,进行下一步操作
					break;
			}
		}
		break;
	case 2:
		if ((snake.axis[0].x >= 0 && snake.axis[0].x <= 300 && snake.axis[0].y >= 120 && snake.axis[0].y <= 130)
			|| (snake.axis[0].x >= 500 && snake.axis[0].x <= 510 && snake.axis[0].y >= 0 && snake.axis[0].y <= 200)
			|| (snake.axis[0].x >= 700 && snake.axis[0].x <= 710 && snake.axis[0].y >= 160 && snake.axis[0].y <= 260)
			|| (snake.axis[0].x >= 0 && snake.axis[0].x <= 100 && snake.axis[0].y >= 650 && snake.axis[0].y <= 660)
			|| (snake.axis[0].x >= 320 && snake.axis[0].x <= 520 && snake.axis[0].y >= 300 && snake.axis[0].y <= 310)
			)
		{
			mciSendString("close music2", 0, 0, 0);
			system("cls");
			printf("You have die!!!");
			putchar('\n');
			while (1)
			{
				printf("Do you want to start new game???(Y / N)");
			end2:
				answer = _getch();
				if (answer != 'y' && answer != 'Y' && answer != 'n' && answer != 'N')
					goto end2;
				if (answer == 'y' || answer == 'n' || answer == 'Y' || answer == 'N')
					break;
			}
		}
		break;
	case 3:
		if ((snake.axis[0].x >= 0 && snake.axis[0].x <= 100 && snake.axis[0].y >= 200 && snake.axis[0].y <= 210)
			|| (snake.axis[0].x >= 250 && snake.axis[0].x <= 260 && snake.axis[0].y >= 200 && snake.axis[0].y <= 350)
			|| (snake.axis[0].x >= 250 && snake.axis[0].x <= 400 && snake.axis[0].y >= 100 && snake.axis[0].y <= 110)
			|| (snake.axis[0].x >= 200 && snake.axis[0].x <= 400 && snake.axis[0].y >= 240 && snake.axis[0].y <= 250)
			|| (snake.axis[0].x >= 400 && snake.axis[0].x <= 410 && snake.axis[0].y >= 0 && snake.axis[0].y <= 250)
			|| (snake.axis[0].x >= 460 && snake.axis[0].x <= 470 && snake.axis[0].y >= 100 && snake.axis[0].y <= 250)
			|| (snake.axis[0].x >= 770 && snake.axis[0].x <= 780 && snake.axis[0].y >= 0 && snake.axis[0].y <= 50)
			|| (snake.axis[0].x >= 680 && snake.axis[0].x <= 780 && snake.axis[0].y >= 280 && snake.axis[0].y <= 290)
			)
		{
			mciSendString("close music6", 0, 0, 0);
			system("cls");
			printf("You have die!!!");
			putchar('\n');
			while (1)
			{
				printf("Do you want to start new game???(Y / N)");
			end3:
				answer = _getch();
				if (answer != 'y' && answer != 'Y' && answer != 'n' && answer != 'N')
					goto end3;
				if (answer == 'y' || answer == 'n' || answer == 'Y' || answer == 'N')
					break;
			}
		}
		break;
	case 4:
		if ((snake.axis[0].x >= 0 && snake.axis[0].x <= 300 && snake.axis[0].y >= 160 && snake.axis[0].y <= 170)
			|| (snake.axis[0].x >= 150 && snake.axis[0].x <= 160 && snake.axis[0].y >= 150 && snake.axis[0].y <= 350)
			|| (snake.axis[0].x >= 290 && snake.axis[0].x <= 300 && snake.axis[0].y >= 170 && snake.axis[0].y <= 270)
			|| (snake.axis[0].x >= 400 && snake.axis[0].x <= 410 && snake.axis[0].y >= 0 && snake.axis[0].y <= 300)
			|| (snake.axis[0].x >= 410 && snake.axis[0].x <= 510 && snake.axis[0].y >= 290 && snake.axis[0].y <= 300)
			|| (snake.axis[0].x >= 600 && snake.axis[0].x <= 700 && snake.axis[0].y >= 140 && snake.axis[0].y <= 150)
			)
		{
			mciSendString("close music3", 0, 0, 0);
			system("cls");
			printf("You have die!!!");
			putchar('\n');
			while (1)
			{
				printf("Do you want to start new game???(Y / N)");
			end4:
				answer = _getch();
				if (answer != 'y' && answer != 'Y' && answer != 'n' && answer != 'N')
					goto end4;
				if (answer == 'y' || answer == 'n' || answer == 'Y' || answer == 'N')
					break;
			}
		}
		break;
	}
}
/*
* 4个关卡
*/
void start_game1()
{
	//进行函数的声明
	void show_snake1();
	void move_snake();
	void direction_snake1();
	void key_input();
	init_game1();//初始化游戏
	back_music_moonglow();//播放背景音乐
	while (1)//确保贪吃蛇的连续自动运行,进入死循环
	{
		show_snake1();
		show_wall1();
		show_food();
		show_eye();
		key_input();
		Sleep(250-snake.speed*2.5);//对蛇进行速度控制
		move_snake();
		direction_snake1();
		eat_food();
		product_food1();
		key_input();
		judge_die();
		jp = clock();//获取当前时间
		for (int i=0;i<6;i++)//对未吃食物进行时间检查
		{
			if (jp - repro[i] >= 25 * CLOCKS_PER_SEC &&
				((snake.axis[0].x > food.coor[i].x + 100 || snake.axis[0].x < food.coor[i].x - 100) &&
					(snake.axis[0].y > food.coor[i].y - 100 || snake.axis[0].y < food.coor[i].y - 100)))
			{
				food.flag[i] = false;
			}
		}
		if (answer == 'y' || answer == 'n' || answer == 'Y' || answer == 'N')//对输入正确的符号,进行下一步操作
			break;
	}
}
void start_game2()
{
	void show_snake2();
	void move_snake();
	void direction_snake2();
	void key_input();
	init_game2();
	back_music_bigfish();
	while (1)
	{
		show_snake2();
		show_wall2();
		show_food();
		show_eye();
		key_input();
		Sleep(250 - snake.speed * 2.5);
		move_snake();
		direction_snake2();
		eat_food();
		product_food2();
		key_input();
		judge_die();
		jp = clock();
		for (int i = 0; i < 6; i++)
		{
			if (jp - repro[i] >= 25 * CLOCKS_PER_SEC &&
				((snake.axis[0].x > food.coor[i].x + 100 || snake.axis[0].x < food.coor[i].x - 100) &&
					(snake.axis[0].y > food.coor[i].y - 100 || snake.axis[0].y < food.coor[i].y - 100)))
			{
				food.flag[i] = false;
			}
		}
		if (answer == 'y' || answer == 'n' || answer == 'Y' || answer == 'N')
			break;
	}
}
void start_game3()
{
	void show_snake3();
	void move_snake();
	void direction_snake3();
	void key_input();
	init_game3();
	back_music_sun();
	
	while (1)
	{
		show_snake3();
		show_wall3();
		show_food();
		show_eye();
		key_input();
		Sleep(250 - snake.speed * 2.5);
		move_snake();
		direction_snake3();
		eat_food();
		product_food3();
		key_input();
		judge_die();
		jp = clock();
		for (int i = 0; i < 6; i++)
		{
			if (jp - repro[i] >= 25 * CLOCKS_PER_SEC &&
				((snake.axis[0].x > food.coor[i].x + 100 || snake.axis[0].x < food.coor[i].x - 100) &&
					(snake.axis[0].y > food.coor[i].y - 100 || snake.axis[0].y < food.coor[i].y - 100)))
			{
				food.flag[i] = false;
			}
		}
		if (answer == 'y' || answer == 'n' || answer == 'Y' || answer == 'N')
			break;
	}
}
void start_game4()
{
	void show_snake4();
	void move_snake();
	void direction_snake4();
	void key_input();
	init_game4();
	back_music_cherrytree();
	while (1)
	{
		show_snake4();
		show_wall4();
		show_food();
		show_eye();
		key_input();
		Sleep(250 - snake.speed * 2.5);
		move_snake();
		direction_snake4();
		eat_food();
		product_food4();
		key_input();
		judge_die();
		jp = clock();
		for (int i = 0; i < 6; i++)
		{
			if (jp - repro[i] >= 25 * CLOCKS_PER_SEC &&
				((snake.axis[0].x > food.coor[i].x + 100 || snake.axis[0].x < food.coor[i].x - 100) &&
					(snake.axis[0].y > food.coor[i].y - 100 || snake.axis[0].y < food.coor[i].y - 100)))
			{
				food.flag[i] = false;
			}
		}
		if (answer == 'y' || answer == 'n' || answer == 'Y' || answer == 'N')
			break;
	}
}
/*
* 主函数
*/
int main()
{
	using namespace std;//使用名称空间
	srand(GetTickCount());//使产生的随机数为真随机数
	start_music();//播放开始音乐
	init_game0();//初始游戏设置
	num = 0;//初始关卡
	count0 = 0;//初始分数
	//进行输入游戏设置的提示
	cout << "Now,Let's change some setting of the snake!" << endl
	<< "please input snake speed (1-100):";
	cin >> snake.speed;//输入蛇的速度
	cout << "please input customs pass(1--4): ";
	cin >> num;//输入关卡
    system("cls");
	cout << "Your score: 0";
	switch (num)//进行游戏关卡的选择
	{
	case 1:
		start_game1();//关卡1
		break;
	case 2:
		start_game2();//关卡2
		break;
	case 3:
		start_game3();//关卡3
		break;
	case 4:
		start_game4();//关卡4
		break;
	}
	//进行 yes or no 的判断
	if (answer == 'y' || answer == 'Y')//"Y"
	{
		initgraph(666,664);//控制图形窗口的大小
		loadimage(&money, "./money.jpg");//加载图片
		putimage(0, 0, &money);//显示图片
		back_music_cherryblossom();//播放YES的音乐
		while (1);//进入死循环,不让再次启动,除非重新RUN
	}
	else//"N"
	{
		initgraph(200, 200);//控制图形窗口的大小
		loadimage(&die, "./die.jpg");//加载图片
		putimage(0, 0, &die);//显示图片
		die_music();//播放死亡后的音乐
		while (1);//进入死循环,
	}
	return 0;
}
/*
* 在地图上显示蛇
*/
void show_snake1()
{
	BeginBatchDraw();
	cleardevice();//清空图形窗口内容
	loadimage(&bk1, "./background(1).jpg");
	putimage(0, 0, &bk1);
	setfillcolor(RGB(52,179,128));//设置图形窗口的前景色
	for (int i = 0; i < snake.size; i++)
	{
		solidcircle(snake.axis[i].x, snake.axis[i].y, 5);//以每一节的蛇的坐标为中心,画出圆
	}
	EndBatchDraw();
}
void show_snake2()
{
	BeginBatchDraw();
	cleardevice();
	loadimage(&bk2, "./background(2).jpg");
	putimage(0, 0, &bk2);
	setfillcolor(RGB(254, 240, 51));
	for (int i = 0; i < snake.size; i++)
	{
		solidcircle(snake.axis[i].x, snake.axis[i].y, 5);
	}
	EndBatchDraw();
}
void show_snake3()
{
	BeginBatchDraw();
	cleardevice();
	loadimage(&bk3, "./background(3).jpg");
	putimage(0, 0, &bk3);
	setfillcolor(RGB(186, 28, 34));
	for (int i = 0; i < snake.size; i++)
	{
		solidcircle(snake.axis[i].x, snake.axis[i].y, 5);
	}
	EndBatchDraw();
}
void show_snake4()
{
	BeginBatchDraw();
	cleardevice();
	loadimage(&bk4, "./background(4).jpg");
	putimage(0, 0, &bk4);
	setfillcolor(RGB(rand() % 256, rand() % 256, rand() % 100));
	for (int i = 0; i < snake.size; i++)
	{
		solidcircle(snake.axis[i].x, snake.axis[i].y, 5);
	}
	EndBatchDraw();
}
/*
* 蛇进行自动移动
*/
void move_snake()
{
	for (int i = snake.size - 1; i > 0; i--)
	{
		snake.axis[i] = snake.axis[i - 1];//使得后一节始终去占领前一节的坐标
	}
}
//控制蛇头的方向
void direction_snake1()
{
	switch (snake.direction)
	{
	case left:
		//使得蛇能够穿出一侧的边框,并且在对立侧出现
		if (snake.axis[0].x == 0)
			snake.axis[0].x = WIDE1;
		snake.axis[0].x -= 10;
		break;
	case right:
		if (snake.axis[0].x == WIDE1)
			snake.axis[0].x = 0;
		snake.axis[0].x += 10;
		break;
	case up:
		if (snake.axis[0].y == 0)
			snake.axis[0].y = HIGH1;
		snake.axis[0].y -= 10;
		break;
	case down:
		if (snake.axis[0].y == HIGH1)
			snake.axis[0].y = 0;
		snake.axis[0].y += 10;
		break;
	}
}
void direction_snake2()
{
	switch (snake.direction)
	{
	case left:
		if (snake.axis[0].x == 0)
			snake.axis[0].x = WIDE2;
		snake.axis[0].x -= 10;
		break;
	case right:
		if (snake.axis[0].x == WIDE2)
			snake.axis[0].x = 0;
		snake.axis[0].x += 10;
		break;
	case up:
		if (snake.axis[0].y == 0)
			snake.axis[0].y = HIGH2;
		snake.axis[0].y -= 10;
		break;
	case down:
		if (snake.axis[0].y == HIGH2)
			snake.axis[0].y = 0;
		snake.axis[0].y += 10;
		break;
	}
}
void direction_snake3()
{
	switch (snake.direction)
	{
	case left:
		if (snake.axis[0].x == 0)
			snake.axis[0].x = WIDE3;
		snake.axis[0].x -= 10;
		break;
	case right:
		if (snake.axis[0].x == WIDE3)
			snake.axis[0].x = 0;
		snake.axis[0].x += 10;
		break;
	case up:
		if (snake.axis[0].y == 0)
			snake.axis[0].y = HIGH3;
		snake.axis[0].y -= 10;
		break;
	case down:
		if (snake.axis[0].y == HIGH3)
			snake.axis[0].y = 0;
		snake.axis[0].y += 10;
		break;
	}
}
void direction_snake4()
{
	switch (snake.direction)
	{
	case left:
		if (snake.axis[0].x == 0)
			snake.axis[0].x = WIDE4;
		snake.axis[0].x -= 10;
		break;
	case right:
		if (snake.axis[0].x == WIDE4)
			snake.axis[0].x = 0;
		snake.axis[0].x += 10;
		break;
	case up:
		if (snake.axis[0].y == 0)
			snake.axis[0].y = HIGH4;
		snake.axis[0].y -= 10;
		break;
	case down:
		if (snake.axis[0].y == HIGH4)
			snake.axis[0].y = 0;
		snake.axis[0].y += 10;
		break;
	}
}
/*
* 获取键盘输入,进行方向变化
*/
void key_input()
{
	if (_kbhit())//判断是否有键盘输入
	{
		switch (_getch())//获取键盘的输入
		{
		//进行方向的控制
		case 75:
		case 'a':
		case 'A':
			if (snake.direction != right)
				snake.direction = left;
			break;
		case 77:
		case 'd':
		case 'D':
			if (snake.direction != left)
				snake.direction = right;
			break;
		case 72:
		case 'w':
		case 'W':
			if (snake.direction != down)
				snake.direction = up;
			break;
		case 80:
		case 's':
		case 'S':
			if (snake.direction != up)
				snake.direction = down;
			break;
		//进行蛇的暂停
		case ' ':
			while (1)
			{
				if (_kbhit())//通过按任意键来取消
					break;
		    }
				break;
		}
	}
}
