#include <graphics.h> //图形化界面
#include <malloc.h>//申请内存
#include <conio.h> //getche()
#include <string>//字符串操作
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <mmsystem.h>//媒体
#include <easyx.h>//图形化
#include <io.h>//缓冲区
#pragma comment(lib,"winmm.lib")//调用Windows自带媒体
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" ) // 设置入口地址

struct schedule
{
	char data[11];//日期
	char time[11];//时间
	char location[50];//地点
	char man[30];//人物
	char schedule[200];//日程
	struct schedule* next;
};

typedef struct schedule Node;//命名

Node* Create_LinkList()
{
	Node* head = (Node*)malloc(sizeof(Node));  //创建头节点
	if (head == NULL)   //创建失败,则返回
	{
		printf("内存分配失败!\n");
		return (NULL);
	}
	head->next = NULL; //头节点的指针域置NULL
	return head;
}

void Select_MainMenu(Node* head);

void ReadTxtFile(Node* head)//读取文件内容
{
	FILE* fp;

	char data[11] = {};
	char time[11] = {};
	char location[50] = {};
	char man[30] = {};
	char schedule[200] = {};

	Node* pnew;
	Node* tail;
	Node* p;


	fp = fopen("schedule.txt", "r");//读取文件

	p = head;

	while (p->next != NULL)//找到尾端
	{
		p = p->next;
	}

	while (fscanf(fp, "%s %s %s %s %s\n", data, time, location, man, schedule) > 0)//判断不为空
	{
		pnew = (Node*)malloc(sizeof(Node));//申请内存

		if (pnew == NULL)
		{
			printf("no enough memory!\n");
			return;
		}

		strcpy(pnew->data, data);//复制
		strcpy(pnew->time, time);
		strcpy(pnew->location, location);
		strcpy(pnew->man, man);
		strcpy(pnew->schedule, schedule);

		pnew->next = NULL;//尾插
		p->next = pnew;
		p = pnew;
	}

	fclose(fp);//关闭文件
}

void WriteToTxtFile(Node* head)//写入文件内容，保存
{
	IMAGE save;
	loadimage(&save, "save.png");//进入新建日程界面
	putimage(0, 0, &save);

	FILE* fp;
	Node* p;

	fp = fopen("schedule.txt", "w");//打开文件

	for (p = head->next; p != NULL; p = p->next)//写入
	{
		fputs(p->data, fp);
		fputs(" ", fp);
		fputs(p->time, fp);
		fputs(" ", fp);
		fputs(p->location, fp);
		fputs(" ", fp);
		fputs(p->man, fp);
		fputs(" ", fp);
		fputs(p->schedule, fp);
		fputs("\n", fp);
	}
	fclose(fp);//关闭文件

	MessageBox(NULL, _T("保存成功!"), _T("提醒窗口"), MB_OK | MB_SYSTEMMODAL);//保存成功
	Select_MainMenu(head);
}

void addSchedule(Node* head)
{
	IMAGE add;
	loadimage(&add, "add.png");//进入新建日程界面
	putimage(0, 0, &add);

	for (; head->next != NULL; head = head->next);//找到尾端

	Node* test = (Node*)malloc(sizeof(Node));//申请内存

	test->next = NULL;//尾结点

	InputBox(test->data, 11, _T("请输入日期:yyyy/mm/dd(例如:2018/01/09)"));//日期
	InputBox(test->time, 11, _T("请输入具体时间:hh:MM:ss(例如:23:01:00)"));//时间
	InputBox(test->location, 50, _T("请输入事件地点(最长为50个字符)"));//地点
	InputBox(test->man, 30, _T("请输入事件人物(最长为30个字符)"));//人物
	InputBox(test->schedule, 200, _T("请输入事件内容(最长为200个字符)"));//日程

	head->next = test;

	MessageBox(NULL, _T("添加成功!"), _T("提醒窗口"), MB_OK | MB_SYSTEMMODAL);//添加成功提醒
}

