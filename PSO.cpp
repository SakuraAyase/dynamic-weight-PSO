#include<vector>
#include<iostream>
#include<random>
#include<ctime>
#include<cmath>

using namespace std;

#define pi 3.1415926
const int D = 10;
const int m = 20; //define number of instances
const double c1 = 2.0;
const double c2 = 2.0;
const double wmax = 0.9;
const double wmin = 0.4;
const int epoch = 2000;
const double percent1 = 0.150;

vector<double>record;//a record for every 100 generations



double randNum(double min, double max)
{

	double temp = rand() % (9999 + 1) / (double)(9999 + 1);
	double result = temp*(max-min) + min;
	return result;
}

class species
{
public:

	vector<double>velocity;
	vector<double>location;
	vector<double>pBest;
	vector<double>gBest;
};

double weight1(int T)
{
	double t = ((double)T) / epoch;
	double w = wmax - (wmax - wmin)*t;
	return w;
}

double weight2(int T)
{
	double w = randNum(0.4, 0.6);
	return w;
}

double weight3(int T)
{
	double t = ((double)T) / epoch;
	double weight = -(wmax - wmin)*t*t + wmax;
	return weight;
}

double weight4(int T)
{
	double t = ((double)T) / epoch;
	double weight = (wmax - wmin)*(t - 1)*(t - 1) + wmin;
	return weight;
}


double func(vector<double>instance)
{
	double x = 0.0;
	for (int i = 0; i < instance.size(); i++) 
	{
		double a = instance[i];
		x += a * a;
	}
		
	return x;
}

bool better(double a, double b)
{
	if (a < b)
		return true;
	else
		return false;
}

void printv(vector<double>ve)
{
	for (int i = 0; i < ve.size(); i++)
		cout << ve[i] << " ";
	cout << endl;
}

void printClan(vector<species>clan, int order)
{
	if (order == 1)
	{
		for (int i = 0; i < clan.size(); i++)
		{
			cout << i << endl;
			cout << "G best: ";
			printv(clan[i].gBest);
			cout << "location: ";
			printv(clan[i].location);
			cout << "velocity: ";
			printv(clan[i].velocity);
			cout << "p best: ";
			printv(clan[i].pBest);
			cout << endl;
		}
	}

	if (order == 2)
	{
		cout << func(clan[0].gBest) << endl;
		for (int i = 1; i <= clan.size(); i++)
		{

			cout << func(clan[i-1].pBest) << " ";
			if (i % 5 == 0)
				cout << endl;
		}
		cout << endl << endl;;
	}

}


species calc_Velocity(species instance, double w, vector<double>min,vector<double>max)
{
	vector<double> vMax = max;
	vector<double> vMin = min; 
	for (int i = 0; i < D; i++)
	{
		vMax[i] = max[i] * percent1;
		vMin[i] = min[i] * percent1;
	}
	
	double r1 = randNum(0, 1);
	double r2 = randNum(0, 1);


	for (int i = 0; i < D; i++)
	{


		double temp = w * instance.velocity[i] + c1 * r1*(instance.pBest[i] - instance.location[i])
			+ c2 * r2*(instance.gBest[i] - instance.location[i]);

		if (temp > vMax[i])
			temp = vMax[i];
		else if (temp < vMin[i])
			temp = vMin[i];
		instance.velocity[i] = temp;
	}

	return instance;

}



vector<species>evaluateP(vector<species>clan)
{
	for (int i = 0; i < clan.size(); i++)
	{
		if (better(func(clan[i].location), func(clan[i].pBest)))
			clan[i].pBest = clan[i].location;
	}
	return clan;
}

vector<species> evaluateG(vector<species> clan)
{
	int best_index = 0;

	for (int i = 1; i < clan.size(); i++) 
	{
		if (better(func(clan[i].pBest),func(clan[best_index].pBest)))
		{
			best_index = i;
		}
	}
	for (int i = 0; i < clan.size(); i++)
	{
		clan[i].gBest = clan[best_index].pBest;
	}

	return clan;
}

vector<species> evaluate(vector<species>clan)
{
	clan = evaluateP(clan);

	clan = evaluateG(clan);

	return clan;
}

vector<species>movement(vector<species>clan,vector<double>min,vector<double>max)
{
	for (int i = 0; i < clan.size(); i++)
	{
		for (int j = 0; j < D; j++)
		{
			clan[i].location[j] = clan[i].location[j] + clan[i].velocity[j];
			if (clan[i].location[j] > max[j])
				clan[i].location[j] = max[j];
			if (clan[i].location[j] < min[j])
				clan[i].location[j] = min[j];
		}
		if (better(func(clan[i].location), func(clan[i].pBest)))
		{
			clan[i].pBest = clan[i].location;
			if (better(func(clan[i].pBest), func(clan[i].gBest))) 
			{
				for (int i = 0; i < D; i++)
				{
					clan[i].gBest = clan[i].pBest;
				}
			}
		}
			
	}
	return clan;
}

vector<species> initial(vector<double> min, vector<double> max)//m is the number of instances, D is used to define the dimention
{
	int T = 0;
	vector<species> clan;


	for (int i = 0; i < m; i++)
	{

		species instance = species();
		vector<double>sp;
		vector<double>v;

		vector<double> vMax = max;
		vector<double> vMin = min;
	
		for (int j = 0; j < D; j++)
		{
			v.push_back(randNum(vMin[j], vMax[j]));
			vMax[j] = max[j] * percent1;
			vMin[j] = min[j] * percent1;
			//v.push_back(5);
			//cout << randNum(min[j], max[j]) << endl;
			sp.push_back(randNum(min[j], max[j]));
			//sp.push_back(1);

		}
		instance.location = sp;
		instance.pBest = sp;
		instance.velocity = v;
		clan.push_back(instance);
		
	}
	
	clan = evaluate(clan);

	
	record.push_back(func(clan[0].gBest));
	
	return clan;
}

vector<species>main_Schedule(vector<double>min,vector<double>max)
{

	vector<species> clan = initial(min, max);
	for (int T = 1; T < epoch; T++) 
	{
		
		for (int i = 0; i < m; i++)
		{
			double w = weight2(T);

			clan[i] = calc_Velocity(clan[i], w, min, max);
		}

		clan = movement(clan, min, max);
		//printClan(clan, 1);
		


		clan = evaluate(clan);
		
		cout << T << endl;
		cout << func(clan[0].pBest)<<" ";
		cout << func(clan[0].gBest) <<" ";
		cout << func(clan[0].location) << endl;
		printv(clan[0].velocity);
		cout << endl;
		
		//printClan(clan, 1);
		if (T % 100 == 0)
		{
			record.push_back(func(clan[0].gBest));
		}
	}
	return clan;
}

int main()
{

	vector<double>min;
	vector<double>max;
	for (int i = 0; i < D; i++)
	{
		min.push_back(-100);
		max.push_back(100);
	}
	vector<species>clan = main_Schedule(min, max);
	cout << endl;
	printv(clan[0].pBest);
	printClan(clan,2);
	cout << endl;
	printv(record);

}