#include <unistd.h>
#include <termios.h>
#include <iostream>
#include <string>
#include <random>
#include <chrono>
#include <vector>
#include <sstream>
#include <time.h>
#include <cmath>

#include "unit_classes-1.2.2.h"
#include "terrainclasses-1.2.2.h"

#define HP_S 0
#define ATK_S 1
#define DEF_S 2
#define RANGE_S 3
#define MOVE_S 4

#define CODE_BLUE 34
#define CODE_RED 31
#define CODE_GREEN 32
#define CODE_YELLOW 33

#ifdef _WIN32
#include <conio.h>
	char singlechar()
	{
		char key = _getchar();
		return key;
	}
	void clearscreen()
	{
		for ( int c = 0 ; c < 49 ; c++ )
		{
			cout << endl;
		}
	}
#else
	char singlechar()
	{
		char buf = 0;
		struct termios old = {0};
		if (tcgetattr(0, &old) < 0)
		        perror("tcsetattr()");
		old.c_lflag &= ~ICANON;
		old.c_lflag &= ~ECHO;
		old.c_cc[VMIN] = 1;
		old.c_cc[VTIME] = 0;
		if (tcsetattr(0, TCSANOW, &old) < 0)
		        perror("tcsetattr ICANON");
		if (read(0, &buf, 1) < 0)
		        perror ("read()");
		old.c_lflag |= ICANON;
		old.c_lflag |= ECHO;
		if (tcsetattr(0, TCSADRAIN, &old) < 0)
		        perror ("tcsetattr ~ICANON");
		return (buf);
	}
	
	void clearscreen()
	{
		cout << "\033[2J\033[1;1H";  //the [2J part clears the screen, whereas the [1;1H part puts the cursor at 1,1.
					     //also, [31m is red, and [34m is blue, [0m is default color, and [4;#m will underline for #=31 or 34.
	}
#endif

using namespace std;

class Complex_maze;
class Dungeon_map;
class Battlefield;
class Game_field;

class Labyrinth
{
protected:
	int rows, cols;
	int Max;
	vector< vector<int> > maze;
	vector<int> wall_pos;
	vector<int> ht_rnd_g;
	int xcoor, ycoor, xcoor2, ycoor2;
	vector<int> pos_dir;
	int steps;
	static int steps2;
	int xfin, yfin, xstart, ystart;
	static double time_s;
	bool fin_at_start;
	bool overwrite;
	int ytest, xtest, postest;
	int playervalue;
	int movesused;
	int maxsteps;
	int tempval;
	vector<int> atksqrs;
	vector<int> movesqrs;
	string playername;

public:
	friend class Complex_maze;
	friend class Dungeon_map;
	friend class Battlefield;
	friend class Game_field;
	
	Labyrinth()
	{
	}

	Labyrinth(int rows2, int cols2, int y, int x) //constructor
		:rows(rows2), cols(cols2), xstart(x), ystart(y), ytest(0), xtest(0), postest(0), playervalue(-7), movesused(0), maxsteps(0)
	{
		overwrite = false;
		playername="Denver";
		tempval=0;
		if ((x >= 0) && (x < cols) && (y >= 0) && (y < rows))
		{
			if (x % 2 == 1)
			{
				++x;
			}
			if (y % 2 == 1)
			{
				++y;
			}
			xstart=x;
			ystart=y;
		}
		else
		{
			xstart=0;
			ystart=0;
		}
		Max = rows*cols;
		maze.reserve(Max+1);
//		vector< vector<int> > maze(cols, vector<int>(cols));
		for (int i = 0; i < rows; i++)
		{
			vector<int> row; // Create an empty row
			for (int j = 0; j < cols; j++)
			{
				row.push_back(0); // Add an element (column) to the row
			}
			maze.push_back(row); // Add the row to the main vector
		}
//		int ht_rnd_g [(Max + 1)] = {0};
		ht_rnd_g.reserve(Max+2);
		pos_dir.reserve(6);
		for (int counter=0; counter <= (Max+1); ++counter)
		{
			ht_rnd_g.push_back(0);
		}
		xcoor2 = xcoor, ycoor2 = ycoor;
//		int pos_dir [5] = {0};
		pos_dir.push_back(0);
		pos_dir.push_back(0);
		pos_dir.push_back(0);
		pos_dir.push_back(0);
		pos_dir.push_back(0);
		pos_dir.push_back(0);
		pos_dir.push_back(0);
		xcoor = xstart;
		ycoor = ystart;
		steps = 0;
		xfin = cols - 2;
		yfin = rows - 2;
		time_init();
		make();
	}

	virtual void construct(int rows2 = 22, int cols2 = 80, int y = 0, int x = 0) //constructor
//		:rows(rows2), cols(cols2), xstart(x), ystart(y)
	{
		overwrite = false;
		playername="Denver";
		tempval=0;
		ytest=0;
		xtest=0;
		postest=0;
		playervalue=-7;
		movesused=0;
		maxsteps=0;
		rows = rows2;
		cols = cols2;
		if ((x >= 0) && (x < cols) && (y >= 0) && (y < rows))
		{
			if (x % 2 == 1)
			{
				++x;
			}
			if (y % 2 == 1)
			{
				++y;
			}
			xstart=x;
			ystart=y;
		}
		else
		{
			xstart=0;
			ystart=0;
		}
		Max = rows*cols;
		maze.reserve(Max+1);
//		vector< vector<int> > maze(cols, vector<int>(cols));
		for (int i = 0; i < rows; i++)
		{
			vector<int> row; // Create an empty row
			for (int j = 0; j < cols; j++)
			{
				row.push_back(0); // Add an element (column) to the row
			}
			maze.push_back(row); // Add the row to the main vector
		}
//		int ht_rnd_g [(Max + 1)] = {0};
		ht_rnd_g.reserve(Max+2);
		pos_dir.reserve(6);
		for (int counter=0; counter <= (Max+1); ++counter)
		{
			ht_rnd_g.push_back(0);
		}
		xcoor2 = xcoor, ycoor2 = ycoor;
//		int pos_dir [5] = {0};
		pos_dir.push_back(0);
		pos_dir.push_back(0);
		pos_dir.push_back(0);
		pos_dir.push_back(0);
		pos_dir.push_back(0);
		pos_dir.push_back(0);
		pos_dir.push_back(0);
		xcoor = xstart;
		ycoor = ystart;
		steps = 0;
		xfin = cols - 2;
		yfin = rows - 2;
		time_init();
		make();
	}
	
	int rand1_4(minstd_rand0& rand2)
	{
		return (rand2() % 4) + 1;
	}

	int rand1_f(minstd_rand0& rand2)
	{
		return (rand2() % Max) + 1;
	}

	void dispv(int choice = 0)
	{
		for (int i = 0; i < rows; ++i)
		{
			for (int j = 0; j < cols; ++j)
			{
				if (choice == 0)
					cout << maze[i][j];
				else
					cout << maze[i][j] << " ";
			}
			cout << endl;
		}
	}

	virtual void dispm(bool flag=false,bool show_zr = true, bool flag2=false)
	{
		int rows9=rows;
		if ((flag) || (flag2))
			rows9=rows - 1;
		for (int i = 0; i < rows9; ++i)
		{
			for (int j = 0; j < cols; ++j)
				switch(maze[i][j])
				{
				case 0:
					if (show_zr == true)
						cout << ".";
					else
						cout << " ";
					break;
				case -1:
					cout << "|";
					break;
				case -6:
					cout << "D";
					break;
				case -7:
					cout << "D";
					break;
				case -8:
					cout << "F";
					break;
				case -9:
					cout << "S";
					break;
				case -10:
					cout << "S";
					break;
				case -11:
					cout << "R";
					break;
				case -12:
					cout << "H";
					break;
				case -13:
					cout << "X";
					break;
				case -14:
					cout << "A";
					break;
				case -15:
					cout << "M";
					break;
				case -16:
					cout << "D";
					break;
				case -17:
					cout << "K";
					break;
				case -18:
					cout << "W";
					break;
				case -19:
					cout << "E";
					break;
				case -20:
					cout << "B";
					break;
				case -21:
					cout << "C";
					break;
				default:
					cout << " ";
					break;
				}
			cout << endl;
		}
		if (flag2)
		{
			cout << "Your " << playername << " is at: <" << xcoor << "," << ycoor << "> and has: " << maxsteps-movesused << "/" << maxsteps << " steps remaining." << endl;
		}
		else
		{
			if (flag)
			{
				cout << "your position (row,col): (" << ycoor << "," << xcoor;
				cout << "); you have taken: " << steps2 << " steps for a " << steps << "-step maze" << endl;
			}
		}
	}

	void setmaxsteps(int max)
	{
		maxsteps=max;
	}
	
	void init()
	{
		for (int k = 1, l = 0, m = 0, e = 0; k < Max; k = k + 2)
		{
			l = k / cols;
			m = k % cols;
			e = l % 2;
			if (m == 0)
				++k;
			maze[l][m] = -1;
			if (e == 1)
				maze[l][m-1] = -1;
			else
				maze[l][m-1] = 0;
		}
		maze[ystart][xstart] = 1;
	}

	virtual void test_dir(int dir, int testfor, bool allow_f2 = false, bool zerosp = true, bool not_wall = true)			// make virtual		(done)
	{
		if (testfor == 0)
		{
			if (zerosp == true)
			{
				if (maze[ycoor2][xcoor2] == 0)
					pos_dir[dir] = 1;
				else
					pos_dir[dir] = 0;
			}
			else
			{
				if (maze[ycoor2][xcoor2] >= 0)
					pos_dir[dir] = 1;
				else
					pos_dir[dir] = 0;
			}
		}
		else if (testfor == -1)
		{
			if (not_wall==true)
			{
				if (maze[ycoor2][xcoor2] != -1)
					pos_dir[dir] = 1;
				else
					pos_dir[dir] = 0;
			}
			else
			{
				if (maze[ycoor2][xcoor2] >= -1)
					pos_dir[dir] = 1;
				else
					pos_dir[dir] = 0;
			}
		}
		else
		{
			if (maze[ycoor2][xcoor2] >= testfor)
				pos_dir[dir] = 1;
			else
				pos_dir[dir] = 0;
		}
		if ((allow_f2) && (maze[ycoor2][xcoor2] == -8))
		{
			pos_dir[dir]=1;
		}
		if ((allow_f2) && (maze[ycoor2][xcoor2] == -9))
		{
			pos_dir[dir]=1;
			pos_dir[5]=ycoor2;
			pos_dir[6]=xcoor2;
		}
	}