void displaySchedule(Node* head)
{
	IMAGE display;

	loadimage(&display, "display.png");//进入显示日程界面
	putimage(0, 0, &display);

	Node* p;

	int position_x = 443;//“开始”横坐标
	int position_y = 91;//“开始”纵坐标

	setbkmode(TRANSPARENT);//背景透明
	settextcolor(RGB(0, 0, 0));//设置字体为黑色
	settextstyle(20, 0, _T("华光粗圆_CNKI"));//设置字体大小20，格式华光粗圆_CNKI
	outtextxy(position_x, position_y, _T("日期                                    时间                                    地点                                    人物                                    日程"));//表头

	for (p = head->next; p != NULL; p = p->next)
	{
		position_y += 20;//向下推一行

		outtextxy(443, position_y, p->data);//日期
		outtextxy(621, position_y, p->time);//时间
		outtextxy(801, position_y, p->location);//地点
		outtextxy(977, position_y, p->man);//人物
		outtextxy(1157, position_y, p->schedule);//日程
	}
}

void deleteByDate(Node* head)//以日期来删除
{
	Node* before, * cur;

	char data[11];
	char time[11];

	InputBox(data, 11, _T("请输入删除的日期:yyyy/mm/dd(例如:2018/01/09)"));//日期
	InputBox(time, 11, _T("请输入删除的具体时间:hh:MM:ss(例如:23:01:00)"));//时间

	for (before = head, cur = head->next; cur != NULL; cur = cur->next, before = before->next)
	{
		if (strcmp(cur->data,data)==0 && strcmp(cur->time,time)==0)
		{
			before->next = cur->next;
			MessageBox(NULL, _T("删除成功！"), _T("删除提醒窗口"), MB_OK | MB_SYSTEMMODAL);//删除提醒窗口
			break;
		}
	}
	if (cur == NULL) 
	{
		MessageBox(NULL, _T("未找到该事件！"), _T("删除提醒窗口"), MB_OK | MB_SYSTEMMODAL);//删除提醒窗口
	}

}

void deleteBySchedule(Node* head)//以日程来删除
{
	Node* before, * cur;
	char temp[200];

	InputBox(temp, 200, _T("请输入删除事件内容(最长为200个字符)"));//日程

	for (before = head, cur = head->next; cur != NULL; before = before->next, cur = cur->next)//找结点
	{
		if (strcmp(temp, cur->schedule) == 0)//判断找到结点
		{
			before->next = cur->next;
			MessageBox(NULL, _T("删除成功！"), _T("删除提醒窗口"), MB_OK | MB_SYSTEMMODAL);//删除提醒窗口
			break;
		}
	}
	if (cur == NULL)
	{
		MessageBox(NULL, _T("未找到该事件！"), _T("删除提醒窗口"), MB_OK | MB_SYSTEMMODAL);//删除提醒窗口
	}
}

void Deletemenu(Node* head)//删除菜单
{
	IMAGE Delete;

	loadimage(&Delete, "delete.png");//进入显示日程界面
	putimage(0, 0, &Delete);

	MessageBox(NULL, _T("-------删除菜单-------\n        1.按时间删除\n        2.按事件来删除"), _T("删除提醒窗口"), MB_OK | MB_SYSTEMMODAL);//删除提醒窗口
	
	char a='0';
	char flag[2]={};

	InputBox(flag, 2, _T("请输入选择"));//选择

	if (strcmp(flag, "1")==0)
	{
		a = '1';
	}

	if (strcmp(flag, "2")==0)
	{
		a = '2';
	}

	switch (a)
	{
	case '1':deleteByDate(head); break;
	case '2':deleteBySchedule(head); break;
	default:MessageBox(NULL, _T("请重新输入"), _T("提醒窗口"), MB_OK | MB_SYSTEMMODAL);//提醒窗口
		break;
	}

}

