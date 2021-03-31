#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <WinUser.h>
#include <math.h>
using namespace std;

#define VK_W 0x57
#define VK_A 0x41
#define VK_S 0x53
#define VK_D 0x44

struct ROBOT {
	COORD PlayerPos;
	COORD Kecoak1;
	COORD Kecoak2;
	COORD Kecoak3;
	COORD Kecoak4;
	COORD Kecoak5;
	COORD PlayerBullet1;
	COORD PlayerBullet2;
	COORD Left;
	COORD Bottom;
	COORD T_PlayerPos;
	COORD T_PlayerHealth;
	COORD T_NearestKecoak;
	COORD T_KecoakPos;
	COORD T_KecoakXY1;
	COORD T_KecoakXY2;
	COORD T_KecoakXY3;
	COORD T_KecoakXY4;
	COORD T_KecoakXY5;
	COORD Meleset;
};

HANDLE hinp, hout;
COORD coord;
ROBOT robot;

const int bottom = 28; // paling bawah 27
const int left = 0; // paling kiri 1
const int right = 90; //paling kanan 119
const int top = 0; // paling atas 1

int killed;
int shoot_distance;
int PlayerHealth;
int KecoakHealth[5];
bool shot;

void arena (ROBOT &robot);
void clear (ROBOT &robot);
void move(ROBOT &robot);
void deployKecoak(ROBOT &robot);
void shoot(ROBOT &robot, int kodearah);
void shoot2(ROBOT &robot, int kodearah);
int Command();
void ubah(int x, int y, int px, int py, int num, int* cont1, int* cont2);
void deployOne(int* cont1, int* cont2);
double distance(int x, int y, int x1, int y1);
double nearestKecoak(ROBOT &robot);
int random(int upper, int lower);
void displayPerKecoak(ROBOT &robot, int x, int y, int* contx, int* conty, int num);
void end();

int main(){
	ROBOT robot;
	COORD coord;
	
	hinp = GetStdHandle(STD_INPUT_HANDLE);
	hout = GetStdHandle(STD_OUTPUT_HANDLE);
	
	SetConsoleMode(hout, ENABLE_PROCESSED_INPUT);
	
	shot = false;
	killed = 0;
	shoot_distance = 20;
	PlayerHealth = 9;
	
	robot.PlayerPos.X = 1; //(0,0) coordinate is at (1,27)
	robot.PlayerPos.Y = 27;
	robot.PlayerBullet1.X = 0;
	robot.PlayerBullet1.Y = 1;
	robot.PlayerBullet2.X = 0;
	robot.PlayerBullet2.Y = 1;
	robot.Kecoak1.X = 0;
	robot.Kecoak1.Y = 0;
	robot.Kecoak2.X = 0;
	robot.Kecoak2.Y = 0;
	robot.Kecoak3.X = 0;
	robot.Kecoak3.Y = 0;
	robot.Kecoak4.X = 0;
	robot.Kecoak4.Y = 0;
	robot.Kecoak5.X = 0;
	robot.Kecoak5.Y = 0;
	robot.T_PlayerPos.X = 95;
	robot.T_PlayerPos.Y = 5;
	robot.T_PlayerHealth.X = 95;
	robot.T_PlayerHealth.Y = 7;
	robot.T_NearestKecoak.X = 95;
	robot.T_NearestKecoak.Y = 9;
	robot.T_KecoakPos.X = 95;
	robot.T_KecoakPos.Y = 11;
	robot.T_KecoakXY1.X = 95;
	robot.T_KecoakXY1.Y = 12;
	robot.T_KecoakXY2.X = 95;
	robot.T_KecoakXY2.Y = 13;
	robot.T_KecoakXY3.X = 95;
	robot.T_KecoakXY3.Y = 14;
	robot.T_KecoakXY4.X = 95;
	robot.T_KecoakXY4.Y = 15;
	robot.T_KecoakXY5.X = 95;
	robot.T_KecoakXY5.Y = 16;
	robot.Meleset.X = 95;
	robot.Meleset.Y = 18;
	
	for(int i=1; i<=5; i++)
		KecoakHealth[i] = 2;
	
	arena(robot);
	while(PlayerHealth>0){
		move(robot);
	}
	
	system("cls");
	if(killed>0)
		printf("You Lost! How Unfortunate!\n\nHowever, you have killed %d mutant cockroaches.\n\n ~THANK YOU~", killed);
	else
		printf("You Lost! How Unfortunate!\n\nHowever, you haven't' killed any mutant cockroaches.\n\nSee you next time!\n");
	system("pause");
	exit(0);
}

