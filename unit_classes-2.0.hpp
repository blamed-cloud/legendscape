#include <SFML/Graphics.hpp>
#include <unistd.h>
#include <termios.h>
#include <iostream>
#include <string>
#include <random>
#include <chrono>
#include <vector>
#include <sstream>
#include <time.h>

using namespace std;


class Scout;
class Swordsman;
class Halberdier;
class Axeman;
class Archer;
class Mage;
class Swordmage;
class Knight;
class Wizard;
class Elf_Archer;
class Berserker;
class Calvalry;

class Unit
{
protected:
	int totalhp,move,range,atk,def,cost;
	int sight;
	bool cutcorners;
	bool lineofsight;
	bool dead;
	int adjacentatkbonus;
	int adjacentdefbonus;
	int xcoor,ycoor;
	int hp;
	int type;
	int tier;
	int mapvalue;
	char repr;
	int movesleft;
	bool canattack;
	string name;
	sf::Sprite sprite;
	
public:
	
	friend class Scout;
	friend class Swordsman;
	friend class Halberdier;
	friend class Axeman;
	friend class Archer;
	friend class Mage;
	friend class Swordmage;
	friend class Knight;
	friend class Wizard;
	friend class Elf_Archer;
	friend class Berserker;
	friend class Calvalry;
	
	Unit()
	{
		movesleft=0;
		canattack=true;
		totalhp=0;
		hp=totalhp;
		move=0;
		range=0;
		sight=0;
		atk=0;
		def=0;
		cost=9001;
		cutcorners=false;
		lineofsight=true;
		adjacentatkbonus=0;
		xcoor=0;
		ycoor=0;
		type=0;
		tier=0;
		dead=false;
		mapvalue=0;
		repr=' ';
		name="Unit";
	}

	sf::Sprite getSprite()
	{
		return sprite;
	}

	void setSprite(sf::Sprite temp)
	{
		sprite=temp;
	}
	
	void resetsight()
	{
		if (range>=move)
			sight=range+5;
		else
			sight=move+5;
	}
	
	string getname()
	{
		return name;
	}
	
	void resetmove()
	{
		movesleft=move;
	}
	
	void resetattack()
	{
		canattack=true;
	}
	
	void unsetattack()
	{
		canattack=false;
	}
	
	void setmovesleft(int m)
	{
		movesleft=m;
	}
	
	int getmovesleft()
	{
		return movesleft;
	}
	
	bool canstillatk()
	{
		return canattack;
	}
	
	int getstat(int stat)  //0 = hp_max ; 1 = atk ; 2 = def ; 3 = range ; 4 = move
	{
		if (stat == 0)
		{
			return totalhp;
		}
		else if (stat==1)
		{
			return atk;
		}
		else if (stat==2)
		{
			return def;
		}
		else if (stat==3)
		{
			return range;
		}
		else
		{
			return move;
		}
	}
	
	int getmapval()
	{
		return mapvalue;
	}
	
	char getrepr()
	{
		return repr;
	}
	
	int getcost()
	{
		return cost;
	}
	
	int gettype()
	{
		return type;
	}
	
	int gettier()
	{
		return tier;
	}
	
	bool cancutcorners()
	{
		return cutcorners;
	}
	
	int getsight()
	{
		return sight;
	}
	
	bool needlineofsight()
	{
		return lineofsight;
	}
	
	int getadjatkbonus()
	{
		return adjacentatkbonus;
	}
	
	int getadjdefbonus()
	{
		return adjacentdefbonus;
	}
	
	int getxcoor()
	{
		return xcoor;
	}
	
	int getycoor()
	{
		return ycoor;
	}
	
	int getcoor(int which) // 0=x ; 1=y
	{
		if (which==0)
		{
			return xcoor;
		}
		else
		{
			return ycoor;
		}
	}
	
	int currenthp()
	{
		return hp;
	}
	
	void setxcoor(int x)
	{
		xcoor=x;
	}
	
	void setycoor(int y)
	{
		ycoor=y;
	}
	
	bool isdead()
	{
		return dead;
	}
	
	bool isalive()
	{
		return (!dead);
	}
	
