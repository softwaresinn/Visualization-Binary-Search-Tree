// Contributed by Huzaifa Ahmad(softwaresinn)
// Libraries used:graphic.h(Borland Graphics)

#include<cstdio>
#include<iostream>
#include<conio.h>
#include<malloc.h>
#include<string.h>
#include<cstring>
#include<string>
#include"graphics.h"
#include<cstdlib>
#include<math.h>
using namespace std;
void initgraph(int *graphdriver, int *graphmode, char *pathtodriver);


#define YINC 60
#define F getmaxx()
#define G getmaxy()
int midx, anim;
//Nodes of the tree
struct node
{
	int data;
	struct node *left;
	struct node *right;
}*root = NULL;
//Helper function to create new nodes
struct node *newNode(int data)
{
	struct node *node = (struct node*)malloc(sizeof(struct node));
	node->data = data;
	node->left = NULL;
	node->right = NULL;
	return(node);
}
void displayData(int data, int x, int y)
{
	//Converting integer to string
	int n = data, i = 0, j = 0;
	char str[10];
	while (n)
	{
		str[i++] = n % 10 + '0';
		n /= 10;
		str[i] = '\0';
	}
	for (i--; j<i; j++, i--)
	{
		str[i] += str[j];
		str[j] = str[i] - str[j];
		str[i] -= str[j];
	}
	setcolor(WHITE);
	outtextxy(x - 5, y, str);
	settextstyle(DEFAULT_FONT,HORIZ_DIR,2);
	setcolor(WHITE);
}
void drawLine(int X0, int Y0, int X1, int Y1)
{
	int i;
	//calculate dx & dy
	int dx = X1 - X0;
	int dy = Y1 - Y0;
	//calculate steps required for generating pixels
	int steps = abs(dx)>abs(dy) ? abs(dx) : abs(dy);
	//calculateincrementin x&y for each steps
	float Xinc = dx / (float)steps;
	float Yinc = dy / (float)steps; //Putpixelforeachstep
	float X = X0;
	float Y = Y0;
	for (i = 0; i <= steps; i++)
	{
		putpixel(X, Y, getcolor());
		X += Xinc;
		Y += Yinc;
		if (anim)
			delay(30);
	}
}
void drawCircle(int xc, int yc, int x, int y)
{
	putpixel(xc + x, yc + y, getcolor());
	putpixel(xc - x, yc + y, getcolor());
	putpixel(xc + x, yc - y, getcolor());
	putpixel(xc - x, yc - y, getcolor());
	putpixel(xc + y, yc + x, getcolor());
	putpixel(xc - y, yc + x, getcolor());
	putpixel(xc + y, yc - x, getcolor());
	putpixel(xc - y, yc - x, getcolor());
}
//Functionforcircle-generation //using Bresenham's algorithm
void circleBres(int xc, int yc, int r)
{
	int x = 0, y = r;
	int d = 3 - 2 * r;
	while (y >= x) {
		//foreachpixelwewill //drawalleightpixels
		drawCircle(xc, yc, x, y);
		x++;
		//checkfordecisionparameter
		//andcorrespondingly //updated,x,y
		if (d>0)
		{
			y--;
			d = d + 4 * (x - y) + 10;
		}
		else
			d = d + 4 * x + 6;
		drawCircle(xc, yc, x, y);
		if (anim)
			delay(60);
	}
}
void drawTree(struct node*ptr, char LorR, int depth, int xc, int yc, int xc1, int yc1)
{
	if (LorR == '\0')
		circleBres(xc1, yc1,30);
	else if (LorR == 'L')
	{
		//Calculation for edges
		int len, x_diff, x1, y1, x2, y2;
		x_diff = abs(xc1 - xc);
		len = sqrt(pow(x_diff, 2) + pow(YINC, 2));
		x1 = 25*x_diff / len;
		y1 = 25*YINC / len;
		x2 = xc1 + x1;
		y2 = yc1 - y1;
		x1 = xc - x1;
		y1 = yc + y1;
		drawLine(x1, y1, x2, y2);
		circleBres(xc1, yc1, 25);
	}
	else
	{
		//Calculationforedges
		int len, x_diff, x1, y1, x2, y2;
		x_diff = abs(xc1 - xc);
		len = sqrt(pow(x_diff, 2) + pow(YINC, 2));
		x1 = 25*x_diff / len;
		y1 = 25*YINC / len;
		x2 = xc1 - x1;
		y2 = yc1 - y1;
		x1 = xc + x1;
		y1 = yc + y1;
		drawLine(x1, y1, x2, y2);
		circleBres(xc1, yc1, 25);
	}
	//Display data inside the circle
	displayData(ptr->data, xc1, yc1);
}
void calPos(int xc, int yc, int *xc1, int *yc1, int depth, char  LorR)
{
	int i = 1, x = midx;
	//Calculatingpositionfornextchild
	for (; (i++) <= depth; x /= 2);
	if (xc == -1);
	else if (LorR == 'L')
	{
		*xc1 = xc - x;
		*yc1 = yc + YINC;
	}
	else if (LorR == 'R')
	{
		*xc1 = xc + x;
		*yc1 = yc + YINC;
	}
}
int calTree(struct node*ptr, char  LorR, int depth, int xc, int yc, int xc1, int yc1)
{
	calPos(xc, yc, &xc1, &yc1, depth, LorR);
	if (ptr != NULL)
	{
		drawTree(ptr, LorR, depth, xc, yc, xc1, yc1);
		calTree(ptr->left, 'L', depth + 1, xc1, yc1, xc1, yc1);
	}
	else
		return 0;
	calTree(ptr->right, 'R', depth + 1, xc1, yc1, xc1, yc1);
}
struct node*insert(struct node*ptr, char  LorR, int depth, int xc, int yc, int xc1, int yc1, int data)
{
	calPos(xc, yc, &xc1, &yc1, depth, LorR);
	if (ptr == NULL)
	{
		ptr = newNode(data);
		drawTree(ptr, LorR, depth, xc, yc, xc1, yc1);
		return ptr;
	}
	if (data == ptr->data)
	{
		puts("Duplicate elements are not allowed");
		delay(3000);
		return ptr;
	}
	else  if (data<ptr->data)
		ptr->left = insert(ptr->left, 'L', depth + 1, xc1, yc1, xc1, yc1, data);
	else
		ptr->right = insert(ptr->right, 'R', depth + 1, xc1, yc1, xc1, yc1, data);
	return ptr;
}
struct node*minValue(struct node*temp)
{
	while (temp->left != NULL)
		temp = temp->left;
	return  temp;
}
struct node*del(struct node*ptr, char  LorR, int depth, int xc, int yc, int xc1, int yc1, int  data)
{
	struct node*temp;
	calPos(xc, yc, &xc1, &yc1, depth, LorR);
	if (ptr == NULL)
	{
		cout << "Invalid Value";
		delay(3000);
		return  ptr;
	}
	if (data<ptr->data)
		ptr->left = del(ptr->left, 'L', depth + 1, xc1, yc1, xc1, yc1, data);
	else  if (data>ptr->data)
		ptr->right = del(ptr->right, 'R', depth + 1, xc1, yc1, xc1, yc1, data);
	else
	{
		setcolor(BLACK);
		if (ptr->left == NULL)
		{
			temp = ptr->right;
			drawTree(ptr, LorR, depth, xc, yc, xc1, yc1);
			free(ptr);
			return  temp;
		}
		else if (ptr->right == NULL)
		{
			temp = ptr->left;
			drawTree(ptr, LorR, depth, xc, yc, xc1, yc1);
			free(ptr);
			return  temp;
		}
		drawTree(ptr, LorR, depth, xc, yc, xc1, yc1);
		temp = minValue(ptr->right);
		ptr->data = temp->data;
		ptr->right = del(ptr->right, 'R', depth + 1, xc1, yc1, xc1, yc1, temp->data);
	}
	return ptr;
}
void boundary(int j, int k);