void changeByDate(Node* head)//以日期来修改
{
	Node* before, * cur;

	char data[11];
	char time[11];

	char datac[11];
	char timec[11];

	char locationc[50];
	char manc[30];
	char schedulec[200];

	InputBox(data, 11, _T("请输入修改的日期:yyyy/mm/dd(例如:2018/01/09)"));//日期
	InputBox(time, 11, _T("请输入修改的具体时间:hh:MM:ss(例如:23:01:00)"));//时间


	for (before = head, cur = head->next; cur != NULL; cur = cur->next, before = before->next)
	{
		if (strcmp(cur->data, data) == 0 && strcmp(cur->time, time) == 0)
		{
			char see;

			do
			{
				MessageBox(NULL, _T("-------修改菜单-------\n        1.修改日期\n        2.修改日期\n        3.修改地点\n        4.修改人物\n        5.修改事件"), _T("修改提醒窗口"), MB_OK | MB_SYSTEMMODAL);//修改选择窗口

				char a = '0';
				char flag[2] = {};

				InputBox(flag, 2, _T("请输入选择:"));//选择

				if (strcmp(flag, "1") == 0)
				{
					a = '1';
				}
				if (strcmp(flag, "2") == 0)
				{
					a = '2';
				}
				if (strcmp(flag, "3") == 0)
				{
					a = '3';
				}
				if (strcmp(flag, "4") == 0)
				{
					a = '4';
				}if (strcmp(flag, "5") == 0)
				{
					a = '5';
				}

				switch (a)
				{
				case '1':InputBox(datac, 11, _T("请输入修改后的日期:yyyy/mm/dd(例如:2018/01/09)"));//日期
					strcpy(cur->data, datac);
					break;
				case '2':InputBox(timec, 11, _T("请输入修改后的具体时间:hh:MM:ss(例如:23:01:00)"));//时间
					strcpy(cur->time, timec);
					break;
				case '3':InputBox(locationc, 50, _T("请输入修改后的事件地点(最长为50个字符)"));//地点
					strcpy(cur->location, locationc);
					break;
				case '4':InputBox(manc, 30, _T("请输入修改后的事件人物(最长为30个字符)"));//人物
					strcpy(cur->man, manc);
					break;
				case '5':InputBox(schedulec, 200, _T("请输入修改后的事件内容(最长为200个字符)"));//日程
					strcpy(cur->schedule, schedulec);
					break;
				default:MessageBox(NULL, _T("请重新输入!"), _T("提醒窗口"), MB_OK | MB_SYSTEMMODAL);//提醒窗口
					break;
				}

				MessageBox(NULL, _T("修改成功！是否继续修改？\n继续(y)       退出(n)"), _T("修改提醒窗口"), MB_OK | MB_SYSTEMMODAL);//删除提醒窗口

				char b = '0';
				char choice[2] = {};

				InputBox(choice, 2, _T("请输入选择:"));//选择

				if (strcmp(choice, "y") == 0)
				{
					b = 'y';
				}
				if (strcmp(flag, "n") == 0)
				{
					b = 'n';
				}

				switch (b)
				{
				case 'y':see = 'y'; break;
				case 'n':see = 'n'; break;
				default:MessageBox(NULL, _T("输入错误!"), _T("提醒窗口"), MB_OK | MB_SYSTEMMODAL);//提醒窗口
				}
			} while (see == 'y');

			break;
		}
	}
	if (cur == NULL) 
	{
		MessageBox(NULL, _T("未找到该事件！"), _T("修改提醒窗口"), MB_OK | MB_SYSTEMMODAL);//删除提醒窗口
	}
}

void changeBySchedule(Node* head)//以日程来修改
{
	Node* before, * cur;
	char temp[200];

	char data[11];
	char time[11];

	char datac[11];
	char timec[11];

	char locationc[50];
	char manc[30];
	char schedulec[200];

	InputBox(temp, 200, _T("请输入修改事件内容(最长为200个字符)"));//日程

	for (before = head, cur = head->next; cur != NULL; before = before->next, cur = cur->next)
	{
		if (strcmp(temp, cur->schedule) == 0)
		{
			char see;

			do
			{
				MessageBox(NULL, _T("-------修改菜单-------\n        1.修改日期\n        2.修改日期\n        3.修改地点\n        4.修改人物\n        5.修改事件"), _T("修改提醒窗口"), MB_OK | MB_SYSTEMMODAL);//修改选择窗口

				char a = '0';
				char flag[2] = {};

				InputBox(flag, 2, _T("请输入选择:"));//选择

				if (strcmp(flag, "1") == 0)
				{
					a = '1';
				}
				if (strcmp(flag, "2") == 0)
				{
					a = '2';
				}
				if (strcmp(flag, "3") == 0)
				{
					a = '3';
				}
				if (strcmp(flag, "4") == 0)
				{
					a = '4';
				}if (strcmp(flag, "5") == 0)
				{
					a = '5';
				}

				switch (a)
				{
				case '1':InputBox(datac, 11, _T("请输入修改后的日期:yyyy/mm/dd(例如:2018/01/09)"));//日期
					strcpy(cur->data, datac);
					break;
				case '2':InputBox(timec, 11, _T("请输入修改后的具体时间:hh:MM:ss(例如:23:01:00)"));//时间
					strcpy(cur->time, timec);
					break;
				case '3':InputBox(locationc, 50, _T("请输入修改后的事件地点(最长为50个字符)"));//地点
					strcpy(cur->location, locationc);
					break;
				case '4':InputBox(manc, 30, _T("请输入修改后的事件人物(最长为30个字符)"));//人物
					strcpy(cur->man, manc);
					break;
				case '5':InputBox(schedulec, 200, _T("请输入修改后的事件内容(最长为200个字符)"));//日程
					strcpy(cur->schedule, schedulec);
					break;
				default:MessageBox(NULL, _T("请重新输入!"), _T("提醒窗口"), MB_OK | MB_SYSTEMMODAL);//提醒窗口
					break;
				}

				MessageBox(NULL, _T("修改成功！是否继续修改？\n继续(y)       退出(n)"), _T("修改提醒窗口"), MB_OK | MB_SYSTEMMODAL);//删除提醒窗口

				char b = '0';
				char choice[2] = {};

				InputBox(choice, 2, _T("请输入选择:"));//选择

				if (strcmp(choice, "y") == 0)
				{
					b = 'y';
				}
				if (strcmp(flag, "n") == 0)
				{
					b = 'n';
				}

				switch (b)
				{
				case 'y':see = 'y'; break;
				case 'n':see = 'n'; break;
				default:MessageBox(NULL, _T("输入错误!"), _T("提醒窗口"), MB_OK | MB_SYSTEMMODAL);//提醒窗口
				}
			} while (see=='y');
			
			break;
		}
	}
	if (cur == NULL)
	{
		MessageBox(NULL, _T("未找到该事件！"), _T("修改提醒窗口"), MB_OK | MB_SYSTEMMODAL);//删除提醒窗口
	}
}