	void sethp(int health)
	{
		if (health<=0)
		{
			hp=0;
			dead=true;
		}
		else if ( health>=totalhp)
		{
			hp=totalhp;
		}
		else
		{
			hp=health;
		}
	}
	
	void dodamage(int damage)
	{
		hp=hp-damage;
		if (hp <= 0)
		{
			hp=0;
			dead=true;
		}
	}

	int num_btwn(int low=1, int high=10)
	{
		unsigned seed = chrono::system_clock::now().time_since_epoch().count();
		minstd_rand0 rand (seed);
		return (rand() % (high - low + 1)) + low;
	}
	
	int rolld6()
	{
		return num_btwn(1,6);
	}
	
	int rollatkdice(int num_dice=1)
	{
		int counter=0;
		int roll=1;
		for ( int k=0; k < num_dice ; k++ )
		{
			roll=rolld6();
			if ((roll % 2)==0)
			{
				counter++;
			}
		}
		return counter;
	}
	
	int rolldefdice(int num_dice=1)
	{
		int counter=0;
		int roll=1;
		for ( int k=0; k < num_dice ; k++ )
		{
			roll=rolld6();
			if ((roll==3)||(roll==5))
			{
				counter++;
			}
		}
		return counter;
	}
	
	int myatkdice()
	{
		return rollatkdice(atk);
	}
	
	int mydefdice()
	{
		return rolldefdice(def);
	}
};

class Scout//: public Unit
{
public:
// 	Scout(int x=0,int y=0)
// 	{
// 		totalhp=5;
// 		hp=totalhp;
// 		move=15;
// 		range=1;
// 		atk=3;
// 		def=5;
// 		cost=50;
// 		cutcorners=false;
// 		lineofsight=true;
// 		adjacentatkbonus=0;
// 		xcoor=x;
// 		ycoor=y;
// 		type=0;
// 		tier=1;
// 		dead=false;
// 		mapvalue=-10;
// 		repr='S';
// 	}
	
	Unit resetvalues(Unit a)
	{
		a.totalhp=5;
		a.hp=a.totalhp;
		a.move=12;
		a.range=1;
		a.atk=3;
		a.def=5;
		a.cost=50;
		a.cutcorners=false;
		a.lineofsight=true;
		a.adjacentatkbonus=0;
		a.adjacentdefbonus=0;
		a.xcoor=0;
		a.ycoor=0;
		a.type=0;
		a.tier=1;
		a.dead=false;
		a.mapvalue=-10;
		a.repr='S';
		a.name="Scout";
		return a;
	}
};

class Swordsman//: public Unit
{
public:
// 	Swordsman(int x=0,int y=0)
// 	{
// 		totalhp=12;
// 		hp=totalhp;
// 		move=8;
// 		range=1;
// 		atk=6;
// 		def=6;
// 		cost=100;
// 		cutcorners=true;
// 		lineofsight=true;
// 		adjacentatkbonus=0;
// 		xcoor=x;
// 		ycoor=y;
// 		type=1;
// 		tier=1;
// 		dead=false;
// 		mapvalue=-11;
// 		repr='R';
// 	}
	
	Unit resetvalues(Unit a)
	{
		a.totalhp=12;
		a.hp=a.totalhp;
		a.move=8;
		a.range=1;
		a.atk=6;
		a.def=6;
		a.cost=100;
		a.cutcorners=true;
		a.lineofsight=true;
		a.adjacentatkbonus=0;
		a.adjacentdefbonus=0;
		a.xcoor=0;
		a.ycoor=0;
		a.type=1;
		a.tier=1;
		a.dead=false;
		a.mapvalue=-11;
		a.repr='R';
		a.name="Swordsman";
		return a;
	}
};

class Halberdier//: public Unit
{
public:
// 	Halberdier(int x=0,int y=0)
// 	{
// 		totalhp=12;
// 		hp=totalhp;
// 		move=8;
// 		range=2;
// 		atk=7;
// 		def=4;
// 		cost=100;
// 		cutcorners=false;
// 		lineofsight=true;
// 		adjacentatkbonus=-1;
// 		xcoor=x;
// 		ycoor=y;
// 		type=2;
// 		tier=1;
// 		dead=false;
// 		mapvalue=-12;
// 		repr='H';
// 	}
	