void arena (ROBOT &robot)
{
	SetConsoleCursorPosition(hout, robot.PlayerPos);
	printf("R");
	
	SetConsoleCursorPosition(hout, robot.PlayerBullet1);
	printf("*");
	
	SetConsoleCursorPosition(hout, robot.PlayerBullet2);
	printf("*");
	
	SetConsoleCursorPosition(hout, robot.Kecoak1);
	printf("1");
	
	SetConsoleCursorPosition(hout, robot.Kecoak2);
	printf("2");
	
	SetConsoleCursorPosition(hout, robot.Kecoak3);
	printf("3");
	
	SetConsoleCursorPosition(hout, robot.Kecoak4);
	printf("4");
	
	SetConsoleCursorPosition(hout, robot.Kecoak5);
	printf("5");
	
	SetConsoleCursorPosition(hout, robot.T_PlayerPos);
	int x = robot.PlayerPos.X;
	int y = bottom - 1 - robot.PlayerPos.Y;
	printf("(%d,%d)", x, y);
	
	SetConsoleCursorPosition(hout, robot.T_PlayerHealth);
	printf("Health : ");
	for(int i=1; i<=PlayerHealth; i++){
		printf("#");
	}
	
	SetConsoleCursorPosition(hout, robot.T_NearestKecoak);
	printf("Dist. %.2lf ", nearestKecoak(robot));
	
	SetConsoleCursorPosition(hout, robot.T_KecoakPos);
	printf("Posisi Kecoak : ");
	
	SetConsoleCursorPosition(hout, robot.T_KecoakXY1);
	x = robot.Kecoak1.X;
	y = robot.Kecoak1.Y;
	printf("(%d,%d) %.2lf ", x, y, distance(robot.PlayerPos.X, robot.PlayerPos.Y, robot.Kecoak1.X, robot.Kecoak1.Y));
	for(int i=0; i<KecoakHealth[1]; i++)
		printf("$");
	
	SetConsoleCursorPosition(hout, robot.T_KecoakXY2);
	x = robot.Kecoak2.X;
	y = robot.Kecoak2.Y;
	printf("(%d,%d) %.2lf ", x, y, distance(robot.PlayerPos.X, robot.PlayerPos.Y, robot.Kecoak2.X, robot.Kecoak2.Y));
	for(int i=0; i<KecoakHealth[2]; i++)
		printf("$");
	
	SetConsoleCursorPosition(hout, robot.T_KecoakXY3);
	x = robot.Kecoak3.X;
	y = robot.Kecoak3.Y;
	printf("(%d,%d) %.2lf ", x, y, distance(robot.PlayerPos.X, robot.PlayerPos.Y, robot.Kecoak3.X, robot.Kecoak3.Y));
	for(int i=0; i<KecoakHealth[3]; i++)
		printf("$");
	
	SetConsoleCursorPosition(hout, robot.T_KecoakXY4);
	x = robot.Kecoak4.X;
	y = robot.Kecoak4.Y;
	printf("(%d,%d) %.2lf ", x, y, distance(robot.PlayerPos.X, robot.PlayerPos.Y, robot.Kecoak4.X, robot.Kecoak4.Y));
	for(int i=0; i<KecoakHealth[4]; i++)
		printf("$");
	
	SetConsoleCursorPosition(hout, robot.T_KecoakXY5);
	x = robot.Kecoak5.X;
	y = robot.Kecoak5.Y;
	printf("(%d,%d) %.2lf ", x, y, distance(robot.PlayerPos.X, robot.PlayerPos.Y, robot.Kecoak5.X, robot.Kecoak5.Y));
	for(int i=0; i<KecoakHealth[5]; i++)
		printf("$");
	
	robot.Left.X = left;
	robot.Left.Y = 0;
	for(int i=0; i<bottom; i++){
		SetConsoleCursorPosition(hout, robot.Left);
		printf("|");
		robot.Left.Y++;
	}
	
	robot.Bottom.X = 0;
	robot.Bottom.Y = bottom;
	for(int i=0; i<right; i++){
		SetConsoleCursorPosition(hout, robot.Bottom);
		printf("_");
		robot.Bottom.X++;
	}
}

