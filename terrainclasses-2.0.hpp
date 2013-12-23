#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Terrain
{
protected:
	int mapval;
	int testval;
	int movecost;
	int textureval;
	string repr;
	string name;
	bool empty;
	vector<int> armyseen;
	bool drawn;
public:
	Terrain(int numarmies, int mvalue=1, int mcost=1, bool is_shroud=true, bool is_fog=true)
	{
		for( int c = 0 ; c<numarmies ; c++)
		{
			if (is_shroud)
			{
				armyseen.push_back(2);			//shroud
			}
			else
			{
				if (is_fog)
				{
					armyseen.push_back(1);		//fog
				}
				else
				{
					armyseen.push_back(0);		//clear
				}
			}
		}
		mapval=mvalue;
		movecost=mcost;
		empty=true;
		testval=1;
		drawn=false;
		if (mapval==-1)
		{
			repr="|";
			name="Wall";
			testval=-1;
			textureval=4;
		}
		else
		{
			repr=" ";
			name="Plains";
			textureval=0;
		}
	}
	
	int getTextureVal()
	{
		return textureval;
	}
	
	int getTempTexture(int team)
	{
		if (armyseen[team]==2)
			return 6;
		else
			return textureval;
	}
	
	void setempty(bool val)
	{
		empty=val;
	}
	
	bool isdrawn()
	{
		return drawn;
	}
	
	void setdrawn(bool drew)
	{
		drawn=drew;
	}
	
	bool isempty()
	{
		return empty;
	}
	
	void flipempty()
	{
		if (empty)
			empty=false;
		else
			empty=true;		
	}
	
	void settestval(int test)
	{
		testval=test;
	}
	
	int gettestval()
	{
		return testval;
	}
	
	void setmapvalue(int mval)
	{
		mapval=mval;
	}
	
	int getmapvalue()
	{
		return mapval;
	}
	
	void setmovecost(int cost)
	{
		movecost=cost;
	}
	
	int getmovecost()
	{
		return movecost;
	}
	
	string show()
	{
		return repr;
	}
	
	string tempshow(bool atk=false)
	{
		string name=repr;
		if (testval==0)
		{
			if (atk)
			{
				name="*";
			}
			else
			{
				name=".";
			}
		}
		return name;
	}
	
	void setrepr(string newrepr)
	{
		repr=newrepr;
	}
	
	void setname(string newname)
	{
		name=newname;
	}
	
	string getname()
	{
		return name;
	}
	
	int getarmysight(int k)
	{
		return armyseen[k];
	}
	
	void setarmysight(int k, int value)
	{
		armyseen[k]=value;
	}
	
	void addarmysight(int value=2)
	{
		armyseen.push_back(2);
	}
	
};