	Unit resetvalues(Unit a)
	{
		a.totalhp=12;
		a.hp=a.totalhp;
		a.move=8;
		a.range=2;
		a.atk=7;
		a.def=4;
		a.cost=100;
		a.cutcorners=false;
		a.lineofsight=true;
		a.adjacentatkbonus=-1;
		a.adjacentdefbonus=0;
		a.xcoor=0;
		a.ycoor=0;
		a.type=2;
		a.tier=1;
		a.dead=false;
		a.mapvalue=-12;
		a.repr='H';
		a.name="Halberdier";
		return a;
	}
};

class Axeman//: public Unit
{
public:
// 	Axeman(int x=0,int y=0)
// 	{
// 		totalhp=15;
// 		hp=totalhp;
// 		move=6;
// 		range=1;
// 		atk=8;
// 		def=3;
// 		cost=100;
// 		cutcorners=false;
// 		lineofsight=true;
// 		adjacentatkbonus=0;
// 		xcoor=x;
// 		ycoor=y;
// 		type=3;
// 		tier=1;
// 		dead=false;
// 		mapvalue=-13;
// 		repr='X';
// 	}
	
	Unit resetvalues(Unit a)
	{
		a.totalhp=15;
		a.hp=a.totalhp;
		a.move=7;
		a.range=1;
		a.atk=8;
		a.def=3;
		a.cost=100;
		a.cutcorners=false;
		a.lineofsight=true;
		a.adjacentatkbonus=0;
		a.adjacentdefbonus=0;
		a.xcoor=0;
		a.ycoor=0;
		a.type=3;
		a.tier=1;
		a.dead=false;
		a.mapvalue=-13;
		a.repr='X';
		a.name="Axeman";
		return a;
	}
};

class Archer//: public Unit
{
public:
// 	Archer(int x=0,int y=0)
// 	{
// 		totalhp=8;
// 		hp=totalhp;
// 		move=7;
// 		range=12;
// 		atk=3;
// 		def=3;
// 		cost=150;
// 		cutcorners=false;
// 		lineofsight=true;
// 		adjacentatkbonus=1;
// 		xcoor=x;
// 		ycoor=y;
// 		type=4;
// 		tier=1;
// 		dead=false;
// 		mapvalue=-14;
// 		repr='A';
// 	}
	
	Unit resetvalues(Unit a)
	{
		a.totalhp=8;
		a.hp=a.totalhp;
		a.move=8;
		a.range=12;
		a.atk=5;
		a.def=5;
		a.cost=150;
		a.cutcorners=false;
		a.lineofsight=true;
		a.adjacentatkbonus=-1;
		a.adjacentdefbonus=-2;
		a.xcoor=0;
		a.ycoor=0;
		a.type=4;
		a.tier=1;
		a.dead=false;
		a.mapvalue=-14;
		a.repr='A';
		a.name="Archer";
		return a;
	}
};

class Mage//: public Unit
{
public:
// 	Mage(int x=0,int y=0)
// 	{
// 		totalhp=7;
// 		hp=totalhp;
// 		move=6;
// 		range=8;
// 		atk=6;
// 		def=2;
// 		cost=150;
// 		cutcorners=false;
// 		lineofsight=false;
// 		adjacentatkbonus=-2;
// 		xcoor=x;
// 		ycoor=y;
// 		type=5;
// 		tier=1;
// 		dead=false;
// 		mapvalue=-15;
// 		repr='M';
// 	}
	
	Unit resetvalues(Unit a)
	{
		a.totalhp=7;
		a.hp=a.totalhp;
		a.move=8;
		a.range=8;
		a.atk=6;
		a.def=6;
		a.cost=150;
		a.cutcorners=false;
		a.lineofsight=false;
		a.adjacentatkbonus=-2;
		a.adjacentdefbonus=-4;
		a.xcoor=0;
		a.ycoor=0;
		a.type=5;
		a.tier=1;
		a.dead=false;
		a.mapvalue=-15;
		a.repr='M';
		a.name="Mage";
		return a;
	}
};