double nearestKecoak(ROBOT &robot){
	int x = robot.PlayerPos.X;
	int y = robot.PlayerPos.Y;
	int x1 = robot.Kecoak1.X;
	int y1 = robot.Kecoak1.Y;
	double a = distance(x, y, x1, y1);
	x1 = robot.Kecoak2.X;
	y1 = robot.Kecoak2.Y;
	double b = distance(x, y, x1, y1);
	x1 = robot.Kecoak3.X;
	y1 = robot.Kecoak3.Y;
	double c = distance(x, y, x1, y1);
	x1 = robot.Kecoak4.X;
	y1 = robot.Kecoak4.Y;
	double d = distance(x, y, x1, y1);
	x1 = robot.Kecoak5.X;
	y1 = robot.Kecoak5.Y;
	double e = distance(x, y, x1, y1);
	if(a<b && a<c && a<d && a<e)
		return a;
	else if(b<a && b<c && b<d && b<e)
		return b;
	else if(c<a && c<b && c<d && c<e)
		return c;
	else if(d<a && d<b && d<c && d<e)
		return d;
	else if(e<a && e<b && e<d && e<c)
		return e;
}

void clear (ROBOT &robot)
{
	SetConsoleCursorPosition(hout, robot.PlayerPos);
	printf(" ");
	
	SetConsoleCursorPosition(hout, robot.PlayerBullet1);
	printf(" ");
	
	SetConsoleCursorPosition(hout, robot.PlayerBullet2);
	printf(" ");
	
	SetConsoleCursorPosition(hout, robot.Kecoak1);
	printf(" ");
	
	SetConsoleCursorPosition(hout, robot.Kecoak2);
	printf(" ");
	
	SetConsoleCursorPosition(hout, robot.Kecoak3);
	printf(" ");
	
	SetConsoleCursorPosition(hout, robot.Kecoak4);
	printf(" ");
	
	SetConsoleCursorPosition(hout, robot.Kecoak5);
	printf(" ");
	
	SetConsoleCursorPosition(hout, robot.T_PlayerPos);
	int x = robot.PlayerPos.X;
	int y = bottom - 1 - robot.PlayerPos.Y;
	printf("       ", x, y);
	
	SetConsoleCursorPosition(hout, robot.T_PlayerHealth);
	printf("Health : ");
	for(int i=1; i<=PlayerHealth; i++){
		printf(" ");
	}
	
	SetConsoleCursorPosition(hout, robot.T_NearestKecoak);
	printf("            ", nearestKecoak(robot));
	
	SetConsoleCursorPosition(hout, robot.T_KecoakPos);
	printf("Posisi Kecoak : ");
	
	SetConsoleCursorPosition(hout, robot.T_KecoakXY1);
	x = robot.Kecoak1.X;
	y = robot.Kecoak1.Y;
	printf("              ", x, y, distance(robot.PlayerPos.X, robot.PlayerPos.Y, robot.Kecoak1.X, robot.Kecoak1.Y));
	for(int i=0; i<KecoakHealth[1]; i++)
		printf(" ");
	
	SetConsoleCursorPosition(hout, robot.T_KecoakXY2);
	x = robot.Kecoak2.X;
	y = robot.Kecoak2.Y;
	printf("              ", x, y, distance(robot.PlayerPos.X, robot.PlayerPos.Y, robot.Kecoak2.X, robot.Kecoak2.Y));
	for(int i=0; i<KecoakHealth[2]; i++)
		printf(" ");
	
	SetConsoleCursorPosition(hout, robot.T_KecoakXY3);
	x = robot.Kecoak3.X;
	y = robot.Kecoak3.Y;
	printf("              ", x, y, distance(robot.PlayerPos.X, robot.PlayerPos.Y, robot.Kecoak3.X, robot.Kecoak3.Y));
	for(int i=0; i<KecoakHealth[3]; i++)
		printf(" ");
	
	SetConsoleCursorPosition(hout, robot.T_KecoakXY4);
	x = robot.Kecoak4.X;
	y = robot.Kecoak4.Y;
	printf("              ", x, y, distance(robot.PlayerPos.X, robot.PlayerPos.Y, robot.Kecoak4.X, robot.Kecoak4.Y));
	for(int i=0; i<KecoakHealth[4]; i++)
		printf(" ");
	
	SetConsoleCursorPosition(hout, robot.T_KecoakXY5);
	x = robot.Kecoak5.X;
	y = robot.Kecoak5.Y;
	printf("              ", x, y, distance(robot.PlayerPos.X, robot.PlayerPos.Y, robot.Kecoak5.X, robot.Kecoak5.Y));
	for(int i=0; i<KecoakHealth[5]; i++)
		printf(" ");
	
	robot.Left.X = left;
	robot.Left.Y = 0;
	for(int i=0; i<bottom; i++){
		SetConsoleCursorPosition(hout, robot.Left);
		printf("|");
		robot.Left.Y++;
	}
	
	robot.Bottom.X = 0;
	robot.Bottom.Y = bottom;
	for(int i=0; i<right; i++){
		SetConsoleCursorPosition(hout, robot.Bottom);
		printf("_");
		robot.Bottom.X++;
	}
}