	virtual int test_dirs(int testfor0 = 0, int num_sq = 2, bool allow_f = false, bool zero_sp = true, bool not_wall = true)     // make virtual (actually, shouldn't need to)
	{
		xcoor2 = xcoor;
		ycoor2 = ycoor;
		for (int dir0 = 1; dir0 < 5; ++dir0)
		{
			xcoor2 = xcoor;
			ycoor2 = ycoor;
			if ((xcoor >= 0) && (xcoor < cols) && (ycoor >= 0) && (ycoor < rows))
			{
				switch(dir0)
				{
				case 1:
					if ((ycoor - num_sq) >= 0) //test up
					{
						ycoor2 = ycoor - num_sq;
						test_dir(dir0,testfor0,allow_f,zero_sp,not_wall);
					}
					else
						pos_dir[dir0] = 0;
					break;
				case 2:
					if ((xcoor + num_sq) <= (cols - 1)) //test right
					{
						xcoor2 = xcoor + num_sq;
						test_dir(dir0,testfor0,allow_f,zero_sp,not_wall);
					}
					else
						pos_dir[dir0] = 0;
					break;
				case 3:
					if ((ycoor + num_sq) <= (rows - 1)) //test down
					{
						ycoor2 = ycoor + num_sq;
						test_dir(dir0,testfor0,allow_f,zero_sp,not_wall);
					}
					else
						pos_dir[dir0] = 0;
					break;
				case 4:
					if ((xcoor - num_sq) >= 0) //test left
					{
						xcoor2 = xcoor - num_sq;
						test_dir(dir0,testfor0,allow_f,zero_sp,not_wall);
					}
					else
						pos_dir[dir0] = 0;
					break;
				}
			}
		}

		pos_dir[0] = pos_dir[1] + pos_dir[2] + pos_dir[3] + pos_dir[4];
		return pos_dir[0];
	}

	void find_max()
	{
		for (int i = 0; i < rows; ++i)
		{
			for (int j = 0; j < cols; ++j)
			{
				if (maze[i][j] > steps)
				{
					steps = maze[i][j];
					xfin = j;
					yfin = i;
				}
			}
		}
	}
	
	void set_fin(int y,int x)
	{
		if ((x >= 0) && (x < cols) && (y >= 0) && (y < rows))
		{
			if (maze[yfin][xfin] == -8)
			{
				maze[yfin][xfin]=steps;
			}
			xfin=x;
			yfin=y;
			maze[yfin][xfin]=-8;
		}	
	}

	virtual void set_start(int y,int x,bool erase=true)		// make virtual 	(done)
	{
		if ((x >= 0) && (x < cols) && (y >= 0) && (y < rows))
		{
			if (erase)
			{
				if (maze[ycoor][xcoor] == playervalue)
				{
					maze[ycoor][xcoor]=1;
				}
			}
			xstart=x;
			ystart=y;
			xcoor=x;
			ycoor=y;
			maze[ycoor][xcoor]=playervalue;
		}
	}

	void set_stair(int y,int x)
	{
		if ((x >= 0) && (x < cols) && (y >= 0) && (y < rows))
		{
			maze[y][x]=-9;
		}
	}

	void reset_hunt()
	{
		for( int counter = 0 ; counter <= Max ; ++counter )
		{
			ht_rnd_g[counter]=0;
		}
	}

	void make()
	{
		unsigned seed1 = chrono::system_clock::now().time_since_epoch().count();
		minstd_rand0 rand (seed1);
		init();
		test_dirs(0);
		int count = 1, num = 0;
		while (count <= Max)
		{
			num = rand1_f(rand);
			if (ht_rnd_g[num] == 0)
			{
				ht_rnd_g[num] = count;
				++count;
			}
		}
		int loop1 = 0;
		bool fin = true;
		bool hunt = true;
		bool hunt2 = true;
		int drctn = 2;
		int mazenum = 0;
		while (fin)
		{
			test_dirs(0);
			hunt = true;
			loop1 = 0;
			while (hunt)
			{
				drctn = rand1_4(rand);
				if (pos_dir[drctn] == 1)
				{
					switch(drctn)
					{
					case 1: //up
						maze[ycoor - 1][xcoor] = maze[ycoor][xcoor] + 1;
						maze[ycoor - 2][xcoor] = maze[ycoor][xcoor] + 2;
						ycoor = ycoor - 2;
						break;
					case 2: //right
						maze[ycoor][xcoor + 1] = maze[ycoor][xcoor] + 1;
						maze[ycoor][xcoor + 2] = maze[ycoor][xcoor] + 2;
						xcoor = xcoor + 2;
						break;
					case 3: //down
						maze[ycoor + 1][xcoor] = maze[ycoor][xcoor] + 1;
						maze[ycoor + 2][xcoor] = maze[ycoor][xcoor] + 2;
						ycoor = ycoor + 2;
						break;
					case 4: //left
						maze[ycoor][xcoor - 1] = maze[ycoor][xcoor] + 1;
						maze[ycoor][xcoor - 2] = maze[ycoor][xcoor] + 2;
						xcoor = xcoor - 2;
						break;
					}
				}
				test_dirs(0);
				if (pos_dir[0] == 0)
					hunt = false;
	//			cout << "stuck in loop 1" << endl;
				++loop1;
				if (loop1 > 100)
				{
	//				cout << "reseeding..." << endl;
					seed1 = chrono::system_clock::now().time_since_epoch().count();
					rand.seed(seed1);
					loop1 = 0;
					break;
				}
			}
			count = 1;
			num = ht_rnd_g[count];
			ycoor = (num - 1) / cols;
			xcoor = num - (ycoor*cols) - 1;
			hunt2 = true;
			while ((hunt2) && (count <= Max))
			{
				if (maze[ycoor][xcoor] != 0)
				{
					++count;
					num = ht_rnd_g[count];
					ycoor = (num - 1) / cols;		//changed
					xcoor = num - (ycoor*cols) - 1;
				}
				else
				{
					test_dirs(1);
					if (pos_dir[0] > 0)
					{

						hunt2 = false;
						maze[ycoor][xcoor] = 3;
						if (pos_dir[1] == 1)
						{
							maze[ycoor - 1][xcoor] = maze[ycoor - 2][xcoor] + 1;
							maze[ycoor][xcoor] = maze[ycoor - 2][xcoor] + 2;
							break;
						}
						else if (pos_dir[2] == 1)
						{
							maze[ycoor][xcoor + 1] = maze[ycoor][xcoor + 2] + 1;
							maze[ycoor][xcoor] = maze[ycoor][xcoor + 2] + 2;
							break;
						}
						else if (pos_dir[3] == 1)
						{
							maze[ycoor + 1][xcoor] = maze[ycoor + 2][xcoor] + 1;
							maze[ycoor][xcoor] = maze[ycoor + 2][xcoor] + 2;
							break;
						}
						else
						{
							maze[ycoor][xcoor - 1] = maze[ycoor][xcoor - 2] + 1;
							maze[ycoor][xcoor] = maze[ycoor][xcoor - 2] + 2;
						}
					}
					else
					{
						++count;
						num = ht_rnd_g[count];
						ycoor = (num - 1) / cols;		//changed
						xcoor = num - (ycoor*cols) - 1;
					}
				}
	//			cout << "stuck in loop 2" << endl;
			}
			++mazenum;
	//		cout << "maze " << mazenum << endl;
			if (count > Max)
				fin = false;
		}
		xcoor = xstart;
		ycoor = ystart;
		set_start(ystart,xstart);
		find_max();
		set_fin(yfin,xfin);
		--steps;
	}
	
	void coors2pos(int y, int x)
	{
		postest = y*cols + x;
	}
	
	void pos2coors(int pos)
	{
		xtest = pos % cols;
		ytest = pos / cols;
	}
	
	void find_walls()
	{
		vector<int> temp;
		for ( int y = 0 ; y < rows - 1 ; y++ )
		{
			for ( int x = 0 ; x < cols - 1 ; x++ )
			{
				if ( maze[y][x] == -1 )
				{
					coors2pos(y,x);
					temp.push_back(postest);
				}
			}
		}
		wall_pos = temp;
	}
	
	int min_p(int a1, int a2, int a3, int a4)
	{
		if (a1 == 0)
			a1 = 1000;
		if (a2 == 0)
			a2 = 1000;
		if (a3 == 0)
			a3 = 1000;
		if (a4 == 0)
			a4 = 1000;
		int dir7 = 1;
		int min0 = (a1 > a2)? a2 : a1;
		min0 = (min0 > a3)? a3 : min0;
		min0 = (min0 > a4)? a4 : min0;
		if (min0 == a1)
			dir7=1;
		if (min0 == a2)
			dir7=2;
		if (min0 == a3)
			dir7=3;
		if (min0 == a4)
			dir7=4;
		return dir7;
	}

	void make_path()
	{
		xcoor = xfin;
		ycoor = yfin;
		maze[ycoor][xcoor]=0;
		int dir1;
		int dir2;
		int dir3;
		int dir4;
		int drctn = 1;
		test_dirs(1,1);
		while ((xcoor != xstart) || (ycoor != ystart))
		{
			if (pos_dir[1] == 1)
				dir1 = maze[ycoor - 1][xcoor];
			else
				dir1 = 1000;
			if (pos_dir[2] == 1)
				dir2 = maze[ycoor][xcoor + 1];
			else
				dir2 = 1000;
			if (pos_dir[3] == 1)
				dir3 = maze[ycoor + 1][xcoor];
			else
				dir3 = 1000;
			if (pos_dir[4] == 1)
				dir4 = maze[ycoor][xcoor - 1];
			else
				dir4 = 1000;
	//		cout << dir1 << " " << dir2 << " " << dir3 << " " << dir4 << endl;
			drctn = min_p(dir1,dir2,dir3,dir4);
	//		cout << drctn << " " << pos_dir[drctn] << endl;
	//		cout << pos_dir[1] << " " << pos_dir[2] << " " << pos_dir[3] << " " << pos_dir[4] << endl;
			if (pos_dir[drctn] == 1)
			{
				switch(drctn)
				{
				case 1: //up
					maze[ycoor - 1][xcoor] = 0;
					ycoor = ycoor - 1;
					break;
				case 2: //right
					maze[ycoor][xcoor + 1] = 0;
					xcoor = xcoor + 1;
					break;
				case 3: //down
					maze[ycoor + 1][xcoor] = 0;
					ycoor = ycoor + 1;
					break;
				case 4: //left
					maze[ycoor][xcoor - 1] = 0;
					xcoor = xcoor - 1;
					break;
				}
			}
			test_dirs(1,1);
			if (pos_dir[0] == 0)
				ycoor = ystart, xcoor = xstart;
	//		dispm();
	//		cout << "(" << xcoor << "," << ycoor << ")" << endl;
		}
		xcoor = xstart;
		ycoor = ystart;
		maze[ystart][xstart] = playervalue;
		maze[yfin][xfin] = -8;
		--steps;
	}
	
	char getch()
	{
		return singlechar();
	}
	
	char game_char ()
	{
		int x = 'a';
		char direc;
		int f=getch();
		if (f==27)
		{
			x=getch();
			if (x==91)
			{
				x=getch();
			}
			switch(x)
			{
				case 65:
					direc='w';
					break;
				case 68:
					direc='a';
					break;
				case 67:
					direc='d';
					break;
				case 66:
					direc='s';
			}
	//		cout << "worked: " << x << endl;
		}
		else
		{
		direc=f;
		}	
	//	cout << direc << endl;
		return (direc);
	}
	
	void time_init()
	{
		time_s=0;
	}

	void setplayer(int value)
	{
		playervalue=value;
	}
	
