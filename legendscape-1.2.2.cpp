//##USAGE## maze_game3_cpp [(m/l/r)/(rows)/(rows)] [()/(cols)/(cols)] [()/()/(x_start)] [()/()/(y_start)] ; sms=N ; $#=0-4
#include <iostream>
#include <string>
#include <sstream>
#include <sys/ioctl.h>
#include <cstdio>
#include <unistd.h>

//#include "unit_classes.h"
#include "maze_classes-1.2.2.h"
using namespace std;

//////////////////////////////////////////////////////////
int main( int argc, char *argv[] )
{
	int maxcols = 80;
	int maxlines = 24;
#ifdef TIOCGSIZE
	struct ttysize ts;
	ioctl(STDIN_FILENO, TIOCGSIZE, &ts);
	maxcols = ts.ts_cols;
	maxlines = ts.ts_lines;
#elif defined(TIOCGWINSZ)
	struct winsize ts;
	ioctl(STDIN_FILENO, TIOCGWINSZ, &ts);
	maxcols = ts.ws_col;
	maxlines = ts.ws_row;
#endif /* TIOCGSIZE */
	stringstream ss, ss1, ss2, ss3, ss4;
	string check_sp="false";
	int row1=22;
	int col1=80;
	int x=0;
	int y=0;
	bool sp=false;
	char m_size;
	if (argc == 3)
	{
		ss << argv[1];
		ss >> row1;
		ss2 << argv[2];
		ss2 >> col1;
//		cout << "check" << endl;
		if ((row1 % 2) == 1)
			++row1;
		if ((col1 % 2) == 1)
			++col1;
		if ((row1 < 6) || (row1 > maxlines))
			row1=22;
		if ((col1 < 6) || (col1 > maxcols))
			col1=80;
	}
	else if (argc == 4)
	{
		ss << argv[1];
		ss >> row1;
		ss2 << argv[2];
		ss2 >> col1;
		ss3 << argv[3];
		ss3 >> check_sp;
//		cout << "check" << endl;
		if ((row1 % 2) == 1)
			++row1;
		if ((col1 % 2) == 1)
			++col1;
		if ((row1 < 6) || (row1 > maxlines))
			row1=22;
		if ((col1 < 6) || (col1 > maxcols))
			col1=80;
		if ((check_sp == "f") || (check_sp == "F") || (check_sp == "false") || (check_sp == "False") || (check_sp == "FALSE"))
		{
			sp=false;
		}
		else
		{
			sp=true;
		}	
	}
	else if (argc == 5)
	{
		ss << argv[1];
		ss >> row1;
		ss1 << argv[2];
		ss1 >> col1;
		ss2 << argv[3];
		ss2 >> x;
		ss3 << argv[4];
		ss3 >> y;
		if ((row1 % 2) == 1)
			++row1;
		if ((col1 % 2) == 1)
			++col1;
		if ((row1 < 6) || (row1 > maxlines))
			row1=22;
		if ((col1 < 6) || (col1 > maxcols))
			col1=80;
	}
	else if (argc == 6)
	{
		ss << argv[1];
		ss >> row1;
		ss1 << argv[2];
		ss1 >> col1;
		ss2 << argv[3];
		ss2 >> x;
		ss3 << argv[4];
		ss3 >> y;
		ss4 << argv[5];
		ss4 >> check_sp;
		if ((row1 % 2) == 1)
			++row1;
		if ((col1 % 2) == 1)
			++col1;
		if ((row1 < 6) || (row1 > maxlines))
			row1=22;
		if ((col1 < 6) || (col1 > maxcols))
			col1=80;
		if ((check_sp == "f") || (check_sp == "F") || (check_sp == "false") || (check_sp == "False") || (check_sp == "FALSE"))
		{
			sp=false;
		}
		else
		{
			sp=true;
		}
	}
	else if (argc == 2)
	{
		ss << argv[1];
		ss >> m_size;
		if (m_size=='m')
		{
			row1=30;
			col1=100;
		}
		else if (m_size == 'M')
		{
			row1=30;
			col1=100;
			sp=true;
		}
		else if (m_size=='l')
		{
			row1=46;
			col1=196;
		}
		else if (m_size == 'L')
		{
			row1=46;
			col1=196;
			sp=true;
		}
		else if (m_size == 'S')
		{
			row1=22;
			col1=80;
			sp=true;		
		}
		else if ((m_size=='r') || (m_size=='R'))
		{
			cout << "input number of rows (6 <= r <= 46): ";
			cin >> row1;
			cout << "input number of cols (6 <= c <= 196): ";
			cin >> col1;
			cout << "input if you want the path shown or not? (t/f): ";
			cin >> check_sp;
			if ((row1 % 2) == 1)
				++row1;
			if ((col1 % 2) == 1)
				++col1;
			if ((row1 < 6) || (row1 > maxlines))
				row1=22;
			if ((col1 < 6) || (col1 > maxcols))
				col1=80;
			if ((check_sp == "f") || (check_sp == "F") || (check_sp == "false") || (check_sp == "False") || (check_sp == "FALSE"))
			{
				sp=false;
			}
			else
			{
				sp=true;
			}
		}
		else
		{
			row1=22;
			col1=80;
		}
	}
	else
	{
		row1 = 22;
		col1 = 80;
	}
	if (row1>maxlines-2)
	{
		row1=maxlines-2;
	}
	if (col1>maxcols)
	{
		col1=maxcols;
	}
	stringstream str;
	int players=2;
	bool shroud=true;
	bool foggy=true;
	cout << "Welcome to Legendscape" << endl;
	cout << "How many players are there? [2-4]" << endl;
	cin >> players;
	if (players<2)
		players=2;
	else if (players>4)
		players=4;
	cout << "Do you want to play with shroud? [Y/n]" << endl;
	char response = singlechar();
	cout << response << endl;
	if ((response=='n') || (response=='N'))
		shroud=false;
	cout << "Do you want to play with fog of war? [Y/n]" << endl;
	char response1 = singlechar();
	cout << response1 << endl;
	if ((response1=='n') || (response1=='N'))
		foggy=false;
	Battlefield steve(row1, col1, players, 2000, shroud, foggy);
	steve.playthegame();
	return 0;
}