void move(ROBOT &robot)
{
	INPUT_RECORD InputRecord;
	DWORD Events = 0;
	
	ReadConsoleInput(hinp, &InputRecord, 1, &Events);
	
	if(InputRecord.EventType == KEY_EVENT){
		if(InputRecord.Event.KeyEvent.wVirtualKeyCode == VK_RIGHT && InputRecord.Event.KeyEvent.bKeyDown == 1){
			clear(robot);
			if(robot.PlayerPos.X < right)
				robot.PlayerPos.X++;
			arena(robot);
		}
		else if(InputRecord.Event.KeyEvent.wVirtualKeyCode == VK_LEFT && InputRecord.Event.KeyEvent.bKeyDown == 1){
			clear(robot);
			if(robot.PlayerPos.X > left)
				robot.PlayerPos.X--;
			arena(robot);
		}
		else if(InputRecord.Event.KeyEvent.wVirtualKeyCode == VK_DOWN && InputRecord.Event.KeyEvent.bKeyDown == 1){
			clear(robot);
			if(robot.PlayerPos.Y < bottom)
				robot.PlayerPos.Y++;
			arena(robot);
		}
		else if(InputRecord.Event.KeyEvent.wVirtualKeyCode == VK_UP && InputRecord.Event.KeyEvent.bKeyDown == 1){
			clear(robot);
			if(robot.PlayerPos.Y > top)
				robot.PlayerPos.Y--;
			arena(robot);
		}
		else if(InputRecord.Event.KeyEvent.wVirtualKeyCode == VK_W && InputRecord.Event.KeyEvent.bKeyDown == 1){ //shoot atas
			shoot(robot, 1);
		}
		else if(InputRecord.Event.KeyEvent.wVirtualKeyCode == VK_A && InputRecord.Event.KeyEvent.bKeyDown == 1){ //shoot kiri
			shoot(robot, 2);
		}
		else if(InputRecord.Event.KeyEvent.wVirtualKeyCode == VK_S && InputRecord.Event.KeyEvent.bKeyDown == 1){ //shoot bawah
			shoot(robot, 3);
		}
		else if(InputRecord.Event.KeyEvent.wVirtualKeyCode == VK_D && InputRecord.Event.KeyEvent.bKeyDown == 1){ //shoot kanan
			shoot(robot, 4);
		}
		else if(InputRecord.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE && InputRecord.Event.KeyEvent.bKeyDown == 1){ // end program
			system("cls");
			if(killed>0)
				printf("Ah~ So you've out already\n\nYou have killed %d mutant cockroaches!\n\nHope to see you again soon!\n", killed);
			else
				printf("You gave up already when you haven't killed any'?\n\nHope to see you again soon with more power!\n");
			system("pause");
			exit(0);
		}
		
		clear(robot);
		deployKecoak(robot);
		arena(robot);
	}
	FlushConsoleInputBuffer(hinp);
}

