#include <graphics.h> //ͼ�λ�����
#include <malloc.h>//�����ڴ�
#include <conio.h> //getche()
#include <string>//�ַ�������
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <mmsystem.h>//ý��
#include <easyx.h>//ͼ�λ�
#include <io.h>//������
#pragma comment(lib,"winmm.lib")//����Windows�Դ�ý��
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" ) // ������ڵ�ַ

struct schedule
{
	char data[11];//����
	char time[11];//ʱ��
	char location[50];//�ص�
	char man[30];//����
	char schedule[200];//�ճ�
	struct schedule* next;
};

typedef struct schedule Node;//����

Node* Create_LinkList()
{
	Node* head = (Node*)malloc(sizeof(Node));  //����ͷ�ڵ�
	if (head == NULL)   //����ʧ��,�򷵻�
	{
		printf("�ڴ����ʧ��!\n");
		return (NULL);
	}
	head->next = NULL; //ͷ�ڵ��ָ������NULL
	return head;
}

void Select_MainMenu(Node* head);

void ReadTxtFile(Node* head)//��ȡ�ļ�����
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


	fp = fopen("schedule.txt", "r");//��ȡ�ļ�

	p = head;

	while (p->next != NULL)//�ҵ�β��
	{
		p = p->next;
	}

	while (fscanf(fp, "%s %s %s %s %s\n", data, time, location, man, schedule) > 0)//�жϲ�Ϊ��
	{
		pnew = (Node*)malloc(sizeof(Node));//�����ڴ�

		if (pnew == NULL)
		{
			printf("no enough memory!\n");
			return;
		}

		strcpy(pnew->data, data);//����
		strcpy(pnew->time, time);
		strcpy(pnew->location, location);
		strcpy(pnew->man, man);
		strcpy(pnew->schedule, schedule);

		pnew->next = NULL;//β��
		p->next = pnew;
		p = pnew;
	}

	fclose(fp);//�ر��ļ�
}

void WriteToTxtFile(Node* head)//д���ļ����ݣ�����
{
	IMAGE save;
	loadimage(&save, "save.png");//�����½��ճ̽���
	putimage(0, 0, &save);

	FILE* fp;
	Node* p;

	fp = fopen("schedule.txt", "w");//���ļ�

	for (p = head->next; p != NULL; p = p->next)//д��
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
	fclose(fp);//�ر��ļ�

	MessageBox(NULL, _T("����ɹ�!"), _T("���Ѵ���"), MB_OK | MB_SYSTEMMODAL);//����ɹ�
	Select_MainMenu(head);
}

void addSchedule(Node* head)
{
	IMAGE add;
	loadimage(&add, "add.png");//�����½��ճ̽���
	putimage(0, 0, &add);

	for (; head->next != NULL; head = head->next);//�ҵ�β��

	Node* test = (Node*)malloc(sizeof(Node));//�����ڴ�

	test->next = NULL;//β���

	InputBox(test->data, 11, _T("����������:yyyy/mm/dd(����:2018/01/09)"));//����
	InputBox(test->time, 11, _T("���������ʱ��:hh:MM:ss(����:23:01:00)"));//ʱ��
	InputBox(test->location, 50, _T("�������¼��ص�(�Ϊ50���ַ�)"));//�ص�
	InputBox(test->man, 30, _T("�������¼�����(�Ϊ30���ַ�)"));//����
	InputBox(test->schedule, 200, _T("�������¼�����(�Ϊ200���ַ�)"));//�ճ�

	head->next = test;

	MessageBox(NULL, _T("��ӳɹ�!"), _T("���Ѵ���"), MB_OK | MB_SYSTEMMODAL);//��ӳɹ�����
}

void displaySchedule(Node* head)
{
	IMAGE display;

	loadimage(&display, "display.png");//������ʾ�ճ̽���
	putimage(0, 0, &display);

	Node* p;

	int position_x = 443;//����ʼ��������
	int position_y = 91;//����ʼ��������

	setbkmode(TRANSPARENT);//����͸��
	settextcolor(RGB(0, 0, 0));//��������Ϊ��ɫ
	settextstyle(20, 0, _T("�����Բ_CNKI"));//���������С20����ʽ�����Բ_CNKI
	outtextxy(position_x, position_y, _T("����                                    ʱ��                                    �ص�                                    ����                                    �ճ�"));//��ͷ

	for (p = head->next; p != NULL; p = p->next)
	{
		position_y += 20;//������һ��

		outtextxy(443, position_y, p->data);//����
		outtextxy(621, position_y, p->time);//ʱ��
		outtextxy(801, position_y, p->location);//�ص�
		outtextxy(977, position_y, p->man);//����
		outtextxy(1157, position_y, p->schedule);//�ճ�
	}
}