void changemenu(Node* head)//修改菜单
{
	IMAGE change;

	loadimage(&change, "change.png");//进入修改日程界面
	putimage(0, 0, &change);

	MessageBox(NULL, _T("-------修改菜单-------\n        1.按时间修改\n        2.按事件来修改"), _T("修改提醒窗口"), MB_OK | MB_SYSTEMMODAL);//修改提醒窗口

	char a = '0';
	char flag[2] = {};

	InputBox(flag, 2, _T("请输入选择:"));//选择

	if (strcmp(flag, "1") == 0)
	{
		a = '1';
	}

	if (strcmp(flag, "2") == 0)
	{
		a = '2';
	}

	switch (a)
	{
	case '1':changeByDate(head); break;
	case '2':changeBySchedule(head); break;
	default:MessageBox(NULL, _T("请重新输入!"), _T("提醒窗口"), MB_OK | MB_SYSTEMMODAL);//提醒窗口
		break;
	}
}

int compare(Node* a, Node* b) //比较俩个事件的先后
{
	//将data和time连接起来,直接比较字符串的大小
	char a1[22] = {};
	char a2[22] = {};
	char b1[22] = {};
	char b2[22] = {};
	
	strcpy(a1, a->data);
	strcpy(a2, a->time);

	strcpy(b1, b->data);
	strcpy(b2, b->time);

	strcat(a1, a2);
	strcat(b1, b2);


	int n = strcmp(a1, b1);
	if (n > 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

Node* sort(Node* L) //事件排序//选择排序
{
	IMAGE sort;

	loadimage(&sort, "sort.png");//进入排序日程界面
	putimage(0, 0, &sort);

	Node* p, * q, * next;

	for (p = L->next; p->next != NULL; p = p->next)    /*每次循环都找出一个最小值，将最小值交换到第一位，然后将指针向后移动一位*/
	{
		next = p;
		for (q = p->next; q; q = q->next)    /*由前向后遍历，找出最小的节点*/
		{
			if (compare(next, q) == 1)//比较
				next = q;
		}
		if (next != p)//交换
		{
			char tem3[11];
			memcpy(tem3, p->data, 11);
			memcpy(p->data, next->data, 11);
			memcpy(next->data, tem3, 11);

			char tem4[11];
			memcpy(tem4, p->time, 11);
			memcpy(p->time, next->time, 11);
			memcpy(next->time, tem4, 11);

			char tem1[50];
			memcpy(tem1, p->location, 50);
			memcpy(p->location, next->location, 50);
			memcpy(next->location, tem1, 50);

			char tem2[30];
			memcpy(tem1, p->man, 30);
			memcpy(p->man, next->man, 30);
			memcpy(next->man, tem1, 30);

			char tem[200];
			memcpy(tem, p->schedule, 200);
			memcpy(p->schedule, next->schedule, 200);
			memcpy(next->schedule, tem, 200);
		}
	}
	MessageBox(NULL, _T("排序成功！"), _T("排序提醒窗口"), MB_OK | MB_SYSTEMMODAL);//排序提醒窗口
	return L;
}

void Welcome(Node* head);
void displaySchedule(Node* head);

void Select_MainMenu(Node* head)//主菜单
{
	IMAGE menu;
	IMAGE exit;
	loadimage(&menu, "fmenu.png");//导入初始界面
	putimage(0, 0, &menu);//显示初始界面
	MOUSEMSG msg;//定义变量，保存鼠标消息
	FlushMouseMsgBuffer();// 清空鼠标消息缓冲区，避免无效鼠标信息带入到正式判断中
	while (true)
	{
		while (MouseHit())
		{
			msg = GetMouseMsg();
			if (WM_LBUTTONDOWN == msg.uMsg)//判断鼠标信息;鼠标左键按下
			{
				if (msg.x > 75 && msg.x < 424 && msg.y > 91 && msg.y < 177)//鼠标点击特定区域，即“add”按钮所在区域
					addSchedule(head);//调用添加函数
				if (msg.x > 75 && msg.x < 424 && msg.y > 201 && msg.y < 289)//鼠标点击特定区域，即“delete”按钮所在区域
					Deletemenu(head);//调用删除函数
				if (msg.x > 75 && msg.x < 424 && msg.y > 309 && msg.y < 400)//鼠标点击特定区域，即“change”按钮所在区域
					changemenu(head);//调用修改函数
				if (msg.x > 75 && msg.x < 424 && msg.y > 424 && msg.y < 508)//鼠标点击特定区域，即“display”按钮所在区域
					displaySchedule(head);//调用显示函数
				if (msg.x > 75 && msg.x < 424 && msg.y > 531 && msg.y < 618)//鼠标点击特定区域，即“sort”按钮所在区域
					sort(head);//调用排序函数
				if (msg.x > 443 && msg.x < 643 && msg.y > 654 && msg.y < 714)//鼠标点击特定区域，即“save”按钮所在区域
					WriteToTxtFile(head);//调用保存函数
				if (msg.x > 1171 && msg.x < 1258 && msg.y > 640 && msg.y < 708)//鼠标点击特定区域，即“back”按钮所在区域
				{
					Welcome(head);
				}

				if (msg.x > 1194 && msg.x < 1242 && msg.y > 10 && msg.y < 79)
				{
					loadimage(&exit, "exit.png");
					putimage(0, 0, &exit);
					Sleep(1000);
					system("taskkill /f /im 图形化界面.exe");//关闭程序
				}
			}
		}
	}
}

void Welcome(Node* head)//欢迎界面
{
	IMAGE welcome;
	IMAGE welcome_button;
	loadimage(&welcome, "welcome.png");//导入欢迎界面
	putimage(0, 0, &welcome);//显示欢迎界面
	/*下面进行鼠标交互*/
	MOUSEMSG msg;//定义变量，保存鼠标消息
	FlushMouseMsgBuffer();// 清空鼠标消息缓冲区，避免无效鼠标信息带入到正式判断中
	while (true) // 主循环,循环监听鼠标信息
	{
		while (MouseHit())	//监听鼠标信息;当有鼠标消息的时候执行,可检测连续的鼠标信息
		{
			msg = GetMouseMsg();//获取鼠标消息
			if (WM_LBUTTONDOWN == msg.uMsg)//判断鼠标信息;鼠标左键按下
			{
				if (msg.x > 140 && msg.x < 562 && msg.y > 394 && msg.y < 495)//鼠标点击特定区域，即ENTER按钮所在区域
				{
					loadimage(&welcome_button, "welcome_button.png");//导入橙色按钮图片
					putimage(0, 0, &welcome_button);//显示橙色按钮图片
					Sleep(100);//延时，降低CPU占用率，并且做到点击效果
					Select_MainMenu(head);
					break;
				}
			}
		}
	}
}

int main()
{
	Node* head = Create_LinkList();

	ReadTxtFile(head);//读取文件

	initgraph(1280, 720);//定义窗口大小

	mciSendString("open Make.mp3", 0, 0, 0);//打开音乐文件
	mciSendString("play Make.mp3 repeat", 0, 0, 0);//循环播放音乐

	Welcome(head);//欢迎界面//主页面

	return 0;
}