int random(int upper, int lower){
	return (rand() % (upper - lower + 1)+(lower));
}

void ubah(int x, int y, int px, int py, int num, int* cont1, int* cont2)
{
	switch(num){
		case 1:
			if((px+1)==x && (py+1)==y){
				*cont1 = 0;
				*cont2 = 0;
			}
			else{
				*cont1 = px + 1;
				*cont2 = py + 1;
			}
			break;
		case 2:
			if((px-1)==x && (py-1)==y){
				*cont1 = 0;
				*cont2 = 0;
			}
			else{
				*cont1 = px - 1;
				*cont2 = py - 1;
			}
			break;
		case 3:
			if((px+1)==x && (py-1)==y){
				*cont1 = 0;
				*cont2 = 0;
			}
			else{
				*cont1 = px + 1;
				*cont2 = py - 1;
			}
			break;
		case 4:
			if((px-1)==x && (py+1)==y){
				*cont1 = 0;
				*cont2 = 0;
			}
			else{
				*cont1 = px - 1;
				*cont2 = py + 1;
			}
			break;
	}
}

void deployOne(int* cont1, int* cont2){
	int a = random(right-1, left+1);
	int b = random(bottom-1, top+1);
	while(a==robot.PlayerPos.X && b==robot.PlayerPos.Y){
		a = random(right-1, left+1);
		b = random(bottom-1, top+1);
	}
	*cont1 = a;
	*cont2 = b;
}

void displayPerKecoak(ROBOT &robot, int x, int y, int* contx, int* conty){
	int a, b;
	int container1(0), container2(0);
	
	if(x==0 || y==0 || x==right-1 || y==bottom-1){
		deployOne(&container1, &container2);
    	*contx = container1;
    	*conty = container2;
	}
	else{
		a = x + 1;
		b = y + 1;
		
		if(a==right-1 || a==1 || b==1 || b==bottom-1){
			*contx = 0;
			*conty = 0;
		}
		else{
			ubah(robot.PlayerPos.X, robot.PlayerPos.Y, x, y, random(4,1), &container1, &container2);
			*contx = container1;
			*conty = container2;
		}
	}
}

void deployKecoak(ROBOT &robot)
{
    clear(robot);
    
    int contx, conty;
    int a, b;
	int container1(0), container2(0);
    
    displayPerKecoak(robot, robot.Kecoak1.X, robot.Kecoak1.Y, &contx, &conty);
    robot.Kecoak1.X = contx;
    robot.Kecoak1.Y = conty;
	
	displayPerKecoak(robot, robot.Kecoak2.X, robot.Kecoak2.Y, &contx, &conty);
    robot.Kecoak2.X = contx;
    robot.Kecoak2.Y = conty;
    
    displayPerKecoak(robot, robot.Kecoak3.X, robot.Kecoak3.Y, &contx, &conty);
    robot.Kecoak3.X = contx;
    robot.Kecoak3.Y = conty;
    
    displayPerKecoak(robot, robot.Kecoak4.X, robot.Kecoak4.Y, &contx, &conty);
    robot.Kecoak4.X = contx;
    robot.Kecoak4.Y = conty;
    
    displayPerKecoak(robot, robot.Kecoak5.X, robot.Kecoak5.Y, &contx, &conty);
    robot.Kecoak5.X = contx;
    robot.Kecoak5.Y = conty;
    
	arena(robot);
	FlushConsoleInputBuffer(hinp);
}