	virtual void play(bool quiet=false, bool start_stair=false, bool show_d=false, int max_moves=-1, bool flag3=false)
	{
		maxsteps=max_moves;
		movesused=0;
		dispm(true,show_d,flag3);
		char direction=game_char();
		stringstream ss;
		auto start = chrono::system_clock::now().time_since_epoch().count();
		xcoor=xstart;
		ycoor=ystart;
		bool first_move=true;
		bool unf=true;
		int move_counter=0;
		int step_c=1;
//		if (quiet==false)
//		{
//			time_init();
//		}
		while (unf)
		{
			test_dirs(0,1,true,false);
			if ((direction=='w') && (pos_dir[1]==1))
			{
				maze[ycoor][xcoor]=step_c;
				ycoor=ycoor-1;
				step_c=maze[ycoor][xcoor];
				maze[ycoor][xcoor]=playervalue;
				++steps2;
				move_counter++;
			}
			if ((direction=='d') && (pos_dir[2]==1))
			{
				maze[ycoor][xcoor]=step_c;
				xcoor=xcoor+1;
				step_c=maze[ycoor][xcoor];
				maze[ycoor][xcoor]=playervalue;
				++steps2;
				move_counter++;
			}
			if ((direction=='s') && (pos_dir[3]==1))
			{
				maze[ycoor][xcoor]=step_c;
				ycoor=ycoor+1;
				step_c=maze[ycoor][xcoor];
				maze[ycoor][xcoor]=playervalue;
				++steps2;
				move_counter++;
			}
			if ((direction=='a') && (pos_dir[4]==1))
			{
				maze[ycoor][xcoor]=step_c;
				xcoor=xcoor-1;
				step_c=maze[ycoor][xcoor];
				maze[ycoor][xcoor]=playervalue;
				++steps2;
				move_counter++;
			}
			if (direction=='p')
			{
				if (show_d == false)
					show_d=true;
				else
					show_d=false;
			}
			if (direction=='q')
			{
				unf=false;
			}
			if ((xcoor==xfin) && (ycoor==yfin))
			{
				unf=false;
				 fin_at_start=false;
			}
			if ((xcoor==xstart) && (ycoor==ystart) && (start_stair) && (first_move==false))
			{
				unf=false;
				fin_at_start=true;
			}
			if (max_moves>0)
			{
				if (move_counter==max_moves)
				{
					unf=false;
					movesused=move_counter;
				}
			}
//			if (((xcoor==pos_dir[6]) && (ycoor==pos_dir[5])) && (maze[ycoor][xcoor]==-9))
//			{
//				unf=false;
//				
//			}
			if ((start_stair) && ((first_move==true) && ((xcoor!=xstart) || (ycoor!=ystart))))
			{
				first_move=false;
				maze[ystart][xstart]=-9;
			}
			clear();
			movesused=move_counter;
			dispm(true,show_d,flag3);
			if (unf)
			{
				direction=game_char();
			}
//			echo(true);
		}
//finished playing maze:
		auto end = chrono::system_clock::now().time_since_epoch().count();
		auto time= end - start;
		stringstream ss9;
		ss9 << time;
		double time2;
		ss9 >> time2;
		time_s=time_s + (time2 / 1000000);
		if (quiet==false)
		{
			echo(false,true);
		}
		movesused = move_counter;
	}
	
	int getmovesused()
	{
		return movesused;
	}
	
	void setname(string name)
	{
		playername=name;
	}
	
	void oldclear()
	{
		for ( int c = 0 ; c < 50 ; ++c)
		{
			cout << endl;
		}
	}

	void clear()
	{
		clearscreen();
	}
	
	void echo (bool flag=false, bool flag2=false)
	{
		if (flag2)
		{
			cout << endl << endl << endl << "you finished the maze!" << endl;
			cout << "you took: " << steps2 << " steps to complete a: " << steps << "-step maze" << endl;
			cout << "it also took you: " << time_s << " seconds to finish the maze" << endl;
		}
		else
		{
			if (flag)
			{
				cout << "your position (row,col): (" << ycoor << "," << xcoor;
				cout << "); you have taken: " << steps2 << " steps for a " << steps << "-step maze";
			}
			else
			{
				cout << "xfin;yfin;steps;rows;cols" << endl;
				cout << xfin << ";" << yfin << ";" << steps << ";" << rows << ";" << cols << endl;
			}
		}
	}

	void rm_sf()
	{
		maze[ystart][xstart]=1;
		maze[yfin][xfin]=1;
	}
	
	int count_walls()
	{
		int num_w=0;
		for ( int y = 0 ; y < rows - 1 ; y++ )
		{
			for ( int x = 0 ; x < cols - 1 ; x++ )
			{
				if ( maze[y][x] == -1 )
				{
					num_w++;
				}
			}
		}
		return num_w;
	}
	
	void refresh()
	{
		rows = maze.size();
		cols = maze[0].size();
		Max = rows*cols;
	}
	
	void fix_edges()
	{
		for ( int c = 0 ; c < cols ; c++ )
		{
			maze[rows-1][c]=-1;
		}
		for ( int r = 0 ; r < rows ; r++ )
		{
			maze[r][cols-1]=-1;
		}
	}
	
	void add_feature(int type,int y, int x, int len, int width,int fill=1)
	{
		if ( type == 0 )	//square or rectangle --- (y,x) is at top-left
		{
			if (y < 0)
			{
				y=0;
			}
			else if (y >= rows)
			{
				y=rows-1;
			}
			if ( x < 0)
			{
				x=0;
			}
			else if (x >= cols)
			{
				x=cols-1;
			}
			for ( int c = 0 ; c < len ; c++ )
			{
				for ( int k = 0 ; k < width ; k++ )
				{
					if ((y+c < rows) && (x+k < cols))
					{
						maze[y+c][x+k]=fill;
					}
				}
			}
		}
	}
	
	virtual void resetspaces(int fill=1)		// make virtual		(done)
	{
		for ( int r = 0; r < rows ; r++ )
		{
			for ( int c = 0 ; c < cols ; c++ )
			{
				if (maze[r][c] >= 0)
				{
					maze[r][c]=fill;
				}
			}
		}
	}
	
	virtual int moveablesqrs(int y, int x, int move, bool flying=false,int recursion=0)
	{
		if (flying==true)
		{
			attackablesqrs(y,x,move,false);
		}
		else
		{
			if (recursion==0)
			{
				resetspaces();
				tempval=0;
			}
			if (move==0)
			{
				return tempval;
			}
			ycoor=y;
			xcoor=x;
			int numdirs = test_dirs(1,-1);
			vector<int> tempvec = pos_dir;
			if (numdirs==0)
			{
				return tempval;
			}
			else
			{
				if (tempvec[1]==1)
				{
					if (maze[y-1][x]==1)
					{
						maze[y-1][x]=0;
					}
					tempval++;
					recursion++;
					moveablesqrs(y-1,x,move-1,false,recursion);
				}
				if (tempvec[2]==1)
				{
					if (maze[y][x+1]==1)
					{
						maze[y][x+1]=0;
					}
					tempval++;
					recursion++;
					moveablesqrs(y,x+1,move-1,false,recursion);
				}
				if (tempvec[3]==1)
				{
					if (maze[y+1][x]==1)
					{
						maze[y+1][x]=0;
					}
					tempval++;
					recursion++;
					moveablesqrs(y+1,x,move-1,false,recursion);
				}
				if (tempvec[4]==1)
				{
					if (maze[y][x-1]==1)
					{
						maze[y][x-1]=0;
					}
					tempval++;
					recursion++;
					moveablesqrs(y,x-1,move-1,false,recursion);
				}
			}
		}
		return tempval;
	}

	virtual void attackablesqrs(int y, int x, int range, bool needclear=true) //make virtual, or just move to Game_field		(done)
	{
		if (range==1)
		{
			needclear=false;
		}
		vector<int> temp;
		atksqrs = temp;
		resetspaces();
		int ymin = y-range;
		int ymax = y+range;
		int xmin = x-range;
		int xmax = x+range;
		if (ymin < 0)
		{
			ymin=0;
		}
		if (ymax > rows-1)
		{
			ymax = rows-1;
		}
		if (xmin < 0)
		{
			xmin=0;
		}
		if (xmax > cols-1)
		{
			xmax=cols-1;
		}
		for ( int y0 = ymin ; y0 <= ymax ; y0++ )
		{
			for ( int x0 = xmin ; x0 <= xmax ; x0++ )
			{
				if ((abs(y-y0)+abs(x-x0)) <= range)
				{
					if (needclear)
					{
						if ((clearpath(y,x,y0,x0)) && (maze[y0][x0]!=-1))
						{
							coors2pos(y0,x0);
							atksqrs.push_back(postest);
							if (maze[y0][x0]==1)
							{
								maze[y0][x0]=0;
							}
						}
					}
					else
					{
						if (maze[y0][x0]!=-1)
						{
							coors2pos(y0,x0);
							atksqrs.push_back(postest);
							if (maze[y0][x0]==1)
							{
								maze[y0][x0]=0;
							}
						}
					}
				}
			}
		}
	}
	
	int abs(int a)
	{
		if (a<0)
		{
			return -1*a;
		}
		else
		{
			return a;
		}
	}
	
	int round(double dec)
	{
		return ceil(dec - 0.5);
	}
	
	double point_slope(double y1, double x1,double rise, double run, double xin) // y - y1 = m * (x - x1)
	{
		double f = rise / run;
		f = ( f * ( xin - x1) ) + y1;  //   (run / rise)*(f - y1) + x1 = xin 
		return f;
	}
	
	virtual bool clearpath(int y1, int x1, int y2, int x2)		//make virtual		(done)
	{
//		cout << "--------------------------" << endl << "checking a new line" << endl << "--------------------------" << endl;
		double f_x=0.0;
		int r_y=0;
		vector<int> pos2check;
		if (x1==x2)
		{
			if (y1>y2)
			{
				int temp = y1;
				y1 = y2;
				y2 = temp;
			}
			for ( int y0 = y1 ; y0 < y2 ; y0++ )
			{
				coors2pos(y0,x1);
				pos2check.push_back(postest);
			}
		}
		else if (y1==y2)
		{
			if (x1>x2)
			{
				int temp = x1;
				x1 = x2;
				x2 = temp;
			}
			for ( int x0 = x1 ; x0 < x2 ; x0++ )
			{
				coors2pos(y1,x0);
				pos2check.push_back(postest);
			}
		}
		else
		{
			if (x1>x2) //switch so x1 is smaller
			{
				int temp = x1;
				x1 = x2;
				x2 = temp;
				temp = y1;
				y1 = y2;
				y2 = temp;
			}
			int rise = y2 - y1;
			int run = x2 - x1;
//			cout << "checking for a clear path between (" << x1 << "," << y1 << ") and (" << x2 << "," << y2 << ")" << endl;
//			cout << "Rise: " << rise << "; Run: " << run << endl;
//			cout << "now creating the list of positions to check..." << endl << "----------------------------------------" << endl;
			for ( int x0 = x1 ; x0 < x2 ; x0++ )
			{
				f_x=point_slope(y1,x1,rise,run,x0);
//				cout << "f(" << x0 << ") = " << f_x << endl;
				r_y = ceil(f_x);
//				cout << "round-up(f(x)) = " << r_y << endl;
				coors2pos(r_y,x0);
				pos2check.push_back(postest);
				r_y=floor(f_x);
//				cout << "round-down(f(x)) = " << r_y << endl;
				coors2pos(r_y,x0);
				pos2check.push_back(postest);
			}
			//---------------
			if (y1>y2)
			{
				int temp = x1;
				x1 = x2;
				x2 = temp;
				temp = y1;
				y1 = y2;
				y2 = temp;
			}
			rise = y2 - y1;
			run = x2 - x1;
			for ( int y0 = y1 ; y0 < y2 ; y0++ )
			{
				f_x=point_slope(x1,y1,run,rise,y0);
				r_y=ceil(f_x);
				coors2pos(y0,r_y);
				pos2check.push_back(postest);
				r_y=floor(f_x);
				coors2pos(y0,r_y);
				pos2check.push_back(postest);
			}
			//---------------
		}
//		cout << "now checking the positions for walls... size of list to check: " << pos2check.size() << endl;
//		cout << "============================================" << endl;
//		cout << pos2check.size() << endl;
		for ( int c = 1 ; c < pos2check.size() ; c++ )
		{
			pos2coors(pos2check[c]);
//			cout << "Y-test = " << ytest << endl;
//			cout << "X-test = " << xtest << endl;
//			cout << "maze[ytest][xtest] = " << maze[ytest][xtest] << endl;
			if (maze[ytest][xtest]==-1)
			{
				return false;
			}
		}
		return true;
	}

};

