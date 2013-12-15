#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Terrain
{
protected:
	int mapval;
	int movecost;
	string repr;
	string name;
	bool empty;
	vector<int> armyseen;
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
		if (mapval==-1)
		{
			repr="|";
			name="Wall";
		}
		else if (mapval==0)
		{
			repr=".";
			name="Test";
		}
		else
		{
			repr=" ";
			name="Plains";
		}
	}
	
	void setempty(bool val)
	{
		empty=val;
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
		if (mapval==0)
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