void shoot(ROBOT &robot, int kodearah)
{	
	if(kodearah==1){ // W (atas)
		robot.PlayerBullet1.Y = robot.PlayerPos.Y - 1;
		robot.PlayerBullet1.X = robot.PlayerPos.X;
	}
	else if(kodearah==2){ // A (kiri)
		robot.PlayerBullet1.Y = robot.PlayerPos.Y;
		robot.PlayerBullet1.X = robot.PlayerPos.X - 1;
	}
	else if(kodearah==3){ // S (bawah)
		robot.PlayerBullet1.Y = robot.PlayerPos.Y + 1;
		robot.PlayerBullet1.X = robot.PlayerPos.X;
	}
	else if(kodearah==4){ // D (kanan)
		robot.PlayerBullet1.Y = robot.PlayerPos.Y;
		robot.PlayerBullet1.X = robot.PlayerPos.X + 1;
	}
	arena(robot);
	clear(robot);
	
	for(int i = 0; i < shoot_distance; i++){
		if(kodearah==1) // W (atas)
			robot.PlayerBullet1.Y = robot.PlayerPos.Y - 1;
		else if(kodearah==2) // A (kiri)
			robot.PlayerBullet1.X = robot.PlayerPos.X - 1;
		else if(kodearah==3) // S (bawah)
			robot.PlayerBullet1.Y = robot.PlayerPos.Y + 1;
		else if(kodearah==4) // D (kanan)
			robot.PlayerBullet1.X = robot.PlayerPos.X + 1;
		
		arena(robot);
		
		int move =	Command();
		switch(move){
		case 1: //Press W (tembak atas)
			shoot2(robot, kodearah);
			break;
		case 2: //Press A (tembak kiri)
			shoot2(robot, kodearah);
			break;
		case 3: //Press S (tembak kanan)
			shoot2(robot, kodearah);
			break;
		case 4: //Press D (tembak bawah)
			shoot2(robot, kodearah);
			break;
		case 5: // right
			clear(robot);
			if(robot.PlayerPos.X < right)
				robot.PlayerPos.X++;
			break;
		case 6: //left
			clear(robot);
			if(robot.PlayerPos.X > left)
				robot.PlayerPos.X--;
			break;
		case 7: //down
			clear(robot);
			if(robot.PlayerPos.Y < bottom)
				robot.PlayerPos.Y++;
			break;
		case 8: //up
			clear(robot);
			if(robot.PlayerPos.Y > top)
				robot.PlayerPos.Y--;
			break;
		}

		arena(robot);
		clear(robot);
		
		if(robot.PlayerBullet1.X == robot.Kecoak1.X && robot.PlayerBullet1.Y == robot.Kecoak1.Y){
			KecoakHealth[1]--;
			if(KecoakHealth[1]==0){
				robot.Kecoak1.X = 0;
				robot.Kecoak1.Y = 0;
				killed += 1;
				KecoakHealth[1] = 2;
				shot = true;
			}
		}
		else if (robot.PlayerBullet1.X == robot.Kecoak2.X && robot.PlayerBullet1.Y == robot.Kecoak2.Y){
			KecoakHealth[2]--;
			if(KecoakHealth[2]==0){
				robot.Kecoak2.X = 0;
				robot.Kecoak2.Y = 0;
				killed += 1;
				KecoakHealth[2] = 2;
				shot = true;
			}
		}
		else if (robot.PlayerBullet1.X == robot.Kecoak3.X && robot.PlayerBullet1.Y == robot.Kecoak3.Y){
			KecoakHealth[3]--;
			if(KecoakHealth[3]==0){
				robot.Kecoak3.X = 0;
				robot.Kecoak3.Y = 0;
				killed += 1;
				KecoakHealth[3] = 2;
				shot = true;
			}
		}
		else if (robot.PlayerBullet1.X == robot.Kecoak4.X && robot.PlayerBullet1.Y == robot.Kecoak4.Y){
			KecoakHealth[4]--;
			if(KecoakHealth[4]==0){
				robot.Kecoak4.X = 0;
				robot.Kecoak4.Y = 0;
				killed += 1;
				KecoakHealth[4] = 2;
				shot = true;
			}
		}
		else if (robot.PlayerBullet1.X == robot.Kecoak5.X && robot.PlayerBullet1.Y == robot.Kecoak5.Y){
			KecoakHealth[5]--;
			if(KecoakHealth[5]==0){
				robot.Kecoak5.X = 0;
				robot.Kecoak5.Y = 0;
				killed += 1;
				KecoakHealth[5] = 3;
				shot = true;
			}
		}
	}
	double jarak1 = distance(robot.PlayerPos.X, robot.PlayerPos.Y, robot.PlayerBullet1.X, robot.PlayerBullet1.Y);
	if(shot==true && (jarak1==21.0)){
		SetConsoleCursorPosition(hout, robot.Meleset);
		printf("SHOT!");
		shot=false;
	}
	else if(shot==false && (jarak1==21.0)){
		SetConsoleCursorPosition(hout, robot.Meleset);
		printf("MISS!");
	}
	robot.PlayerBullet1.X = 0;
	robot.PlayerBullet1.Y = 0;
	arena(robot);
}

