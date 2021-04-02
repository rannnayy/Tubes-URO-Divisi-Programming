#include <bits/stdc++.h>

using namespace std;

typedef struct robot{
	int x, y, range, health;
}robot, Robot, *RobotPtr;

typedef struct kecoak{
	int x, y, health, att;
}kecoak, Kecoak, *KecoakPtr;

int random(int n, int r){
	int ans = rand() % n;
	while(ans == r){
		ans = rand() % n;
	}
	return ans;
}

void init_kecoak(kecoak **ke, robot *r, int n){
	kecoak *k = (kecoak*)malloc(sizeof(kecoak));
	k->x = random(n, r->x);
	k->y = random(n, r->y);
	k->health = 2;
	k->att = 1;
	*ke = k;
}

void menembak(robot **r, kecoak **k){
	robot *temp = *r;
	kecoak *temp1 = *k;
	if(temp->range+temp->x >= temp1->x && temp->range+temp->y >= temp1->y && (*k)->health != 1){
		(*k)->health--;
		return;
	}
	if((*k)->health == 1){
		free(*k);
		*k = 0;
		return;
	}
}

char ask_user(robot **r, kecoak **k, int n, int pts){
	char c;
	cout << "Instructions :\nShoot: 'm'\nMove: 'w' = upward, 's' = downward, 'a' = leftward, 'd' = rightward\n";
	cout << "command > "; 
	cin >> c;
	if(c == 'w'){
		(*r)->x = ((*r)->x - 1 + n) % n;
		return c;
	}
	else if(c == 's'){
		(*r)->x = ((*r)->x + 1) % n;
		return c;
	}
	else if(c == 'a'){
		(*r)->y = ((*r)->y - 1 + n) % n;
		return c;
	}
	else if(c == 'd'){
		(*r)->y = ((*r)->y + 1) % n;
		return c;
	}
	else if(c == 'm'){
		menembak(r, k);
		return c;
	}
	else if(c == 'e'){
		printf("error!");
		system("sleep 1");
		system("cls");
		return c;
	}
	return c;
}

void kecoak_mendekat(robot *r, kecoak **k, int n){
	kecoak *temp1 = *k;
	robot *temp = r;
	if(temp1->x > temp->x){
		(*k)->x = ((*k)->x - 1 + n) % n;
		return;
	}
	else if(temp1->x < temp->x){
		(*k)->x = ((*k)->x + 1 + n) % n;
  		return;
 	}
 	else if(temp1->y > temp->y){
  		(*k)->y = ((*k)->y - 1 + n) % n;
  		return;
 	}
 	else if(temp1->y < temp->y){
  		(*k)->y = ((*k)->y + 1 + n) % n;
  		return;
 	}
}

void kecoak_serang(robot **r, kecoak **k){
	robot *temp = *r;
	kecoak *temp1 = *k;
	if(abs(temp->x - temp1->x) == 1 || abs(temp->y - temp1->y) == 1){
		(*r)->health--;
	}
}

double distance(int x, int y, int x1, int y1){
	return sqrt((double)(((x1-x)*(x1-x))+((y1-y)*(y1-y))));
}

int main(){
	srand(time(0));
	int n = 20;
	char arr[n][n];
	for(int i = 0; i < n; ++i){
		for(int j = 0; j < n; ++j){
			arr[i][j] = '-';
		}
	}
	system("cls");
	cout << "Instructions : " << "\n";
	cout << "Use the keyboard key as follows:\n'w' key to move upward\n'a' key to move leftward\n's' key to move downwards\n'd' key to move rightward\n\n";
	cout << "Characters : \nR = Robot\nK = Kecoa\nHit range = 2 Hyphens ('-')\n\n";
	robot *r = (robot*)malloc(sizeof(robot));
	r->x = n-1;
	r->y = 0;
	r->health = 10;
	r->range = 2;
	kecoak *k;
	init_kecoak(&k, r, n);
	arr[r->x][r->y] = 'R';
	arr[k->x][k->y] = 'K';
	int pts = 0, p = 0;
	char c = '\0', ch;
	cout << "Want to Play (y/n)? ";
	cin >> ch;
	ch == 'y'? p = 1: p = 0;
	
	while(p){
		system("cls");
		if(c == 'm' && k == 0){   
			init_kecoak(&k, r, n);
			arr[k->x][k->y] = 'K';
			cout << "Kecoa: \"Oh no! You killed me!\"\n";
			cout << "Kecoa No." << pts+1 << "\n";
		}
		if(c == '\0')
			cout << "Kecoa No. " << pts+1 << "\n";
		
		if(k->health == 1 && c == 'm')
			cout << "SHOT!\n";
		else if(distance(r->x, r->y, k->x, k->y)>2.0 && c == 'm')
			cout << "MISS!\n";
		
		//kecoak menyerang
		kecoak_serang(&r, &k);
		
		//kecoa mendekat
		int px = k->x, py = k->y;
		kecoak_mendekat(r, &k, n);
		if(arr[k->x][k->y] != 'R'){
			arr[px][py] = '-';
			arr[k->x][k->y] = 'K';
		}
		else{
			k->x = px;
			k->y = py;
		}  
		
		if(r->health == 0)
			break;
		
		//printing
		for(int i = 0; i < n; ++i){
			for(int j = 0; j < n; ++j){
				cout << arr[i][j] << " ";
			}
			cout << "\n";
		}
		
		cout << "User health: " << r->health << "\n";  
		cout << "Kecoak health: " << k->health << "\n";
		cout << "Current position: (" << (r->y) << "," << n-(r->x) << ")\n";
		cout << "Enemy Distance: " << distance(r->x, r->y, k->x, k->y) << endl;
		
		//setting posisi dll
		int tx = r->x, ty = r->y; 
		px = k->x, py = k->y;
		c = ask_user(&r, &k, n, pts);
		if(c == 'w' || c == 'a' || c == 's' || c == 'd'){   
			while(arr[r->x][r->y] == 'K'){
				printf("There is a Kecoa near!\n");
				r->x = tx;
				r->y = ty;
				c = ask_user(&r, &k, n, pts);
			}
			arr[tx][ty] = '-';
			arr[r->x][r->y] = 'R';   
		}  
		if(c == 'm' && k == 0){
			arr[px][py] = '-';
			++pts;
		}
	}
	cout << "\n-- GAME OVER --\n";
	cout << "Number of Kecoa killed : " << pts << "\n"; 
	system("sleep 2");
	return 0;
}