class Swordmage//: public Unit
{
public:
// 	Swordmage(int x=0,int y=0)
// 	{
// 		totalhp=20;
// 		hp=totalhp;
// 		move=8;
// 		range=8;
// 		atk=8;
// 		def=8;
// 		cost=500;
// 		cutcorners=true;
// 		lineofsight=false;
// 		adjacentatkbonus=4;
// 		xcoor=x;
// 		ycoor=y;
// 		type=6;
// 		tier=2;
// 		dead=false;
// 		mapvalue=-16;
// 		repr='D';
// 	}
	
	Unit resetvalues(Unit a)
	{
		a.totalhp=20;
		a.hp=a.totalhp;
		a.move=8;
		a.range=8;
		a.atk=8;
		a.def=8;
		a.cost=500;
		a.cutcorners=true;
		a.lineofsight=false;
		a.adjacentatkbonus=4;
		a.adjacentdefbonus=0;
		a.xcoor=0;
		a.ycoor=0;
		a.type=6;
		a.tier=2;
		a.dead=false;
		a.mapvalue=-16;
		a.repr='D';
		a.name="Swordmage";
		return a;
	}
};

class Knight//: public Unit
{
public:
// 	Knight(int x=0,int y=0)
// 	{
// 		totalhp=25;
// 		hp=totalhp;
// 		move=8;
// 		range=1;
// 		atk=14;
// 		def=6;
// 		cost=500;
// 		cutcorners=false;
// 		lineofsight=true;
// 		adjacentatkbonus=0;
// 		xcoor=x;
// 		ycoor=y;
// 		type=7;
// 		tier=2;
// 		dead=false;
// 		mapvalue=-17;
// 		repr='K';
// 	}
	
	Unit resetvalues(Unit a)
	{
		a.totalhp=25;
		a.hp=a.totalhp;
		a.move=7;
		a.range=1;
		a.atk=7;
		a.def=8;
		a.cost=500;
		a.cutcorners=false;
		a.lineofsight=true;
		a.adjacentatkbonus=0;
		a.adjacentdefbonus=4;
		a.xcoor=0;
		a.ycoor=0;
		a.type=7;
		a.tier=2;
		a.dead=false;
		a.mapvalue=-17;
		a.repr='K';
		a.name="Knight";
		return a;
	}
};

class Wizard//: public Unit
{
public:
// 	Wizard(int x=0,int y=0)
// 	{
// 		totalhp=10;
// 		hp=totalhp;
// 		move=7;
// 		range=12;
// 		atk=10;
// 		def=5;
// 		cost=500;
// 		cutcorners=false;
// 		lineofsight=false;
// 		adjacentatkbonus=-2;
// 		xcoor=x;
// 		ycoor=y;
// 		type=8;
// 		tier=2;
// 		dead=false;
// 		mapvalue=-18;
// 		repr='W';
// 	}
	
	Unit resetvalues(Unit a)
	{
		a.totalhp=10;
		a.hp=a.totalhp;
		a.move=8;
		a.range=12;
		a.atk=10;
		a.def=8;
		a.cost=500;
		a.cutcorners=false;
		a.lineofsight=false;
		a.adjacentatkbonus=-2;
		a.adjacentdefbonus=-3;
		a.xcoor=0;
		a.ycoor=0;
		a.type=8;
		a.tier=2;
		a.dead=false;
		a.mapvalue=-18;
		a.repr='W';
		a.name="Wizard";
		return a;
	}
};

class Elf_Archer//: public Unit
{
public:
// 	Elf_Archer(int x=0,int y=0)
// 	{
// 		totalhp=10;
// 		hp=totalhp;
// 		move=10;
// 		range=15;
// 		atk=6;
// 		def=5;
// 		cost=500;
// 		cutcorners=false;
// 		lineofsight=true;
// 		adjacentatkbonus=2;
// 		xcoor=x;
// 		ycoor=y;
// 		type=9;
// 		tier=2;
// 		dead=false;
// 		mapvalue=-19;
// 		repr='E';
// 	}
	
	Unit resetvalues(Unit a)
	{
		a.totalhp=10;
		a.hp=a.totalhp;
		a.move=10;
		a.range=15;
		a.atk=8;
		a.def=7;
		a.cost=500;
		a.cutcorners=false;
		a.lineofsight=true;
		a.adjacentatkbonus=-2;
		a.adjacentdefbonus=-2;
		a.xcoor=0;
		a.ycoor=0;
		a.type=9;
		a.tier=2;
		a.dead=false;
		a.mapvalue=-19;
		a.repr='E';
		a.name="Elf Archer";
		return a;
	}
};