int Labyrinth::steps2=0;
double Labyrinth::time_s=0;

class Dungeon_map
{
protected:
	Labyrinth total_maze, temp_maze, vtemp_maze;
	int t_rows, t_cols;
	int m_rows, m_cols;
	int temp_rows, temp_cols;
	int row_count, counter1;
	int min_f, max_f, num_f;
public:
	Dungeon_map(int r1=22, int c1=80, int mn=4, int mx = -1)
		:t_rows(r1),t_cols(c1), min_f(mn), max_f(mx)
	{
		int this_rows;
		counter1=2;
		m_rows=10;
		m_cols=20;
		row_count = 0;
		if (t_rows < 22)
		{
			t_rows=22;
		}
		if (t_cols < 80)
		{
			t_cols=80;
		}
		temp_rows = t_rows;
		while (temp_rows > 2)
		{
			if ( temp_rows >= (2 * m_rows))
			{
				this_rows = m_rows + 2;
			}
			else
			{
				this_rows = temp_rows;
			}
			temp_cols = t_cols - m_cols;
			vtemp_maze = Labyrinth(this_rows,m_cols+2,0,0);
			vtemp_maze.rm_sf();
			counter1=2;
			while (temp_cols >= (2 * m_cols))
			{
				temp_maze = Labyrinth(this_rows,m_cols+2,0,0);
				temp_maze.rm_sf();
				vtemp_maze = join_h(vtemp_maze,temp_maze);
				temp_cols = temp_cols - m_cols;
				++counter1;
			}
			temp_maze = Labyrinth(this_rows,temp_cols,0,0);
			temp_maze.rm_sf();
			vtemp_maze = join_h(vtemp_maze,temp_maze);
			temp_rows = temp_rows - this_rows + 2;
			if ( row_count == 0)
			{
				total_maze.overwrite = true;
			}
			total_maze = join_v(total_maze,vtemp_maze);
			if ( row_count == 0)
			{
				total_maze.overwrite = false;
			}
			++row_count;
		}
		if ( max_f == -1 )
		{	
			num_f = num_btwn(min_f,row_count+counter1);
		}
		else
		{
			num_f = num_btwn(min_f,max_f);
		}
		int t_walls = total_maze.count_walls();
		int num_w = t_walls / 3;
		total_maze = add_rooms(total_maze, num_f);
		int check = total_maze.count_walls();
//		cout << check << ";" << num_w << endl;
		while ( check >= (2*num_w) )
		{
			total_maze = add_rooms(total_maze, 1);
			check = total_maze.count_walls();
// 			cout << check << ";" << num_w << endl;
		}
		total_maze = rm_walls(total_maze, num_w);
		total_maze.fix_edges();
//		total_maze.dispm();
	}
	
	Labyrinth get_map()
	{
		return total_maze;
	}
	
	Labyrinth rm_walls(Labyrinth a, int n=-1)
	{
		a.find_walls();
		int n_walls = a.wall_pos.size();
		if ( n <= 0)
		{
			n = n_walls / 4;
		}
		else if ( n >= n_walls )
		{
			for ( int y = 0 ; y < a.rows - 1 ; y++ )
			{
				for ( int x = 0 ; x < a.cols - 1 ; x++ )
				{
					if ( a.maze[y][x] == -1 )
					{
						a.maze[y][x] = 1;
					}
				}
			}
			return a;
		}
		for ( int c = 0 ; c < n ; c++ )
		{
			int k = num_btwn(0,a.wall_pos.size()-1);
			a.pos2coors(a.wall_pos[k]);
			a.maze[a.ytest][a.xtest]=1;
			a.wall_pos.erase(a.wall_pos.begin()+k);
		}
		return a;
	}

	Labyrinth add_rooms(Labyrinth a, int n=1)
	{
		int type, y, x, len, width;
		for ( int k = 0 ; k < n ; k++ )
		{
			type = num_btwn(1,5);
			if ( type == 1 )
			{
				len = num_btwn(2,3);
				width = num_btwn(10,a.cols+5);
				y = num_btwn(0,a.rows-4);
				x = num_btwn(0,a.cols-11);
			}
			else if ( type == 5 )
			{
				len = num_btwn(10,a.rows+5);
				width = num_btwn(2,3);
				y = num_btwn(0,a.rows-11);
				x = num_btwn(0,a.cols-4);
			}
			else
			{
				len = num_btwn(4,(a.rows)/4+3);
				width = num_btwn(6,((a.cols * a.rows) / (20 * len)));
				y = num_btwn(0,a.rows-len);
				x = num_btwn(0,a.cols-width);
			}
			a.add_feature(0,y,x,len,width);
		}
		return a;
	}
	
	Labyrinth join_h(Labyrinth a, Labyrinth b, int fill = 1)
	{
		if (b.rows > a.rows)
		{
			for( int r0 = a.rows ; r0 < b.rows ; ++r0 )
			{
				vector<int> row; // Create an empty row
				for (int j = 0; j < a.cols ; j++)
				{
					row.push_back(fill); // Add an element (column) to the row
				}
				a.maze.push_back(row); // Add the row to the main vector
			}
//			a.rows=b.rows;
		}
		else if (a.rows > b.rows )
		{
			for( int r0 = b.rows ; r0 < a.rows ; ++r0 )
			{
				vector<int> row; // Create an empty row
				for (int j = 0; j < b.cols ; j++)
				{
					row.push_back(fill); // Add an element (column) to the row
				}
				b.maze.push_back(row); // Add the row to the main vector
			}
//			b.rows=a.rows;
		}
		a.refresh();
		b.refresh();
		for ( int r1 = 0 ; r1 < a.rows ; ++r1 )
		{
			a.maze[r1][a.cols - 2] = wall_or(b.maze[r1][0],a.maze[r1][a.cols - 2]);
			a.maze[r1][a.cols - 1] = b.maze[r1][1];
			for ( int c1 = 2 ; c1 < b.cols ; ++c1 )
			{
				a.maze[r1].push_back(b.maze[r1][c1]);
			}
		}
//		a.cols=a.cols+b.cols-2;
		a.refresh();
		return a;
	}
	
	Labyrinth join_v(Labyrinth a, Labyrinth b, int fill = 1)
	{
		if ( a.overwrite == true )
		{
			return b;
		}
		if (b.cols > a.cols)
		{
			cout << "in if-1" << endl;
			for( int r0 = 0 ; r0 < a.rows ; ++r0 )
			{
				//vector<int> row; // Create an empty row
				for (int j = a.cols; j < b.cols ; j++)
				{
					a.maze[r0].push_back(fill); // Add an element (column) to the row
				}
				//a.maze.push_back(row); // Add the row to the main vector
			}
//			a.cols=b.cols;
		}
		else if (a.cols > b.cols )
		{
			cout << "in if-2" << endl;
			for ( int r0 = 0 ; r0 < b.rows ; ++r0 )
			{
				//vector<int> row; // Create an empty row
				for (int j = b.cols; j < a.cols ; j++)
				{
					b.maze[r0].push_back(fill); // Add an element (column) to the row
				}
				//b.maze.push_back(row); // Add the row to the main vector
			}
//			b.cols=a.cols;
		}
		a.refresh();
		b.refresh();
		for ( int c1 = 0 ; c1 < a.cols ; ++c1 )
		{
			a.maze[a.rows-2][c1] = wall_or(a.maze[a.rows-2][c1],b.maze[0][c1]);
			a.maze[a.rows-1][c1] = b.maze[1][c1];
		}
//		a.maze[a.rows-1] = b.maze[1];
		for ( int r1 = 2 ; r1 < b.rows ; ++r1 )
		{
			//for ( int c1 = 0 ; c1 < b.cols ; ++c1 )
			//{
				a.maze.push_back(b.maze[r1]);
			//}
		}
//		a.rows = a.rows + b.rows - 2;
		a.refresh();
		return a;
	}
	
	int wall_or(int sq1, int sq2)
	{
		if (( sq1 == -1) || ( sq2 == -1))
		{
			return -1;
		}
		else
		{
			return 1;
		}  
	}
	
	int num_btwn(int low=1, int high=10)
	{
		unsigned seed = chrono::system_clock::now().time_since_epoch().count();
		minstd_rand0 rand (seed);
		return (rand() % (high - low + 1)) + low;
	}
};

class Game_field: public Labyrinth
{
protected:
	vector< vector <Terrain> > terrainmap;
	Unit player;
	int team_val;
	int unit_val;
	bool atkphase;
	bool fogofwar;
	bool shroud;
public:
	Armies teams;
	friend class Battlefield;
	
	Game_field()
	{
		
	}
	
	Game_field(Labyrinth a,int numarmies, bool is_shroud=true, bool is_fog=true)
	{
		team_val=0;
		unit_val=0;
		atkphase=false;
		fogofwar=is_fog;
		shroud=is_shroud;
		rows=a.rows;
		cols=a.cols;
		Max=rows*cols;
		maze=a.maze;
		wall_pos=a.wall_pos;
		ht_rnd_g=a.ht_rnd_g;
		xcoor=a.xcoor;
		ycoor=a.ycoor;
		xcoor2=a.xcoor2;
		ycoor2=a.ycoor2;
		pos_dir=a.pos_dir;
		steps=a.steps;
		xfin=a.xfin;
		yfin=a.yfin;
		xstart=a.xstart;
		ystart=a.ystart;
		fin_at_start=a.fin_at_start;
		overwrite=a.overwrite;
		ytest=a.ytest;
		xtest=a.xtest;
		postest=a.postest;
		playervalue=a.playervalue;
		movesused=a.movesused;
		maxsteps=a.maxsteps;
		tempval=a.tempval;
		atksqrs=a.atksqrs;
		playername=a.playername;
		for (int i = 0; i < rows; i++)
		{
			vector <Terrain> row; // Create an empty row
			for (int j = 0; j < cols; j++)
			{
				row.push_back(Terrain(numarmies,maze[i][j],1,is_shroud,is_fog)); // Add an element (column) to the row
			}
			terrainmap.push_back(row); // Add the row to the main vector
		}
	}
	
	void resetspaces(int fill=1)
	{
		for ( int r = 0; r < rows ; r++ )
		{
			for ( int c = 0 ; c < cols ; c++ )
			{
				if (terrainmap[r][c].getmapvalue() >= 0)
				{
					terrainmap[r][c].setmapvalue(fill);
				}
			}
		}
	}
	