// Main
int main()
{
	int gdriver = DETECT, gmode,errorcode;
	int data;
	char cmnd[5];
	system("cls");
	initgraph(&gdriver, &gmode, "");


	midx = getmaxx() / 2;
	while (1)
	{
		anim = 1;
		cin >> cmnd;
		if (strcmp(cmnd, "ins") == 0)
		{
			cin >> data;
			if (root == NULL)
				root = insert(root, '\0', 0, -1, -1, midx, 30, data);
			else
				insert(root, '\0', 0, -1, -1, midx, 30, data);
		}
		else  if (strcmp(cmnd, "del") == 0)
		{
			cin >> data;
			if (root == NULL)
				cout << "Tree is already empty" << endl;
			else
				root = del(root, '\0', 0, -1, -1, midx, 30, data);
		}
		else  if (strcmp(cmnd, "root") == 0)
		{
			cout << endl << root->data;
			delay(3000);
		}
		else  if (strcmp(cmnd, "exit") == 0)
		{
			cleardevice();
			setbkcolor(15);
			boundary(8, 5);
			setcolor(5);
			settextstyle(3, 0, 5);
			outtextxy(130, 110, "EXIT");
			settextstyle(3, 0, 4);
			outtextxy(110, 180, "EXIT");
			delay(2000);
			closegraph();
			exit(0);
		}
		else
		{
			cout << "Not a Valid Command" << endl;
			delay(3000);
		}
		setcolor(WHITE);
		system("cls");
		cleardevice();
		anim = 0;
		calTree(root, '\0', 0, -1, -1, midx, 30);
	}
	system("pause>0");
	return 0;

}
//boundary animations
void boundary(int j, int k)
{
	int i, r = 10;
	for (i = 10; i<getmaxx(); i++)
	{
		setcolor(j);
		circle(10, i, r);
		circle(getmaxx() - r, i, r);
		setcolor(k);
		circle(i, 10, r);
		circle(i, getmaxy() - r, r);
		delay(1);
	}
}