class Berserker//: public Unit
{
public:
// 	Berserker(int x=0,int y=0)
// 	{
// 		totalhp=30;
// 		hp=totalhp;
// 		move=10;
// 		range=1;
// 		atk=16;
// 		def=0;
// 		cost=500;
// 		cutcorners=false;
// 		lineofsight=true;
// 		adjacentatkbonus=0;
// 		xcoor=x;
// 		ycoor=y;
// 		type=10;
// 		tier=2;
// 		dead=false;
// 		mapvalue=-20;
// 		repr='B';
// 	}
	
	Unit resetvalues(Unit a)
	{
		a.totalhp=30;
		a.hp=a.totalhp;
		a.move=10;
		a.range=1;
		a.atk=16;
		a.def=0;
		a.cost=500;
		a.cutcorners=false;
		a.lineofsight=true;
		a.adjacentatkbonus=0;
		a.adjacentdefbonus=2;
		a.xcoor=0;
		a.ycoor=0;
		a.type=10;
		a.tier=2;
		a.dead=false;
		a.mapvalue=-20;
		a.repr='B';
		a.name="Berserker";
		return a;
	}
};

class Calvalry//: public Unit
{
public:
// 	Calvalry(int x=0,int y=0)
// 	{
// 		totalhp=25;
// 		hp=totalhp;
// 		move=15;
// 		range=2;
// 		atk=10;
// 		def=5;
// 		cost=500;
// 		cutcorners=false;
// 		lineofsight=true;
// 		adjacentatkbonus=0;
// 		xcoor=x;
// 		ycoor=y;
// 		type=11;
// 		tier=2;
// 		dead=false;
// 		mapvalue=-21;
// 		repr='C';
// 	}
	
	Unit resetvalues(Unit a)
	{
		a.totalhp=25;
		a.hp=a.totalhp;
		a.move=15;
		a.range=2;
		a.atk=10;
		a.def=5;
		a.cost=500;
		a.cutcorners=false;
		a.lineofsight=true;
		a.adjacentatkbonus=0;
		a.adjacentdefbonus=0;
		a.xcoor=0;
		a.ycoor=0;
		a.type=11;
		a.tier=2;
		a.dead=false;
		a.mapvalue=-21;
		a.repr='C';
		a.name="Calvalry";
		return a;
	}
};

class Army
{
protected:
//	Unit * unitarray[];
	vector<Unit> unitarray;
	int num_units;
	int startingfunds;
	int money;
	int maxunits;
	int colorcode;
	string colorname;
	string armyname;
	bool stillplaying;
	string password;
	bool units2bringin;
	vector<Unit> reinforcements;
	bool computer;
	sf::Color teamcolor;
	
public:
	Army(int dollars=2000,bool isai=false)
	{
		startingfunds=dollars;
		stillplaying=true;
		computer=isai;
		money=startingfunds;
		num_units=0;
		maxunits=20;
		colorcode=0;
		colorname="Blue";
		armyname=colorname + " Team";
	}
	
	Unit operator [] (int index)
	{
		return unitarray[index];
	}

	void setColor(sf::Color colour)
	{
		teamcolor=colour;
	}
	
	sf::Color getColor()
	{
		return teamcolor;
	}
	
	bool iscomputer()
	{
		return computer;
	}
	