void deleteByDate(Node* head)//��������ɾ��
{
	Node* before, * cur;

	char data[11];
	char time[11];

	InputBox(data, 11, _T("������ɾ��������:yyyy/mm/dd(����:2018/01/09)"));//����
	InputBox(time, 11, _T("������ɾ���ľ���ʱ��:hh:MM:ss(����:23:01:00)"));//ʱ��

	for (before = head, cur = head->next; cur != NULL; cur = cur->next, before = before->next)
	{
		if (strcmp(cur->data,data)==0 && strcmp(cur->time,time)==0)
		{
			before->next = cur->next;
			MessageBox(NULL, _T("ɾ���ɹ���"), _T("ɾ�����Ѵ���"), MB_OK | MB_SYSTEMMODAL);//ɾ�����Ѵ���
			break;
		}
	}
	if (cur == NULL) 
	{
		MessageBox(NULL, _T("δ�ҵ����¼���"), _T("ɾ�����Ѵ���"), MB_OK | MB_SYSTEMMODAL);//ɾ�����Ѵ���
	}

}

void deleteBySchedule(Node* head)//���ճ���ɾ��
{
	Node* before, * cur;
	char temp[200];

	InputBox(temp, 200, _T("������ɾ���¼�����(�Ϊ200���ַ�)"));//�ճ�

	for (before = head, cur = head->next; cur != NULL; before = before->next, cur = cur->next)//�ҽ��
	{
		if (strcmp(temp, cur->schedule) == 0)//�ж��ҵ����
		{
			before->next = cur->next;
			MessageBox(NULL, _T("ɾ���ɹ���"), _T("ɾ�����Ѵ���"), MB_OK | MB_SYSTEMMODAL);//ɾ�����Ѵ���
			break;
		}
	}
	if (cur == NULL)
	{
		MessageBox(NULL, _T("δ�ҵ����¼���"), _T("ɾ�����Ѵ���"), MB_OK | MB_SYSTEMMODAL);//ɾ�����Ѵ���
	}
}

void Deletemenu(Node* head)//ɾ���˵�
{
	IMAGE Delete;

	loadimage(&Delete, "delete.png");//������ʾ�ճ̽���
	putimage(0, 0, &Delete);

	MessageBox(NULL, _T("-------ɾ���˵�-------\n        1.��ʱ��ɾ��\n        2.���¼���ɾ��"), _T("ɾ�����Ѵ���"), MB_OK | MB_SYSTEMMODAL);//ɾ�����Ѵ���
	
	char a='0';
	char flag[2]={};

	InputBox(flag, 2, _T("������ѡ��"));//ѡ��

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
	default:MessageBox(NULL, _T("����������"), _T("���Ѵ���"), MB_OK | MB_SYSTEMMODAL);//���Ѵ���
		break;
	}

}

void changeByDate(Node* head)//���������޸�
{
	Node* before, * cur;

	char data[11];
	char time[11];

	char datac[11];
	char timec[11];

	char locationc[50];
	char manc[30];
	char schedulec[200];

	InputBox(data, 11, _T("�������޸ĵ�����:yyyy/mm/dd(����:2018/01/09)"));//����
	InputBox(time, 11, _T("�������޸ĵľ���ʱ��:hh:MM:ss(����:23:01:00)"));//ʱ��


	for (before = head, cur = head->next; cur != NULL; cur = cur->next, before = before->next)
	{
		if (strcmp(cur->data, data) == 0 && strcmp(cur->time, time) == 0)
		{
			char see;

			do
			{
				MessageBox(NULL, _T("-------�޸Ĳ˵�-------\n        1.�޸�����\n        2.�޸�����\n        3.�޸ĵص�\n        4.�޸�����\n        5.�޸��¼�"), _T("�޸����Ѵ���"), MB_OK | MB_SYSTEMMODAL);//�޸�ѡ�񴰿�

				char a = '0';
				char flag[2] = {};

				InputBox(flag, 2, _T("������ѡ��:"));//ѡ��

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
				case '1':InputBox(datac, 11, _T("�������޸ĺ������:yyyy/mm/dd(����:2018/01/09)"));//����
					strcpy(cur->data, datac);
					break;
				case '2':InputBox(timec, 11, _T("�������޸ĺ�ľ���ʱ��:hh:MM:ss(����:23:01:00)"));//ʱ��
					strcpy(cur->time, timec);
					break;
				case '3':InputBox(locationc, 50, _T("�������޸ĺ���¼��ص�(�Ϊ50���ַ�)"));//�ص�
					strcpy(cur->location, locationc);
					break;
				case '4':InputBox(manc, 30, _T("�������޸ĺ���¼�����(�Ϊ30���ַ�)"));//����
					strcpy(cur->man, manc);
					break;
				case '5':InputBox(schedulec, 200, _T("�������޸ĺ���¼�����(�Ϊ200���ַ�)"));//�ճ�
					strcpy(cur->schedule, schedulec);
					break;
				default:MessageBox(NULL, _T("����������!"), _T("���Ѵ���"), MB_OK | MB_SYSTEMMODAL);//���Ѵ���
					break;
				}

				MessageBox(NULL, _T("�޸ĳɹ����Ƿ�����޸ģ�\n����(y)       �˳�(n)"), _T("�޸����Ѵ���"), MB_OK | MB_SYSTEMMODAL);//ɾ�����Ѵ���

				char b = '0';
				char choice[2] = {};

				InputBox(choice, 2, _T("������ѡ��:"));//ѡ��

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
				default:MessageBox(NULL, _T("�������!"), _T("���Ѵ���"), MB_OK | MB_SYSTEMMODAL);//���Ѵ���
				}
			} while (see == 'y');

			break;
		}
	}
	if (cur == NULL) 
	{
		MessageBox(NULL, _T("δ�ҵ����¼���"), _T("�޸����Ѵ���"), MB_OK | MB_SYSTEMMODAL);//ɾ�����Ѵ���
	}
}