	void test_dir(int dir, int testfor, bool allow_f2 = false, bool zerosp = true, bool not_wall = true)
	{
		if (testfor == 0)
		{
			if (zerosp == true)
			{
				if (terrainmap[ycoor2][xcoor2].getmapvalue() == 0)
					pos_dir[dir] = 1;
				else
					pos_dir[dir] = 0;
			}
			else
			{
				if (terrainmap[ycoor2][xcoor2].getmapvalue() >= 0)
					pos_dir[dir] = 1;
				else
					pos_dir[dir] = 0;
			}
		}
		else if (testfor == -1)
		{
			if (not_wall==true)
			{
				if (terrainmap[ycoor2][xcoor2].getmapvalue() != -1)
					pos_dir[dir] = 1;
				else
					pos_dir[dir] = 0;
			}
			else
			{
				if (terrainmap[ycoor2][xcoor2].getmapvalue() >= -1)
					pos_dir[dir] = 1;
				else
					pos_dir[dir] = 0;
			}
		}
		else
		{
			if (terrainmap[ycoor2][xcoor2].getmapvalue() >= testfor)
				pos_dir[dir] = 1;
			else
				pos_dir[dir] = 0;
		}
	}
	
	bool is_isolated(int y, int x)
	{
		int yreplace=ycoor;
		int xreplace=xcoor;
		ycoor=y;
		xcoor=x;
		bool flag=false;
		test_dirs(0,1);
		if (pos_dir[0]==0)
			flag=true;
		ycoor=yreplace;
		xcoor=xreplace;
		return flag;
	}
	
	vector<int> rmisolatedtests(vector<int> original)
	{
		vector<int> new_vec;
		for (int c = 0 ; c < original.size() ; c++ )
		{
			pos2coors(original[c]);
			if (terrainmap[ytest][xtest].getmapvalue()==1)
				terrainmap[ytest][xtest].setmapvalue(0);
		}
		for (int c = 0 ; c < original.size() ; c++ )
		{
			pos2coors(original[c]);
			if (!is_isolated(ytest,xtest))
			{
				new_vec.push_back(original[c]);
			}
		}
		resetspaces();
		return new_vec;
	}
	
	void attackablesqrs(int y, int x, int range, bool needclear=true, bool targetwalls=false, bool needsight=true)
	{
		if (range==1)
		{
			needclear=false;
		}
		vector<int> temp;
		atksqrs = temp;
		resetspaces();
		int ymin = y-range;
		int ymax = y+range;
		int xmin = x-range;
		int xmax = x+range;
		if (ymin < 0)
		{
			ymin=0;
		}
		if (ymax > rows-1)
		{
			ymax = rows-1;
		}
		if (xmin < 0)
		{
			xmin=0;
		}
		if (xmax > cols-1)
		{
			xmax=cols-1;
		}
		for ( int y0 = ymin ; y0 <= ymax ; y0++ )
		{
			for ( int x0 = xmin ; x0 <= xmax ; x0++ )
			{
				if ((abs(y-y0)+abs(x-x0)) <= range)
				{
					if (needclear)
					{
						if (needsight)
						{
							if (terrainmap[y0][x0].getarmysight(team_val)==0)
							{
								if (clearpath(y,x,y0,x0))
								{
									if ((targetwalls) || (terrainmap[y0][x0].getmapvalue() != -1))
									{
										coors2pos(y0,x0);
// 										atksqrs.push_back(postest);
// 										if (terrainmap[y0][x0].getmapvalue()==1)
// 										{
// 											terrainmap[y0][x0].setmapvalue(0);
// 										}
										temp.push_back(postest);
									}
								}
							}
						}
						else
						{
							if (clearpath(y,x,y0,x0))
							{
								if ((targetwalls) || (terrainmap[y0][x0].getmapvalue() != -1))
								{
									coors2pos(y0,x0);
									temp.push_back(postest);
								}
							}
						}
						
					}
					else
					{
						if (needsight)
						{
							if (terrainmap[y0][x0].getarmysight(team_val)==0)
							{
								if ((targetwalls) || (terrainmap[y0][x0].getmapvalue() != -1))
								{
									coors2pos(y0,x0);
									temp.push_back(postest);
								}
							}	
						}
						else
						{
							if ((targetwalls) || (terrainmap[y0][x0].getmapvalue() != -1))
							{
								coors2pos(y0,x0);
								temp.push_back(postest);
							}
						}
					}
				}
			}
		}
		atksqrs = rmisolatedtests(temp);
		for(int c = 0 ; c < atksqrs.size() ; c++ )
		{
			pos2coors(atksqrs[c]);
			if (terrainmap[ytest][xtest].getmapvalue()==1)
			{
				terrainmap[ytest][xtest].setmapvalue(0);
			}
		}
		
	}