	string passwordentry(sf::Font font, string isprompt="Type the password:")		//use SFML text		(done)
	{
		sf::RenderWindow window(sf::VideoMode(1000, 200), "Password Entry");
		sf::Text prompt;
		prompt.setFont(font);
		prompt.setCharacterSize(32);
		prompt.setColor(teamcolor);
		prompt.setString(isprompt);
		string pass="";
		while (window.isOpen())
		{
			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
				{
					window.close();
					return "password";
				}
				if (event.type == sf::Event::KeyPressed)
				{
					if (event.key.code == sf::Keyboard::Return)
					{
						window.close();
						return pass;
					}
				}
				else if (event.type == sf::Event::TextEntered)
				{
					if ( (event.text.unicode < 128) && (event.text.unicode > 31) )
						pass += event.text.unicode;
				}
			}
			
			window.clear(sf::Color::White);
			window.draw(prompt);
			window.display();
		}
	}
	
	void setpass(sf::Font font)
	{
		string initpass="";
		string check="a";
		string firstprompt="Please enter a password for this army:";
		while (check!=initpass)
		{
//			cout << "Please enter a password for this army:" << endl;
			initpass=passwordentry(font,firstprompt);
//			cout << "Please re-enter the password to double-check." << endl;
			check=passwordentry(font,"Please re-enter the password to double-check.");
			firstprompt="Please enter a password for this army:";
			if (check!=initpass)
				firstprompt = firstprompt + "\nPasswords didn't match, please try agian.";
		}
		password=initpass;
	}
	
	bool checkpass(string ispass)
	{
		if (ispass==password)
			return true;
		else
			return false;
	}
	
	void setplaying(bool still)
	{
		stillplaying=still;
	}
	
	bool isplaying()
	{
		return stillplaying;
	}
	
	void refreshname()
	{
		armyname=colorname + " Team";
	}
	
	void setname(string name)
	{
		armyname=name;
	}
	
	string getname()
	{
		return armyname;
	}
	
	string getcolorname()
	{
		return colorname;
	}
	
	void setcolorname(string name)
	{
		colorname=name;
	}
	
	void setcolorcode(int newcode)
	{
		colorcode=newcode;
	}
	
	int getcolorcode()
	{
		return colorcode;
	}
	
	void add_money(int extra)
	{
		money=money + extra;
	}
	
	string rollcall()
	{
		string str = "";
		for ( int c = 0 ; c < num_units ; c++ )
		{
			str = str + unitarray[c].getrepr();
		}
		return str;
	}
	
	string r_rollcal()
	{
		string str = "";
		for ( int c = 0 ; c < reinforcements.size() ; c++ )
		{
			str = str + reinforcements[c].getrepr();
		}
		return str;
	}
	
	int getmoney()
	{
		return money;
	}
	
	int initialmoney()
	{
		return startingfunds;
	}
	
	int maxsize()
	{
		return maxunits;
	}
	
	int size()
	{
		return unitarray.size();
	}
	
	void buyreinforcements(Unit recruit)
	{
		int price = recruit.getcost();
		if (( price <= money) && (num_units < maxunits))
		{
			money = money - price;
			reinforcements.push_back(recruit);
			units2bringin = true;
		}
		else
		{
			cout << "You do not have enough money left for that unit." << endl;
		}
	}
	
	int r_size()
	{
		return reinforcements.size();
	}
	
	void setreinforcement(Unit a, int c)
	{
		reinforcements[c]=a;
	}
	
	Unit getreinforcement(int c)
	{
		return reinforcements[c];
	}
	
	void sendinreinforcements()
	{
		Unit temp;
		while (reinforcements.size() > 0)
		{
			temp = reinforcements[reinforcements.size()-1];
			reinforcements.pop_back();
			unitarray.push_back(temp);
			num_units++;
		}
	}
	
	void buyunit( Unit recruit)
	{
		int price = recruit.getcost();
		if (( price <= money) && (num_units < maxunits))
		{
			money = money - price;
			unitarray.push_back(recruit);
			num_units++;
		}
		else
		{
			cout << "You do not have enough money left for that unit" << endl;
		}
	}
	
	bool aretherereinforcements()
	{
		return units2bringin;
	}
	
	void setunit(Unit a, int place)
	{
		if ((place>=0) && (place < unitarray.size()))
		{
			unitarray[place]=a;
		}
		else if (place < 0)
		{
			unitarray[0]=a;
		}
		else
		{
			unitarray[unitarray.size()-1]=a;
		}
	}
	
	Unit getunit(int place)
	{
		if ((place>=0) && (place < unitarray.size()))
		{
			return unitarray[place];
		}
		else if (place < 0)
		{
			return unitarray[0];
		}
		else
		{
			return unitarray[unitarray.size()-1];
		}
	}
	
	vector<Unit> bringoutyourdead()
	{
		vector<Unit> alive;
		vector<Unit> dead;
		Unit a;
		for ( int c = 0; c < unitarray.size() ; c++ )
		{
			a=getunit(c);
			if (a.isdead())
			{
				dead.push_back(a);
			}
			else
			{
				alive.push_back(a);
			}
		}
		num_units=alive.size();
		unitarray=alive;
		if (num_units==0)
			stillplaying=false;
		return dead;
	}
};