void changeBySchedule(Node* head)//���ճ����޸�
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

	InputBox(temp, 200, _T("�������޸��¼�����(�Ϊ200���ַ�)"));//�ճ�

	for (before = head, cur = head->next; cur != NULL; before = before->next, cur = cur->next)
	{
		if (strcmp(temp, cur->schedule) == 0)
		{
			char see;

			do
			{
				MessageBox(NULL, _T("-------�޸Ĳ˵�-------\n        1.�޸�����\n        2.�޸�����\n        3.�޸ĵص�\n        4.�޸�����\n        5.�޸��¼�"), _T("�޸����Ѵ���"), MB_OK | MB_SYSTEMMODAL);//�޸�ѡ�񴰿�

				char a = '0';
				char flag[2] = {};

				InputBox(flag, 2, _T("������ѡ��:"));//ѡ��

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
				case '1':InputBox(datac, 11, _T("�������޸ĺ������:yyyy/mm/dd(����:2018/01/09)"));//����
					strcpy(cur->data, datac);
					break;
				case '2':InputBox(timec, 11, _T("�������޸ĺ�ľ���ʱ��:hh:MM:ss(����:23:01:00)"));//ʱ��
					strcpy(cur->time, timec);
					break;
				case '3':InputBox(locationc, 50, _T("�������޸ĺ���¼��ص�(�Ϊ50���ַ�)"));//�ص�
					strcpy(cur->location, locationc);
					break;
				case '4':InputBox(manc, 30, _T("�������޸ĺ���¼�����(�Ϊ30���ַ�)"));//����
					strcpy(cur->man, manc);
					break;
				case '5':InputBox(schedulec, 200, _T("�������޸ĺ���¼�����(�Ϊ200���ַ�)"));//�ճ�
					strcpy(cur->schedule, schedulec);
					break;
				default:MessageBox(NULL, _T("����������!"), _T("���Ѵ���"), MB_OK | MB_SYSTEMMODAL);//���Ѵ���
					break;
				}

				MessageBox(NULL, _T("�޸ĳɹ����Ƿ�����޸ģ�\n����(y)       �˳�(n)"), _T("�޸����Ѵ���"), MB_OK | MB_SYSTEMMODAL);//ɾ�����Ѵ���

				char b = '0';
				char choice[2] = {};

				InputBox(choice, 2, _T("������ѡ��:"));//ѡ��

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
				default:MessageBox(NULL, _T("�������!"), _T("���Ѵ���"), MB_OK | MB_SYSTEMMODAL);//���Ѵ���
				}
			} while (see=='y');
			
			break;
		}
	}
	if (cur == NULL)
	{
		MessageBox(NULL, _T("δ�ҵ����¼���"), _T("�޸����Ѵ���"), MB_OK | MB_SYSTEMMODAL);//ɾ�����Ѵ���
	}
}

void changemenu(Node* head)//�޸Ĳ˵�
{
	IMAGE change;

	loadimage(&change, "change.png");//�����޸��ճ̽���
	putimage(0, 0, &change);

	MessageBox(NULL, _T("-------�޸Ĳ˵�-------\n        1.��ʱ���޸�\n        2.���¼����޸�"), _T("�޸����Ѵ���"), MB_OK | MB_SYSTEMMODAL);//�޸����Ѵ���

	char a = '0';
	char flag[2] = {};

	InputBox(flag, 2, _T("������ѡ��:"));//ѡ��

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
	default:MessageBox(NULL, _T("����������!"), _T("���Ѵ���"), MB_OK | MB_SYSTEMMODAL);//���Ѵ���
		break;
	}
}

