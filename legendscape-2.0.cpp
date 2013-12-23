//##USAGE## maze_game3_cpp [(m/l/r)/(rows)/(rows)] [()/(cols)/(cols)] [()/()/(x_start)] [()/()/(y_start)] ; sms=N ; $#=0-4
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include <sys/ioctl.h>
#include <cstdio>
#include <unistd.h>

//#include "unit_classes.h"
#include "maze_classes-2.0.hpp"
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
	int players=0;
	bool shroud=true;
	bool foggy=true;
	bool units=false;
	
	
	sf::RenderWindow window(sf::VideoMode(1024, 512), "Legendscape");
	sf::Font font;
	font.loadFromFile("DroidSansMono.ttf");
	sf::Text words("Welcome to Legendscape!\nHow many players are there? [2-4]",font,32);
	words.setColor(sf::Color::Black);
	
	while (players==0)
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type==sf::Event::Closed)
			{
				window.close();
				return -1;
			}
			else if (event.type == sf::Event::KeyPressed)
			{
				if ( (event.key.code == sf::Keyboard::Num2) || (event.key.code == sf::Keyboard::Numpad2) )
				{
					players=2;
				}
				else if ( (event.key.code == sf::Keyboard::Num3) || (event.key.code == sf::Keyboard::Numpad3) )
				{
					players=3;
				}
				else if ( (event.key.code == sf::Keyboard::Num4) || (event.key.code == sf::Keyboard::Numpad4) )
				{
					players=4;
				}
			}
		}
		
		window.clear(sf::Color::White);
		window.draw(words);
		window.display();
	}
	
	string output = words.getString();
	stringstream strstm;
	strstm << players;
	output += "\n";
	output += strstm.str();
	output += "\nDo you want to play with shroud? [Y/n]";
	
	words.setString(output);
	
	string key;
	bool notchanged=true;
	while (notchanged)
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
				return -1;
			}
			else if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Y)
				{
					shroud=true;
					notchanged=false;
					key="Y";
				}
				else if (event.key.code == sf::Keyboard::N)
				{
					shroud=false;
					notchanged=false;
					key="N";
				}
			}
		}
		
		window.clear(sf::Color::White);
		window.draw(words);
		window.display();
	}
	
	output += "\n";
	output += key;
	output += "\nDo you want to play with fog of war? [Y/n]";
	
	words.setString(output);
	
	notchanged=true;
	while (notchanged)
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
				return -1;
			}
			else if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Y)
				{
					foggy=true;
					notchanged=false;
					key="Y";
				}
				else if (event.key.code == sf::Keyboard::N)
				{
					foggy=false;
					notchanged=false;
					key="N";
				}
			}
		}
		
		window.clear(sf::Color::White);
		window.draw(words);
		window.display();
	}
	
	output += "\n";
	output += key;
	output += "\nDo you want to be able to purchase reinforcements? [y/N]";
	
	words.setString(output);

	notchanged=true;
	while (notchanged)
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
				return -1;
			}
			else if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Y)
				{
					units=true;
					notchanged=false;
				}
				else if (event.key.code == sf::Keyboard::N)
				{
					units=false;
					notchanged=false;
				}
			}
		}
		
		window.clear(sf::Color::White);
		window.draw(words);
		window.display();
	}
	
	
	window.close();
	Battlefield steve(row1, col1, players, 2000, shroud, foggy, units);
	steve.playthegame();
	return 0;
}