	bool is_adjacent(int pos0, int pos1)
	{
		pos2coors(pos0);
		int x0 = xtest;
		int y0 = ytest;
		pos2coors(pos1);
		int x1 = xtest;
		int y1 = ytest;
		int dx = abs(x1-x0);
		int dy = abs(y1-y0);
		if (dx+dy==1)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	
	vector<int> contiguoussubset(vector<int> pos_list) // 0'th element of pos_list is starting point for subset.
	{
		vector<int> connections;
		connections.push_back(1);
		int change=0;
		int pos0=pos_list[0];
		int pos1;
		for (int k = 1 ; k < pos_list.size() ; k++ )
		{
			pos1=pos_list[k];
			if (is_adjacent(pos0,pos1))
			{
				connections.push_back(1);
				change++;
			}
			else
			{
				connections.push_back(0);
			}
		}
		if (change==0)
		{
			return connections;
		}
		else
		{
			int current=0;
			while (change!=0)
			{
				current++;
				change=0;
				for (int c = 1 ; c < pos_list.size() ; c++)
				{
					int t = connections[c];
					if (t==current)
					{
						pos1=pos_list[c];
						for ( int k = 1 ; k < pos_list.size() ; k++ )
						{
							int cr=pos_list[k];
							if ((is_adjacent(pos1,cr)) && (connections[k]==0))
							{
								connections[k]=current+1;
								change++;
							}
						}
					}
				}
			}
		}
		return connections;
	}
	
	int moveablesqrs(int y, int x, int move, int flying=false)
	{
		if (flying)
		{
			attackablesqrs(y,x,move,false);
			return 0;
		}
		vector<int> possiblemoves;
		movesqrs=possiblemoves;
		coors2pos(y,x);
		possiblemoves.push_back(postest);
		resetspaces();
		int ymin = y-move;
		int ymax = y+move;
		int xmin = x-move;
		int xmax = x+move;
		if (ymin < 0)
		{
			ymin=0;
		}
		if (ymax > rows-1)
		{
			ymax = rows-1;
		}
		if (xmin < 0)
		{
			xmin=0;
		}
		if (xmax > cols-1)
		{
			xmax=cols-1;
		}
		teams.setfriends(team_val,cols);
		for ( int y0 = ymin ; y0 <= ymax ; y0++ )
		{
			for ( int x0 = xmin ; x0 <= xmax ; x0++ )
			{
				if ((abs(y-y0)+abs(x-x0)) <= move)
				{
					if (terrainmap[y0][x0].isempty())
					{
						if (terrainmap[y0][x0].getmapvalue()==1)
						{
							coors2pos(y0,x0);
							possiblemoves.push_back(postest);
						}
					}
					else
					{
						coors2pos(y0,x0);
						if (teams.pos_is_friend(postest))
						{
							possiblemoves.push_back(postest);
						}
					}
				}
			}
		}
		vector<int> stepstaken = contiguoussubset(possiblemoves);
		for (int c = 1; c < possiblemoves.size() ; c++ )
		{
			if ((stepstaken[c] > 0) && (stepstaken[c] <=move))
			{
				pos2coors(possiblemoves[c]);
				if (stepstaken[c]==move)
				{
					if (terrainmap[ytest][xtest].isempty())
					{
						movesqrs.push_back(possiblemoves[c]);
						if (terrainmap[ytest][xtest].getmapvalue()==1)
						{
							terrainmap[ytest][xtest].setmapvalue(0);
						}
					}
				}
				else
				{
					movesqrs.push_back(possiblemoves[c]);
					if (terrainmap[ytest][xtest].getmapvalue()==1)
					{
						terrainmap[ytest][xtest].setmapvalue(0);
					}
				}
			}
		}
		
	}
	
	int moveablesqrs_recursive(int y, int x, int move, bool flying=false,int recursion=0)
	{
		if (flying)
		{
			attackablesqrs(y,x,move,false);
		}
		else
		{
			if (recursion==0)
			{
				resetspaces();
				tempval=0;
			}
			if (move==0)
			{
				return tempval;
			}
			ycoor=y;
			xcoor=x;
			int numdirs = test_dirs(1,-1);
			vector<int> tempvec = pos_dir;
			if (numdirs==0)
			{
				return tempval;
			}
			else
			{
				if (tempvec[1]==1)
				{
					if (terrainmap[y-1][x].getmapvalue()==1)
					{
						terrainmap[y-1][x].setmapvalue(0);
					}
					tempval++;
					recursion++;
					moveablesqrs_recursive(y-1,x,move-1,false,recursion);
				}
				if (tempvec[2]==1)
				{
					if (terrainmap[y][x+1].getmapvalue()==1)
					{
						terrainmap[y][x+1].setmapvalue(0);
					}
					tempval++;
					recursion++;
					moveablesqrs_recursive(y,x+1,move-1,false,recursion);
				}
				if (tempvec[3]==1)
				{
					if (terrainmap[y+1][x].getmapvalue()==1)
					{
						terrainmap[y+1][x].setmapvalue(0);
					}
					tempval++;
					recursion++;
					moveablesqrs_recursive(y+1,x,move-1,false,recursion);
				}
				if (tempvec[4]==1)
				{
					if (terrainmap[y][x-1].getmapvalue()==1)
					{
						terrainmap[y][x-1].setmapvalue(0);
					}
					tempval++;
					recursion++;
					moveablesqrs_recursive(y,x-1,move-1,false,recursion);
				}
			}
		}
		return tempval;
	}
	
	bool clearpath(int y1, int x1, int y2, int x2, int round = 3) // round=0 -> actually round, round=1 -> round down, round=2 -> round up, round=3 -> do both
	{
		double f_x=0.0;
		int r_y=0;
		vector<int> pos2check;
		if (x1==x2)
		{
			if (y1>y2)
			{
				int temp = y1;
				y1 = y2;
				y2 = temp;
			}
			for ( int y0 = y1 ; y0 < y2 ; y0++ )
			{
				coors2pos(y0,x1);
				pos2check.push_back(postest);
			}
		}
		else if (y1==y2)
		{
			if (x1>x2)
			{
				int temp = x1;
				x1 = x2;
				x2 = temp;
			}
			for ( int x0 = x1 ; x0 < x2 ; x0++ )
			{
				coors2pos(y1,x0);
				pos2check.push_back(postest);
			}
		}
		else
		{
			if (x1>x2) //switch so x1 is smaller
			{
				int temp = x1;
				x1 = x2;
				x2 = temp;
				temp = y1;
				y1 = y2;
				y2 = temp;
			}
			int rise = y2 - y1;
			int run = x2 - x1;
			for ( int x0 = x1 ; x0 < x2 ; x0++ )
			{
				f_x=point_slope(y1,x1,rise,run,x0);
				if (round==3)
				{
					r_y = ceil(f_x);				//up
					coors2pos(r_y,x0);
					pos2check.push_back(postest);
					r_y=floor(f_x);					//down
					coors2pos(r_y,x0);
					pos2check.push_back(postest);
				}
				else if (round==2)
				{
					r_y = ceil(f_x);				//up
					coors2pos(r_y,x0);
					pos2check.push_back(postest);
				}
				else if (round==1)
				{
					r_y=floor(f_x);					//down
					coors2pos(r_y,x0);
					pos2check.push_back(postest);
				}
				else
				{
					r_y=floor(f_x+0.5);
					coors2pos(r_y,x0);
					pos2check.push_back(postest);
				}
			}
			if (y1>y2)
			{
				int temp = x1;
				x1 = x2;
				x2 = temp;
				temp = y1;
				y1 = y2;
				y2 = temp;
			}
			rise = y2 - y1;
			run = x2 - x1;
			for ( int y0 = y1 ; y0 < y2 ; y0++ )
			{
				f_x=point_slope(x1,y1,run,rise,y0);
				if (round==3)
				{
					r_y = ceil(f_x);				//up
					coors2pos(y0,r_y);
					pos2check.push_back(postest);
					r_y=floor(f_x);					//down
					coors2pos(y0,r_y);
					pos2check.push_back(postest);
				}
				else if (round==2)
				{
					r_y = ceil(f_x);				//up
					coors2pos(y0,r_y);
					pos2check.push_back(postest);
				}
				else if (round==1)
				{
					r_y=floor(f_x);					//down
					coors2pos(y0,r_y);
					pos2check.push_back(postest);
				}
				else
				{
					r_y=floor(f_x+0.5);
					coors2pos(y0,r_y);
					pos2check.push_back(postest);
				}
			}
		}
		for ( int c = 1 ; c < pos2check.size() ; c++ )
		{
			pos2coors(pos2check[c]);
			if (terrainmap[ytest][xtest].getmapvalue() == -1)
			{
				return false;
			}
		}
		return true;
	}

	void updatesight() // updates the sight of the current_team	// need to make it so dead units don't contribute	//did that, but it broke other things, i think.
	{
		Unit tempplayer;
		int tempcurrentunit=unit_val;
		int tempcurrentteam=team_val;
		resetspaces();
		for ( int c = 0 ; c < teams[team_val].size() ; c++ )
		{
			unit_val=c;
			tempplayer=teams.getarmyunit(team_val,unit_val);
			if (!tempplayer.isdead())
			{
				attackablesqrs(tempplayer.getycoor(), tempplayer.getxcoor(), tempplayer.getsight(), true, true, false);
				for (int j = 0 ; j < atksqrs.size() ; j++ )
				{
					pos2coors(atksqrs[j]);
					terrainmap[ytest][xtest].setarmysight(team_val, -1);
				}
				if (terrainmap[tempplayer.getycoor()][tempplayer.getxcoor()].isempty())
				{
					terrainmap[tempplayer.getycoor()][tempplayer.getxcoor()].setempty(false);
				}
				terrainmap[tempplayer.getycoor()][tempplayer.getxcoor()].setarmysight(team_val,-1);
				resetspaces();
			}
		}
		for ( int i = 0 ; i < rows ; i++ )
		{
			for ( int n = 0 ; n < cols ; n++ )
			{
				if (terrainmap[i][n].getarmysight(team_val) == 0)
				{
					if (fogofwar)
						terrainmap[i][n].setarmysight(team_val,1);
				}
				else if (terrainmap[i][n].getarmysight(team_val) == -1)
				{
					terrainmap[i][n].setarmysight(team_val,0);
				}
			}
		}
		team_val=tempcurrentteam;
		unit_val=tempcurrentunit;
	}
	
	int echo_sqr(int y, int x)		// use terrainmap not maze	(done)
	{
		int ytest,xtest;
		Unit a;
		string output=" ";
		bool under=false;
		if (terrainmap[y][x].getarmysight(team_val) == 2)
			cout << "\033[7;30m?\033[0m";
		else if (terrainmap[y][x].getarmysight(team_val) == 1)
		{
			output = "\033[7;30m" + terrainmap[y][x].show() + "\033[0m";
			cout << output;
		}
		else
		{
			if (terrainmap[y][x].isempty())
				cout << terrainmap[y][x].tempshow(atkphase);
			else
			{
				if ((y==ycoor) && (x==xcoor))
					under=true;
				for ( int k = 0 ; k < teams.size() ; k++)
				{
					for ( int c = 0 ; c < teams[k].size() ; c++ )
					{
						a=teams[k].getunit(c);
						ytest=a.getycoor();
						xtest=a.getxcoor();
						stringstream ss, ss1;
						string temp, temp1;
						ss << a.getrepr();
						ss >> temp;
						ss1 << teams[k].getcolorcode(); 
						ss1 >> temp1;
						if ((y==ytest) && (x==xtest) && (a.isalive()))
						{
							if (under)
							{
								output = "\033[7;" + temp1 + "m" + temp + "\033[0m";
							}
							else
							{
								output = "\033[0;" + temp1 + "m" + temp + "\033[0m";
							}
							cout << output;
							return 0;
						}
					}
				}
				cout << " ";
			}
		}
		return 0;
	}
	
	void dispm(bool flag=false,bool show_zr = true, bool flag2=false)
	{
		int rows9=rows;
		if ((flag) || (flag2))
			rows9=rows - 1;
		for (int i = 0; i < rows9; ++i)
		{
			for (int j = 0; j < cols; ++j)
			{
				echo_sqr(i,j);
			}
			cout << endl;
		}
		if (flag2)
		{
			cout << "Your " << playername << " is at: <" << xcoor << "," << ycoor << "> and has: " << maxsteps-movesused << "/" << maxsteps << " steps remaining." << endl;
		}
	}
	
	void noendmovestack()
	{
		if (pos_dir[1]==1)
		{
			if (!(terrainmap[ycoor-1][xcoor].isempty()))
			{
				pos_dir[1]=0;
			}
		}
		if (pos_dir[2]==1)
		{
			if (!(terrainmap[ycoor][xcoor+1].isempty()))
			{
				pos_dir[2]=0;
			}
		}
		if (pos_dir[3]==1)
		{
			if (!(terrainmap[ycoor+1][xcoor].isempty()))
			{
				pos_dir[3]=0;
			}
		}
		if (pos_dir[4]==1)
		{
			if (!(terrainmap[ycoor][xcoor-1].isempty()))
			{
				pos_dir[4]=0;
			}
		}
	}

	void nostackenemies()
	{
		teams.setfriends(team_val, cols);
		if (pos_dir[1]==1)
		{
			if (!(terrainmap[ycoor-1][xcoor].isempty()))
			{
				coors2pos(ycoor-1,xcoor);
				if (!teams.pos_is_friend(postest))
				{
					pos_dir[1]=0;
				}
			}
		}
		if (pos_dir[2]==1)
		{
			if (!(terrainmap[ycoor][xcoor+1].isempty()))
			{
				coors2pos(ycoor,xcoor+1);
				if (!teams.pos_is_friend(postest))
				{
					pos_dir[2]=0;
				}
			}
		}
		if (pos_dir[3]==1)
		{
			if (!(terrainmap[ycoor+1][xcoor].isempty()))
			{
				coors2pos(ycoor+1,xcoor);
				if (!teams.pos_is_friend(postest))
				{
					pos_dir[3]=0;
				}
			}
		}
		if (pos_dir[4]==1)
		{
			if (!(terrainmap[ycoor][xcoor-1].isempty()))
			{
				coors2pos(ycoor,xcoor-1);
				if (!teams.pos_is_friend(postest))
				{
					pos_dir[4]=0;
				}
			}
		}
	}
	
	void play(bool quiet=false, bool start_stair=false, bool show_d=false, int max_moves=-1, bool flag3=false)	// change to use terrainmap	(done)
	{
		maxsteps=max_moves;
		bool stacked=false;
		bool out_flag=false;
		movesused=0;
		xcoor=xstart;
		ycoor=ystart;
		player=teams.find_unit(ycoor,xcoor,true);
		team_val=teams.getcurrentarmyval();
		unit_val=teams.getcurrentunitval();
		updatesight();
		moveablesqrs(ycoor,xcoor,max_moves);
		dispm(true,show_d,flag3);
		char direction=game_char();
		bool unf=true;
		int move_counter=0;
		while (unf)
		{
			test_dirs(0,1,true,false);
			nostackenemies();
			if (max_moves-move_counter==1)
			{
				noendmovestack();
			}
			if ((direction=='w') && (pos_dir[1]==1))
			{
				terrainmap[ycoor][xcoor].setempty(true);
				ycoor=ycoor-1;
				if (terrainmap[ycoor][xcoor].isempty())
				{
					stacked=false;
				}
				else
				{
					stacked=true;
				}
				terrainmap[ycoor][xcoor].setempty(false);
				player.setycoor(ycoor);
				player.setxcoor(xcoor);
				teams.setcurrentunit(player);
				move_counter++;
			}
			if ((direction=='d') && (pos_dir[2]==1))
			{
				terrainmap[ycoor][xcoor].setempty(true);
				xcoor=xcoor+1;
				if (terrainmap[ycoor][xcoor].isempty())
				{
					stacked=false;
				}
				else
				{
					stacked=true;
				}
				terrainmap[ycoor][xcoor].setempty(false);
				player.setycoor(ycoor);
				player.setxcoor(xcoor);
				teams.setcurrentunit(player);
				move_counter++;
			}
			if ((direction=='s') && (pos_dir[3]==1))
			{
				terrainmap[ycoor][xcoor].setempty(true);
				ycoor=ycoor+1;
				if (terrainmap[ycoor][xcoor].isempty())
				{
					stacked=false;
				}
				else
				{
					stacked=true;
				}
				terrainmap[ycoor][xcoor].setempty(false);
				player.setycoor(ycoor);
				player.setxcoor(xcoor);
				teams.setcurrentunit(player);
				move_counter++;
			}
			if ((direction=='a') && (pos_dir[4]==1))
			{
				terrainmap[ycoor][xcoor].setempty(true);
				xcoor=xcoor-1;
				if (terrainmap[ycoor][xcoor].isempty())
				{
					stacked=false;
				}
				else
				{
					stacked=true;
				}
				terrainmap[ycoor][xcoor].setempty(false);
				player.setycoor(ycoor);
				player.setxcoor(xcoor);
				teams.setcurrentunit(player);
				move_counter++;
			}
			if (direction=='q')
			{
				if (stacked)
				{
					out_flag=true;
				}
				else
				{
					unf=false;
					out_flag=false;
				}
			}
			if (max_moves>0)
			{
				if (move_counter==max_moves)
				{
					unf=false;
				}
			}
			clear();
			movesused=move_counter;
			updatesight();
			moveablesqrs(ycoor,xcoor,max_moves-move_counter);
			dispm(true,show_d,flag3);
			if (out_flag)
			{
				cout << "You may not end your movement with Units stacked." << endl;
				out_flag=false;
			}
			if (unf)
			{
				direction=game_char();
			}
		}
		movesused = move_counter;
		updatesight();
	}
	
	void set_start(int y,int x,bool erase=true)
	{
		if ((x >= 0) && (x < cols) && (y >= 0) && (y < rows))
		{
			xstart=x;
			ystart=y;
			xcoor=x;
			ycoor=y;
			//maze[ycoor][xcoor]=playervalue;		// dont think i need to do this
		}
	}
	
	void setatkphase(bool flag)
	{
		atkphase=flag;
	}
	
	bool getatkphase()
	{
		return atkphase;
	}
	
};

class Battlefield
{
protected:
	Labyrinth originalmap;
	Game_field map;
	Army temparmy;
	int numarmies;
	Unit warrior, defender;
	int turn;
	vector<int> sqrs2atk;
	vector<int> friendlies;
	vector<int> unitsleftperteam;
	int wincondition;
	int atker_army, defer_army, atker_unit, defer_unit;
	vector<string> colornames;
	vector<int> colorcodes;
public:
	Battlefield(int ysize=80, int xsize=22, int num_armies=2, int funds=2000,bool shroud=true, bool fog=true)
	{
		numarmies=num_armies;
		turn=0;
		atker_army=0;
		defer_army=0;
		atker_unit=0;
		defer_unit=0;
		wincondition=0;
		Dungeon_map field(ysize,xsize);
		originalmap=field.get_map();
		map=Game_field(originalmap,num_armies,shroud,fog);
		colornames.push_back("Blue");
		colorcodes.push_back(CODE_BLUE);
		colornames.push_back("Red");
		colorcodes.push_back(CODE_RED);
		colornames.push_back("Green");
		colorcodes.push_back(CODE_GREEN);
		colornames.push_back("Yellow");
		colorcodes.push_back(CODE_YELLOW);
		for ( int j = 0 ; j < numarmies ; j++ )
		{
			string leader=colornames[j] + " Team Leader";
			temparmy = Army(funds);
			temparmy = buildarmy(temparmy,leader);
			temparmy.setcolorcode(colorcodes[j]);
			unitsleftperteam.push_back(temparmy.size());
			map.teams.add_army(temparmy);
		}
		placeunits();
		resetall();
		for ( int k = 0 ; k < map.teams.size() ; k++ )
		{
			map.team_val=k;
			map.updatesight();
		}
	}
	
