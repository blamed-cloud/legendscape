//##USAGE## maze_game3_cpp [(m/l/r)/(rows)/(rows)] [()/(cols)/(cols)] [()/()/(x_start)] [()/()/(y_start)] ; sms=N ; $#=0-4
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <sstream>

//#include "unit_classes.h"
#include "maze_classes-2.1.hpp"
using namespace std;

//////////////////////////////////////////////////////////
int main()
{
	int row1=32;
	int col1=96;
	int players=0;
	bool shroud=true;
	bool foggy=true;
	bool units=false;
	sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Legendscape");
	window.setKeyRepeatEnabled(false);
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

	
	//erase when more sprites are implemented
	players=2;
	
	Battlefield steve(window, row1, col1, players, 2000, shroud, foggy, units);
	steve.playthegame(window);
	return 0;
}

