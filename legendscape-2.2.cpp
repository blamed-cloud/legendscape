//##USAGE## maze_game3_cpp [(m/l/r)/(rows)/(rows)] [()/(cols)/(cols)] [()/()/(x_start)] [()/()/(y_start)] ; sms=N ; $#=0-4
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <sstream>

//#include "unit_classes.h"
#include "maze_classes-2.2.hpp"
using namespace std;

string num2str(int num)
{
	stringstream ss;
	ss << num;
	return ss.str();
}

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
	words.setColor(sf::Color::White);
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
		
		window.clear();
		window.draw(words);
		window.display();
	}
	string output = words.getString();
	stringstream strstm;
	strstm << players;
	int size=0;
	output += "\n";
	output += strstm.str();
	output += "\nWhat map size do you want? [S/M/L]";
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
				if (event.key.code == sf::Keyboard::S)
				{
					row1=32;
					col1=96;
					notchanged=false;
					key="S";
					size=1;
				}
				else if (event.key.code == sf::Keyboard::M)
				{
					row1=64;
					col1=160;
					notchanged=false;
					key="M";
					size=2;
				}
				else if (event.key.code == sf::Keyboard::L)
				{
					row1=96;
					col1=256;
					notchanged=false;
					key="L";
					size=3;
				}
			}
		}
		
		window.clear();
		window.draw(words);
		window.display();
	}
	
	output += "\n";
	output += key;
	output += "\nDo you want to play with shroud? [Y/n]";
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
		
		window.clear();
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
		
		window.clear();
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
					key="Y";
				}
				else if (event.key.code == sf::Keyboard::N)
				{
					units=false;
					notchanged=false;
					key="N";
				}
			}
		}
		
		window.clear();
		window.draw(words);
		window.display();
	}

	output += "\n";
	output += key;
	output += "\nHow Much money do you want?\nUse the arrow keys to select an amount [1000-";
	if (size==1)
	{
		output += "2000]";
	}
	else if (size==2)
	{
		output += "3000]";
	}
	else if (size==3)
	{
		output += "4000]\n";
	}
	string amtmoney="[$$$$$$$$$$";
	for (int c = 0 ; c < size*10 ; c++)
	{
		amtmoney += " ";
	}
	amtmoney += "]\n$1000";
	string temp = output;
	temp += amtmoney;
	words.setString(temp);
	
	window.setKeyRepeatEnabled(true);
	
	int extra=0;
	int money=1000;
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
				if ((event.key.code == sf::Keyboard::Left) && (extra > 0) )
				{
					extra--;
				}
				else if ( (event.key.code == sf::Keyboard::Right) && (extra < 10*size ) )
				{
					extra++;
				}
				else if (event.key.code == sf::Keyboard::Return)
				{
					money=1000 + (extra*100);
					notchanged=false;
				}
			}
		}
		
		string amtmoney="\n[$$$$$$$$$$";
		for (int c = 0 ; c < size*10 ; c++)
		{
			if (c < extra)
				amtmoney += "$";
			else
				amtmoney += " ";
		}
		amtmoney += "]\n$";
		amtmoney += num2str(1000+extra*100);
		temp = output;
		temp += amtmoney;
		words.setString(temp);
		
		window.clear();
		window.draw(words);
		window.display();
	}
	
	window.setKeyRepeatEnabled(false);
	
	Battlefield steve(window, row1, col1, players, money, shroud, foggy, units);
	steve.playthegame(window);
	return 0;
}