	string getleadername(int k)
	{
		return colornames[k] + " Team Leader";
	}
	
	char getch()
	{
		return singlechar();
	}
	
	void char2unit(char represent)
	{
		if ((represent == 's') || (represent == 'S'))
		{
			Scout temp;
			warrior = temp.resetvalues(warrior);
		}
		else if ((represent == 'r') || (represent == 'R'))
		{
			Swordsman temp;
			warrior = temp.resetvalues(warrior);
		}
		else if ((represent == 'h') || (represent == 'H'))
		{
			Halberdier temp;
			warrior = temp.resetvalues(warrior);
		}
		else if ((represent == 'x') || (represent == 'X'))
		{
			Axeman temp;
			warrior = temp.resetvalues(warrior);
		}
		else if ((represent == 'a') || (represent == 'A'))
		{
			Archer temp;
			warrior = temp.resetvalues(warrior);
		}
		else if ((represent == 'm') || (represent == 'M'))
		{
			Mage temp;
			warrior = temp.resetvalues(warrior);
		}
		else if ((represent == 'd') || (represent == 'D'))
		{
			Swordmage temp;
			warrior = temp.resetvalues(warrior);
		}
		else if ((represent == 'k') || (represent == 'K'))
		{
			Knight temp;
			warrior = temp.resetvalues(warrior);
		}
		else if ((represent == 'w') || (represent == 'W'))
		{
			Wizard temp;
			warrior = temp.resetvalues(warrior);
		}
		else if ((represent == 'e') || (represent == 'E'))
		{
			Elf_Archer temp;
			warrior = temp.resetvalues(warrior);
		}
		else if ((represent == 'b') || (represent == 'B'))
		{
			Berserker temp;
			warrior = temp.resetvalues(warrior);
		}
		else
		{
			Calvalry temp;
			warrior = temp.resetvalues(warrior);
		}
	}
	
	void cls()
	{
		clearscreen();
	}

	Army buildarmy(Army team, string prompt="General")
	{
		char choice;
		bool buying=true;
		int count = 0;
		string roster;
		while (buying)
		{
			roster = team.rollcall();
			cout << prompt << ", you have: " << roster << endl;
			cout << "and " << team.getmoney() << " dollars remaining." << endl;
			cout << "Your options for units are:" << endl;
			cout << "[S]cout:50, swo[R]dsman:100, [H]alberdier:100, a[X]eman:100," << endl;
			cout << "[A]rcher:150, [M]age:150, swor[D]mage:500, [K]night:500," << endl;
			cout << "[W]izard:500, [E]lf archer:500, [B]erserker:500, [C]alvalry:500" << endl;
			cout << "What would you like to buy next > " << endl;
			choice = getch();
			cout << choice << endl;
			char2unit(choice);
			warrior.resetsight();
			team.buyunit(warrior);
			if ((team.getmoney() < 50) || (team.size() == team.maxsize()))
			{
				buying=false;
			}
			cls();
		}
		return team;
	}
	
	int num_btwn(int low=1, int high=10)
	{
		unsigned seed = chrono::system_clock::now().time_since_epoch().count();
		minstd_rand0 rand (seed);
		return (rand() % (high - low + 1)) + low;
	}
	
	int randcoor(int type,int sector) //TYPE: 0=y, 1=x; Sectors: 1=TL, 2=BR, 3=TR, 4=BL, 5=TC, 6=BC, 7=ML, 8=MR, 9=MC
	{
		if (sector==1)						//	TL | TC | TR
		{							//	ML | MC | MR
			if (type == 0)					//	BL | BC | BR
				return num_btwn(0,map.rows/4);					//T
			else
				return num_btwn(0,map.cols/4);					//L
		}
		else if (sector==2)
		{
			if (type==0)
				return num_btwn(map.rows-1 - map.rows/4,map.rows-1);		//B
			else
				return num_btwn(map.cols-1 - map.cols/4,map.cols-1);		//R
		}
		else if (sector==3)
		{
			if (type==0)
				return num_btwn(0,map.rows/4);					//T
			else
				return num_btwn(map.cols-1 - map.cols/4,map.cols-1);		//R
		}
		else if (sector==4)
		{
			if (type==0)
				return num_btwn(map.rows-1 - map.rows/4,map.rows-1);		//B
			else
				return num_btwn(0,map.cols/4);					//L
		}
		else if (sector==5)
		{
			if (type==0)
				return num_btwn(0,map.rows/4);					//T
			else
				return num_btwn(3*map.cols/8,5*map.cols/8);			//C
		}
		else if (sector==6)
		{
			if (type==0)
				return num_btwn(map.rows-1 - map.rows/4,map.rows-1);		//B
			else
				return num_btwn(3*map.cols/8,5*map.cols/8);			//C
		}
		else if (sector==7)
		{
			if (type==0)
				return num_btwn(3*map.rows/8,5*map.cols/8);			//M
			else
				return num_btwn(0,map.cols/4);					//L
		}
		else if (sector==8)
		{
			if (type==0)
				return num_btwn(3*map.rows/8,5*map.cols/8);			//M
			else
				return num_btwn(map.cols-1 - map.cols/4,map.cols-1);		//R
		}
		else
		{
			if (type==0)
				return num_btwn(3*map.rows/8,5*map.cols/8);			//M
			else
				return num_btwn(3*map.cols/8,5*map.cols/8);			//C
		}
	}
	
	void placeunits()		// rewrite to use terrainmap  (done)
	{
		Unit temp;
		int x=0,y=0;
		bool notplaced=true;
		for ( int k = 0 ; k < map.teams.size() ; k++ )
		{
			for ( int c = 0 ; c < map.teams[k].size() ; c++ )
			{
				notplaced=true;
				temp = map.teams[k].getunit(c);
				while (notplaced)
				{
					y=randcoor(0,k+1);
					x=randcoor(1,k+1);
					if ((map.terrainmap[y][x].getmapvalue()>0) && (map.terrainmap[y][x].isempty()))
					{
						map.terrainmap[y][x].setempty(false);
						temp.setxcoor(x);
						temp.setycoor(y);
						notplaced=false;
					}
				}
				map.teams.setarmyunit(temp,k,c);
			}
		}
	}

	void resetall()
	{
		for ( int k = 0 ; k < map.teams.size() ; k++ )
		{
			cout << map.teams[k].isplaying();
			if (map.teams[k].isplaying())
			{
				cout << " " << k << ": ";
				for ( int c = 0 ; c < map.teams[k].size() ; c++ )
				{
					cout << c <<"-";
					warrior = map.teams[k].getunit(c);
					warrior.resetattack();
					warrior.resetmove();
					cout << warrior.getmovesleft() << ";";
					map.teams.setarmyunit(warrior,k,c);
				}
				cout << endl;
			}
		}
		cout << endl;
	}
	
	int selectunitonscreen(bool showhp = false, int selection=0)	//	selection: 0=none ; 1=units that can attack ; 2=units that haven't moved ; 3=second movephasae
	{
		int player = turnmodnumarmies();
		vector<int> unitsonscreen;
		int enemies=0;
		int numunits=0;
		for (int j = 0 ; j < map.rows ; j++ )
		{
			for (int k = 0 ; k < map.cols ; k++)
			{
				if (!(map.terrainmap[j][k].isempty()))
				{
//					cout << "hello?" << endl;
					if (map.terrainmap[j][k].getarmysight(player)==0)
					{
//						cout << "alrighty..." << endl;
						map.coors2pos(j,k);
						finddefender(map.postest,true,true);
						if (defer_army!=player)
						{
//							cout << "enemy unit?" << endl;
							unitsonscreen.push_back(map.postest);
							enemies++;
						}
						else
						{
//							cout << "player's unit" << endl;
							if (selection==1)
							{
								if (defender.canstillatk())
								{
									unitsonscreen.push_back(map.postest);
									numunits++;
								}
							}
							else if (selection==2)
							{
								cout << defender.getmovesleft() << ";" << defender.getstat(MOVE_S) << endl;
								if (defender.getmovesleft()==defender.getstat(MOVE_S))
								{
									unitsonscreen.push_back(map.postest);
									numunits++;
								}
							}
							else if (selection==3)
							{
								if (defender.getmovesleft() > 0)
								{
									unitsonscreen.push_back(map.postest);
									numunits++;
								}
							}
							else
							{
								unitsonscreen.push_back(map.postest);
								numunits++;
							}
						}
					}
				}
			}
		}
//		cout << "done with for loop" << endl;
//		cout << unitsonscreen.size() << endl;
		int index=0;
		char direction = 'e';
		bool unf=true;
		if (numunits==0)
			return 0;
		if (unitsonscreen.size()==0)
			return 0;
		else if (unitsonscreen.size()==1)
		{
			finddefender(unitsonscreen[0],true,true);
			if (defer_army==player)
			{
				map.setname(defender.getname());
				map.set_start(defender.getycoor(),defender.getxcoor(),false);
				cls();
				return 1;
			}
			else
			{
				return 0;
			}
		}
		if ((selection==1) && (enemies==0))
			return 0;
		int return_val=1;
		while (unf)
		{
			finddefender(unitsonscreen[index],true,true);
			map.setname(defender.getname());
			map.set_start(defender.getycoor(),defender.getxcoor(),false);
			cls();
			map.dispm();
			if (selection==1)
				cout << "Attack Phase" << endl;
			else if (selection==2)
				cout << "First Move Phase	(press 'q' to quit this phase)" << endl;
			else if (selection==3)
				cout << "Second Move Phase	(press 'q' to quit this phase)" << endl;
			cout << "Press the left and right arrows to cycle through units. Press 's' to select unit." << endl;
			if (showhp)
			{
				cout << "This " << defender.getname() << " has " << defender.currenthp() << "/" << defender.getstat(HP_S) << " HP remaining: ";
				cout << "[";
				for (int c = 0 ; c < defender.getstat(HP_S) ; c++ )
				{
					if (c < defender.currenthp())
						cout << "=";
					else
						cout << " ";
				}
				cout << "]" << endl;
			}
			direction = map.game_char();
			if (direction=='a')
				index--;
			else if (direction=='d')
				index++;
			if (index==-1)
				index=unitsonscreen.size()-1;
			else if (index==unitsonscreen.size())
				index=0;
			if ((direction=='s') && (defer_army==player))
				unf=false;
			else if (direction=='q')
			{
				unf=false;
				return_val=0;
			}
		}
		finddefender(unitsonscreen[index],true,true);
		map.setname(defender.getname());
		map.set_start(defender.getycoor(),defender.getxcoor(),false);
		return return_val;
	}
	