void shoot2(ROBOT &robot, int kodearah){
	int xbegin, ybegin;
	if(kodearah==1){ // W (atas)
		robot.PlayerBullet2.Y = robot.PlayerPos.Y - 1;
		robot.PlayerBullet2.X = robot.PlayerPos.X;
	}
	else if(kodearah==2){ // A (kiri)
		robot.PlayerBullet2.Y = robot.PlayerPos.Y;
		robot.PlayerBullet2.X = robot.PlayerPos.X - 1;
	}
	else if(kodearah==3){ // S (bawah)
		robot.PlayerBullet2.Y = robot.PlayerPos.Y + 1;
		robot.PlayerBullet2.X = robot.PlayerPos.X;
	}
	else if(kodearah==4){ // D (kanan)
		robot.PlayerBullet2.Y = robot.PlayerPos.Y;
		robot.PlayerBullet2.X = robot.PlayerPos.X + 1;
	}
	arena(robot);
	clear(robot);
	
	for(int i = 0; i < shoot_distance; i++){
		if(kodearah==1){ // W (atas)
			robot.PlayerBullet1.Y -= 1;
			robot.PlayerBullet2.Y -= 1;
		}
		else if(kodearah==2){ // A (kiri)
			robot.PlayerBullet1.X -= 1;
			robot.PlayerBullet2.X -= 1;
		}
		else if(kodearah==3){ // S (bawah)
			robot.PlayerBullet1.Y += 1;
			robot.PlayerBullet2.Y += 1;
		}
		else if(kodearah==4){ // D (kanan)
			robot.PlayerBullet1.X += 1;
			robot.PlayerBullet2.X +=1;
		}
		
		arena(robot);

		int move =	Command();
		switch(move){
		case 5:
			clear(robot);
			if(robot.PlayerPos.X < right)
				robot.PlayerPos.X++;
			break;
		case 6:
			clear(robot);
			if(robot.PlayerPos.X > left)
				robot.PlayerPos.X--;
			break;
		case 7:
			clear(robot);
			if(robot.PlayerPos.Y < bottom)
				robot.PlayerPos.Y++;
			break;
		case 8:
			clear(robot);
			if(robot.PlayerPos.Y > top)
				robot.PlayerPos.Y--;
			break;
		}

		arena(robot);
		clear(robot);
		
		if((robot.PlayerBullet1.X == robot.Kecoak1.X && robot.PlayerBullet1.Y == robot.Kecoak1.Y) || (robot.PlayerBullet2.X == robot.Kecoak1.X && robot.PlayerBullet2.Y == robot.Kecoak1.Y)){
			KecoakHealth[1]--;
			if(KecoakHealth[1]==0){
				robot.Kecoak1.X = 0;
				robot.Kecoak1.Y = 0;
				killed += 1;
				KecoakHealth[1] = 2;
				shot = true;
			}
		}
		else if ((robot.PlayerBullet1.X == robot.Kecoak2.X && robot.PlayerBullet1.Y == robot.Kecoak2.Y) || (robot.PlayerBullet2.X == robot.Kecoak2.X && robot.PlayerBullet2.Y == robot.Kecoak2.Y)){
			KecoakHealth[2]--;
			if(KecoakHealth[2]==0){
				robot.Kecoak2.X = 0;
				robot.Kecoak2.Y = 0;
				killed += 1;
				KecoakHealth[2] = 2;
				shot = true;
			}
		}
		else if ((robot.PlayerBullet1.X == robot.Kecoak3.X && robot.PlayerBullet1.Y == robot.Kecoak3.Y) || (robot.PlayerBullet2.X == robot.Kecoak3.X && robot.PlayerBullet2.Y == robot.Kecoak3.Y)){
			KecoakHealth[3]--;
			if(KecoakHealth[3]==0){
				robot.Kecoak3.X = 0;
				robot.Kecoak3.Y = 0;
				killed += 1;
				KecoakHealth[3] = 2;
				shot = true;
			}
		}
		else if ((robot.PlayerBullet1.X == robot.Kecoak4.X && robot.PlayerBullet1.Y == robot.Kecoak4.Y) || (robot.PlayerBullet2.X == robot.Kecoak4.X && robot.PlayerBullet2.Y == robot.Kecoak4.Y)){
			KecoakHealth[4]--;
			if(KecoakHealth[4]==0){
				robot.Kecoak4.X = 0;
				robot.Kecoak4.Y = 0;
				killed += 1;
				KecoakHealth[4] = 2;
				shot = true;
			}
		}
		else if ((robot.PlayerBullet1.X == robot.Kecoak5.X && robot.PlayerBullet1.Y == robot.Kecoak5.Y) || (robot.PlayerBullet2.X == robot.Kecoak5.X && robot.PlayerBullet2.Y == robot.Kecoak5.Y)){
			KecoakHealth[5]--;
			if(KecoakHealth[5]==0){
				robot.Kecoak5.X = 0;
				robot.Kecoak5.Y = 0;
				killed += 1;
				KecoakHealth[5] = 3;
				shot = true;
			}
		}
	}
	double jarak1 = distance(robot.PlayerPos.X, robot.PlayerPos.Y, robot.PlayerBullet1.X, robot.PlayerBullet1.Y);
	double jarak2 = distance(robot.PlayerPos.X, robot.PlayerPos.Y, robot.PlayerBullet2.X, robot.PlayerBullet2.Y);
	if(shot==true && (jarak1==21.0 || jarak2==21.0)){
		SetConsoleCursorPosition(hout, robot.Meleset);
		printf("SHOT!");
		shot=false;
	}
	else if(shot==false && (jarak1==21.0 || jarak2==21.0)){
		SetConsoleCursorPosition(hout, robot.Meleset);
		printf("MISS!");
	}
	robot.PlayerBullet1.X = 0;
	robot.PlayerBullet1.Y = 0;
	robot.PlayerBullet2.X = 0;
	robot.PlayerBullet2.Y = 1;
	arena(robot);
}