class Armies
{
protected:
	int numteams;
	vector<Army> armyarray;
	vector<int> friends;
	int current_team;
	int current_unit;
	Unit tempunit;
	vector<Unit> deadsoldiers;
public:
	
	Armies()
	{
		numteams=0;
		int current_team=0;
		int current_unit=0;
	}

	Army operator [] (int index)
	{
		return armyarray[index];
	}
	
	int size()
	{
		return armyarray.size();
	}
	
	void add_army(Army a)
	{
		armyarray.push_back(a);
		refreshnumteams();
	}
	
	void setnumteams(int num)
	{
		numteams=num;
	}
	
	void refreshnumteams()
	{
		numteams=armyarray.size();
	}
	
	int getnumteams()
	{
		return numteams;
	}
	
	Army getarmy(int c)
	{
		if ((c>=0) && (c<armyarray.size()))
		{
			return armyarray[c];
		}
		else if (c<0)
		{
			return armyarray[0];
		}
		else
		{
			return armyarray[armyarray.size()-1];
		}
	}
	
	void setarmy(Army a, int c)
	{
		if ((c>=0) && (c<armyarray.size()))
		{
			armyarray[c]=a;
		}
		else if (c<0)
		{
			armyarray[0]=a;
		}
		else
		{
			armyarray[armyarray.size()-1]=a;
		}
	}
	
	int coorstopos(int y, int x, int xmax)
	{
		return y*xmax + x;
	}
	
	void setfriends(int k, int columns)
	{
		vector<int> temp;
		friends=temp;
		if (k<0)
		{
			k=0;
		}
		else if (k>=armyarray.size())
		{
			k=armyarray.size()-1;
		}
		for ( int c = 0 ; c < armyarray[k].size() ; c++ )
		{
			tempunit=armyarray[k].getunit(c);
			int pos = coorstopos(tempunit.getycoor(),tempunit.getxcoor(),columns);
			friends.push_back(pos);
		}
	}

	void setcurrentarmyval(int k)
	{
		current_team=k;
	}
	
	void setcurrentunitval(int c)
	{
		current_unit=c;
	}
	
	int getcurrentunitval()
	{
		return current_unit;
	}
	
	int getcurrentarmyval()
	{
		return current_team;
	}
	
	Unit getarmyunit(int k, int c)
	{
		return armyarray[k].getunit(c);
	}
	
	void setarmyunit(Unit a,int k, int c)
	{
		armyarray[k].setunit(a,c);
	}

	Unit getcurrentunit()
	{
		return armyarray[current_team].getunit(current_unit);
	}
		
	void setcurrentunit(Unit a)
	{
		armyarray[current_team].setunit(a,current_unit);
	}
	
	Unit find_unit(int y, int x, bool setvalues=false)
	{
		int ytest, xtest;
		Unit a;
		for ( int k = 0 ; k < armyarray.size() ; k++)
		{
			for ( int c = 0 ; c < armyarray[k].size() ; c++ )
			{
				a=armyarray[k].getunit(c);
				ytest=a.getycoor();
				xtest=a.getxcoor();
				if ((y==ytest) && (x==xtest))
				{
					if (setvalues)
					{
						setcurrentarmyval(k);
						setcurrentunitval(c);
					}
					return a;
				}	
			}
		}
	}
	
	bool pos_is_friend(int pos)
	{
		for (int c = 0 ; c < friends.size() ; c++ )
		{
			if (pos == friends[c])
			{
				return true;
			}
		}
		return false;
	}
	
	void deadunitcleanup()
	{
		vector<Unit> newly_dead, temp1;
		for( int k = 0 ; k < armyarray.size() ; k++ )
		{
			newly_dead=temp1;
			newly_dead = armyarray[k].bringoutyourdead();
			for (int j = 0 ; j < newly_dead.size() ; j++)
			{
				deadsoldiers.push_back(newly_dead[j]);
			}
		}
	}
};