	int turnmodnumarmies()
	{
		return turn % map.teams.size();
	}
	
	void setfriendpos()
	{
		vector<int> temp;
		friendlies=temp;
		int k = turnmodnumarmies();
		for ( int c=0 ; c < map.teams[k].size() ; c++)
		{
			warrior=map.teams[k].getunit(c);
			map.coors2pos(warrior.getycoor(),warrior.getxcoor());
			friendlies.push_back(map.postest);
		}
	}
	
	bool isfriend(int pos) // doesn't remake friendlies first
	{
		for ( int c = 0 ; c < friendlies.size() ; c++)
		{
			if (friendlies[c]==pos)
			{
				return true;
			}
		}
		return false;
	}
	
	int finddefender(int pos, bool setvals = true, bool same_team=false) //sets defer_army and defer_unit to be the unit on position:pos
	{					// ( (same_team) || ( (!same_team) && (k!=k1) ) )		if same_team==true:	allows selected unit to be on the same team.
		int k1 = turnmodnumarmies();								//	if same_team==false:	doesn't allow selected unit to be on the same team.
		for (int k = 0 ; k < map.teams.size() ; k++)
		{
			if ( ( (same_team) || ( (!same_team) && (k!=k1) ) ) && (map.teams[k].isplaying()) )
			{
				for (int c = 0 ; c < map.teams[k].size() ; c++)
				{
					defender=map.teams[k].getunit(c);
					map.coors2pos(defender.getycoor(),defender.getxcoor());
					if ((pos==map.postest) && (defender.isalive()))
					{
						defer_army=k;
						defer_unit=c;
						return 0;
					}
				}	
			}
		}
		return -1;
	}
	
	int atkwithunit(int k, int c)		// rewrite to use terrainmap	(done) (maybe)
	{
		vector<int> temp;
		sqrs2atk = temp;
		setfriendpos();
		atker_army=k;
		atker_unit=c;
		int numatks=0;
		int which=0;
		char attack=' ';
		string prompt="";
		int defendsqr=0;
		warrior=map.teams[k].getunit(c);
		prompt=getleadername(k);
		if (!warrior.canstillatk())
		{
			cout << "This " << warrior.getname() << " can no longer attack this turn." << endl;
			return 0;
		}
		if (warrior.isdead())
		{
			return 0;
		}
		map.setname(warrior.getname());
		map.set_start(warrior.getycoor(),warrior.getxcoor(),false);
		map.attackablesqrs(warrior.getycoor(),warrior.getxcoor(),warrior.getstat(RANGE_S),warrior.needlineofsight());
		for ( int c1 = 0 ; c1 < map.atksqrs.size() ; c1++ )
		{
			map.pos2coors(map.atksqrs[c1]);
			if (!(map.terrainmap[map.ytest][map.xtest].isempty()))
			{
				if (!isfriend(map.atksqrs[c1]))
				{
					sqrs2atk.push_back(map.atksqrs[c1]);
				}
			}
		}
		numatks=sqrs2atk.size();
		if (numatks>1)
		{
			map.dispm();
			cout << prompt << ", this " << warrior.getname() << " has " << numatks << " possible attacks to make." << endl;
			cout << "Please enter an integer corresping to which attack you want to make. [ 0 <= input <= " << numatks << " ]" << endl;
			cout << "0 corresponds to not attacking this turn." << endl;
			which=-1;
			while (which==-1)
			{
				cin >> which;
				if (which==0)
				{
					map.resetspaces();
					return 0;
				}
				which=which-1;
				if (which>=numatks)
				{
					which=-1;
				}
				if (which<0)
				{
					which=-1;
				}				
			}
			defendsqr=sqrs2atk[which];

		}
		else if (numatks==1)
		{
			map.dispm();
			attack='h';
			cout << prompt << ", this " << warrior.getname() << " has 1 possible attack to make." << endl;
			cout << "Do you want to attack? [Y/n]" << endl;
			while (attack=='h')
			{
				attack=getch();
				if ((attack=='n')||(attack=='N'))
				{
					map.resetspaces();
					return 0;	
				}
				else if ((attack=='y')||(attack=='Y'))
				{
					defendsqr=sqrs2atk[0];
				}
				else
				{
					attack='h';
				}
			}
		}
		else
		{
			cout << prompt << ", your " << warrior.getname() << " has no viable attacks." << endl;
			warrior.unsetattack();
			map.teams.setarmyunit(warrior,atker_army,atker_unit);
			map.resetspaces();
			return 0;
		}
		int armyval=finddefender(defendsqr);
		bool dead = doattack();
		warrior.setmovesleft(0);
		warrior.unsetattack();
		map.teams.setarmyunit(warrior,atker_army,atker_unit);
		map.teams.setarmyunit(defender,defer_army,defer_unit);
		if (defender.isdead())
		{
			map.terrainmap[defender.getycoor()][defender.getxcoor()].setempty(true);
			map.teams.deadunitcleanup();
			unitsleftperteam[defer_army]--;
		}
		map.resetspaces();
		return 1;
	}
	
	void attackphase()
	{
		int k = turnmodnumarmies();
		map.setatkphase(true);
		int c = 0;
		int test=1;
		while (c < map.teams[k].size())
		{
			test=selectunitonscreen(true,1);
			cls();
			if (test==1)
			{
				atkwithunit(defer_army,defer_unit);
				c++;
			}
			else
			{
				c = map.teams[k].size();
			}
		}
		map.setatkphase(false);
	}
	
	void playthegame()
	{
		while (wincondition==0)
		{
			doallturns();
		}
		string leader = getleadername(wincondition-1);
		string output = "Congratulations, " + leader + ", you have vanquished the opposing Forces to win the game!";
		cout << output << endl;
	}
	
	void endphase()
	{
		int numstillin = 0;
		int winner=0;
		for ( int k = 0 ; k < map.teams.size() ; k++)
		{
			if (map.teams[k].isplaying())
			{
				numstillin++;
				winner=k+1;
			}
		}
		if (numstillin==1)
		{
			wincondition=winner;
		}
	}
	
	void doallturns()
	{
		string tempstring="";
		string leader;
		resetall();
		for ( int k = 0 ; k < map.teams.size() ; k++)
		{
			if (map.teams[k].isplaying())
			{
				cls();
				leader=getleadername(k);
				cout <<  leader << ", it is your turn." << endl;
				cout << "Press enter to continue." << endl;
				cin >> tempstring;
				cls();
				movephase();
				cout << "done with move phase" << endl;
				cls();
				attackphase();
				endphase();
				if (wincondition==0)
				{
					cls();
					movephase(false);
				}
			}
			turn++;
		}
	}
	
	bool areadj(Unit a,Unit b)
	{
		int x1=a.getxcoor();
		int x2=b.getxcoor();
		int y1=a.getycoor();
		int y2=b.getycoor();
		int dx = map.abs(x2-x1);
		int dy = map.abs(y2-y1);
		if ((dx+dy)==1)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	
	bool doattack()
	{
		int atknum=0;
		int defnum=0;
		if (areadj(warrior,defender))
		{
			atknum=warrior.rollatkdice(warrior.getstat(ATK_S)+warrior.getadjatkbonus());
			defnum=defender.rolldefdice(defender.getstat(DEF_S)+defender.getadjdefbonus());
		}
		else
		{
			atknum=warrior.myatkdice();
			defnum=defender.mydefdice();
		}
		int damage = atknum - defnum;
		cout << "Your attacking " << warrior.getname() << " has rolled " << atknum << " hits." << endl;
		if (defnum==0)
		{
			cout << "And the defending " << defender.getname() << " didn't even block!" << endl;
		}
		else
		{
			cout << "However, the defending " << defender.getname() << " blocked " << defnum << " times." << endl;
		}
		if (damage < 0)
		{
			damage=0;
		}
		cout << "Meaning that this attack resulted in " << damage << " damage to the opponent's " << defender.getname() << "." << endl;
		warrior.unsetattack();
		if (damage > 0)
		{
			defender.dodamage(damage);
			if (defender.isdead())
			{
				cout << "You killed the defending " << defender.getname() << "." << endl;
				cout << "Press any key to continue." << endl;
				getch();
				cls();
				return true;
			}
			else
			{
				cout << "The defending " << defender.getname() << " survived with " << defender.currenthp() << " health remaining." << endl;
				cout << "Press any key to continue." << endl;
				getch();
				cls();
			}
		}
		else
		{
			cout << "The defending " << defender.getname() << " still has " << defender.currenthp() << " health remaining." << endl;
			cout << "Press any key to continue." << endl;
			getch();
			cls();
		}
		return false;
	}
	
	void movephase(bool first=true)
	{
		int k = turnmodnumarmies();
		int c = 0;
		int trial = 1;
		int select1=2;
		if (!(first))
		{
			select1=3;
		}
		map.team_val=k;
		while (c < map.teams[k].size())
		{
			cout << "selecting unit" << endl;
			cout << select1 << endl;
			trial=selectunitonscreen(true,select1);
			cls();
			warrior=defender;
			if ( ((!warrior.isdead()) && (warrior.getmovesleft() > 0)) && (trial==1) )
			{
				map.setname(warrior.getname());
				map.set_start(warrior.getycoor(),warrior.getxcoor(),false);
				map.setmaxsteps(warrior.getstat(MOVE_S));
				map.play(true, false, false, warrior.getmovesleft(),true);
				cls();
				warrior.setmovesleft(warrior.getmovesleft() - map.movesused);
				warrior.setxcoor(map.xcoor);
				warrior.setycoor(map.ycoor);
				map.teams.setarmyunit(warrior,defer_army,defer_unit);
				c++;
			}
			if (trial!=1)
			{
				c=map.teams[k].size();
			}
		}
// 		for ( int c = 0 ; c < map.teams[k].size() ; c++)
// 		{
// 			warrior=map.teams[k].getunit(c);
// 			if ((!warrior.isdead()) && (warrior.getmovesleft() > 0))
// 			{
// 				map.setname(warrior.getname());
// 				map.set_start(warrior.getycoor(),warrior.getxcoor(),false);
// 				map.setmaxsteps(warrior.getstat(MOVE_S));
// 				map.play(true, false, false, warrior.getmovesleft(),true);
// 				cls();
// 				warrior.setmovesleft(warrior.getmovesleft() - map.movesused);
// 				warrior.setxcoor(map.xcoor);
// 				warrior.setycoor(map.ycoor);
// 				map.teams.setarmyunit(warrior,k,c);
// 			}
// 		}
	}
};