int Command()
{	
	INPUT_RECORD InputRecord;
	DWORD Events = 0;
	
	if(WAIT_TIMEOUT == WaitForSingleObject(hinp,1))
				return 0;
	ReadConsoleInput(hinp, &InputRecord, 1, &Events);

	if(InputRecord.EventType == KEY_EVENT){
		if(InputRecord.Event.KeyEvent.wVirtualKeyCode == VK_W && InputRecord.Event.KeyEvent.bKeyDown == 1)
			return 1;
		if(InputRecord.Event.KeyEvent.wVirtualKeyCode == VK_A && InputRecord.Event.KeyEvent.bKeyDown == 1)
			return 2;
		if(InputRecord.Event.KeyEvent.wVirtualKeyCode == VK_S && InputRecord.Event.KeyEvent.bKeyDown == 1)
			return 3;
		if(InputRecord.Event.KeyEvent.wVirtualKeyCode == VK_D && InputRecord.Event.KeyEvent.bKeyDown == 1)
			return 4;
		if(InputRecord.Event.KeyEvent.wVirtualKeyCode == VK_RIGHT && InputRecord.Event.KeyEvent.bKeyDown == 1)
			return 5;
		if(InputRecord.Event.KeyEvent.wVirtualKeyCode == VK_LEFT && InputRecord.Event.KeyEvent.bKeyDown == 1)
			return 6;
		if(InputRecord.Event.KeyEvent.wVirtualKeyCode == VK_DOWN && InputRecord.Event.KeyEvent.bKeyDown == 1)
			return 7;
		if(InputRecord.Event.KeyEvent.wVirtualKeyCode == VK_UP && InputRecord.Event.KeyEvent.bKeyDown == 1)
			return 8;
	}
	FlushConsoleInputBuffer(hinp);
}

double distance(int x, int y, int x1, int y1){
	return sqrt((double)(((x1-x)*(x1-x))+((y1-y)*(y1-y))));
}