int compare(Node* a, Node* b) //�Ƚ������¼����Ⱥ�
{
	//��data��time��������,ֱ�ӱȽ��ַ����Ĵ�С
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

Node* sort(Node* L) //�¼�����//ѡ������
{
	IMAGE sort;

	loadimage(&sort, "sort.png");//���������ճ̽���
	putimage(0, 0, &sort);

	Node* p, * q, * next;

	for (p = L->next; p->next != NULL; p = p->next)    /*ÿ��ѭ�����ҳ�һ����Сֵ������Сֵ��������һλ��Ȼ��ָ������ƶ�һλ*/
	{
		next = p;
		for (q = p->next; q; q = q->next)    /*��ǰ���������ҳ���С�Ľڵ�*/
		{
			if (compare(next, q) == 1)//�Ƚ�
				next = q;
		}
		if (next != p)//����
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
	MessageBox(NULL, _T("����ɹ���"), _T("�������Ѵ���"), MB_OK | MB_SYSTEMMODAL);//�������Ѵ���
	return L;
}

void Welcome(Node* head);
void displaySchedule(Node* head);

void Select_MainMenu(Node* head)//���˵�
{
	IMAGE menu;
	IMAGE exit;
	loadimage(&menu, "fmenu.png");//�����ʼ����
	putimage(0, 0, &menu);//��ʾ��ʼ����
	MOUSEMSG msg;//������������������Ϣ
	FlushMouseMsgBuffer();// ��������Ϣ��������������Ч�����Ϣ���뵽��ʽ�ж���
	while (true)
	{
		while (MouseHit())
		{
			msg = GetMouseMsg();
			if (WM_LBUTTONDOWN == msg.uMsg)//�ж������Ϣ;����������
			{
				if (msg.x > 75 && msg.x < 424 && msg.y > 91 && msg.y < 177)//������ض����򣬼���add����ť��������
					addSchedule(head);//������Ӻ���
				if (msg.x > 75 && msg.x < 424 && msg.y > 201 && msg.y < 289)//������ض����򣬼���delete����ť��������
					Deletemenu(head);//����ɾ������
				if (msg.x > 75 && msg.x < 424 && msg.y > 309 && msg.y < 400)//������ض����򣬼���change����ť��������
					changemenu(head);//�����޸ĺ���
				if (msg.x > 75 && msg.x < 424 && msg.y > 424 && msg.y < 508)//������ض����򣬼���display����ť��������
					displaySchedule(head);//������ʾ����
				if (msg.x > 75 && msg.x < 424 && msg.y > 531 && msg.y < 618)//������ض����򣬼���sort����ť��������
					sort(head);//����������
				if (msg.x > 443 && msg.x < 643 && msg.y > 654 && msg.y < 714)//������ض����򣬼���save����ť��������
					WriteToTxtFile(head);//���ñ��溯��
				if (msg.x > 1171 && msg.x < 1258 && msg.y > 640 && msg.y < 708)//������ض����򣬼���back����ť��������
				{
					Welcome(head);
				}

				if (msg.x > 1194 && msg.x < 1242 && msg.y > 10 && msg.y < 79)
				{
					loadimage(&exit, "exit.png");
					putimage(0, 0, &exit);
					Sleep(1000);
					system("taskkill /f /im ͼ�λ�����.exe");//�رճ���
				}
			}
		}
	}
}

void Welcome(Node* head)//��ӭ����
{
	IMAGE welcome;
	IMAGE welcome_button;
	loadimage(&welcome, "welcome.png");//���뻶ӭ����
	putimage(0, 0, &welcome);//��ʾ��ӭ����
	/*���������꽻��*/
	MOUSEMSG msg;//������������������Ϣ
	FlushMouseMsgBuffer();// ��������Ϣ��������������Ч�����Ϣ���뵽��ʽ�ж���
	while (true) // ��ѭ��,ѭ�����������Ϣ
	{
		while (MouseHit())	//���������Ϣ;���������Ϣ��ʱ��ִ��,�ɼ�������������Ϣ
		{
			msg = GetMouseMsg();//��ȡ�����Ϣ
			if (WM_LBUTTONDOWN == msg.uMsg)//�ж������Ϣ;����������
			{
				if (msg.x > 140 && msg.x < 562 && msg.y > 394 && msg.y < 495)//������ض����򣬼�ENTER��ť��������
				{
					loadimage(&welcome_button, "welcome_button.png");//�����ɫ��ťͼƬ
					putimage(0, 0, &welcome_button);//��ʾ��ɫ��ťͼƬ
					Sleep(100);//��ʱ������CPUռ���ʣ������������Ч��
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

	ReadTxtFile(head);//��ȡ�ļ�

	initgraph(1280, 720);//���崰�ڴ�С

	mciSendString("open Make.mp3", 0, 0, 0);//�������ļ�
	mciSendString("play Make.mp3 repeat", 0, 0, 0);//ѭ����������

	Welcome(head);//��ӭ����//��ҳ��

	return 0;
}