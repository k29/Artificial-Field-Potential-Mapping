#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define xsize 10
#define ysize 10
#define no_obstacles 5
#define r_rho0 3
#define k_repulsive 5
#define k_attractive 5

using namespace std;


int grid[xsize][ysize];
struct points
{
	int x;
	int y;
};

struct obs_repulsion
{
	double mag;
	double direct;
	double x_mag;
	double y_mag;
};

struct goal_attraction
{
	double mag;
	double direct;
	double x_mag;
	double y_mag;
};

struct points obstacles[no_obstacles],current,goal;
struct obs_repulsion obs_info[no_obstacles];
struct goal_attraction goal_info;
double rforce,rho,rdirect,aforce,dist,adirect;

double attractive_magnitude(struct points a,struct points b) // a is the curretn location and b is the goal location
{
	dist=sqrt(pow((b.x-a.x),2)+pow((b.y-a.y),2));
	aforce=k_attractive*(1/pow(dist,2))+k_attractive/20*dist;
	cout<<"\naforce: "<<aforce;
	return aforce;
}

double attractive_direction(struct points a, struct points b) //a is the current location and b is the goal location
{
	adirect=atan2((b.y-a.y),(b.x-a.x));
	cout<<"\nadirect: "<<adirect;
	return adirect;
}



double repulsive_magnitude(struct points a,struct points b) // a is the current location and b is the obstacle location
{
	
	rho=sqrt(pow((a.x-b.x),2)+pow((a.y-b.y),2));
	cout<<"\na.x"<<a.x;
		cout<<"\na.y"<<a.y;
		cout<<"\nb.x"<<b.x;
		cout<<"\nb.y"<<b.y;
		cout<<"\nrho"<<rho;
	
	if(rho<r_rho0) //region of influence
	{

		//rforce=k_repulsive*(1/rho-1/r_rho0)*1/pow(rho,2);
		rforce=k_repulsive*(1/pow(rho,2));
	}
	else
	{
		rforce=0;
		cout<<"genorai";
	}
	cout<<"\nrforce: "<<rforce;
	return rforce;
	
}

double repulsive_direction(struct points a,struct points b) // a is the current location and b is the obstacle location
{
	rdirect=atan2((b.y-a.y),(b.x-a.x));
	rdirect+=3.14;
	if (rdirect>6.2831)
		rdirect=rdirect-6.2831;
	cout<<"\nrdirect: "<<rdirect;
	return rdirect;
	
}


void printgrid()
{
	cout<<"\n\n\n\n\n\n";
	for(int i=0;i<xsize;i++)
	{
		for(int j=0;j<ysize;j++)
		{
			if(grid[i][j]==-1)
				cout<<"*"<<" ";

			else if(grid[i][j]==-2)
				cout<<"-"<<" ";
			

			else 
			{
				cout<<grid[i][j]<<" ";
			}
				

		}
		cout<<"\n";
	}
	
}

int main()
{


srand(time(NULL));
for(int i=0;i<xsize;i++)
{
	for(int j=0;j<ysize;j++)
	{
		grid[i][j]=-2;
	}
}

// obstacles[0].x=2;
// obstacles[0].y=0;
// grid[obstacles[0].x][obstacles[0].y]=1;

// obstacles[1].x=3;
// obstacles[1].y=0;
// grid[obstacles[1].x][obstacles[1].y]=1;

// obstacles[2].x=0;
// obstacles[2].y=4;
// grid[obstacles[2].x][obstacles[2].y]=1;

// obstacles[3].x=0;
// obstacles[3].y=3;
// grid[obstacles[3].x][obstacles[3].y]=1;

// obstacles[4].x=4;
// obstacles[4].y=4;
// grid[obstacles[4].x][obstacles[4].y]=1;
for(int i=0;i<no_obstacles;i++)
{
	obstacles[i].x=rand() % xsize;
	obstacles[i].y=rand() % ysize;
	grid[obstacles[i].x][obstacles[i].y]=1;

}

goal.x=xsize-1;
goal.y=ysize-1;

current.x=0;
current.y=0;

grid[goal.x][goal.y]= -1;
grid[current.x][current.y]=0;



printgrid();

double xsum;double ysum;
double final_direction;

while(current.x!=goal.x && current.y!=goal.y)
{
	printgrid();
	xsum=0;
	ysum=0;
	for(int i=0;i<no_obstacles;i++)
	{
		obs_info[i].mag=repulsive_magnitude(current,obstacles[i]);
		obs_info[i].direct=repulsive_direction(current,obstacles[i]);
		obs_info[i].y_mag=obs_info[i].mag*sin(obs_info[i].direct);
		ysum+=obs_info[i].y_mag;
		obs_info[i].x_mag=obs_info[i].mag*cos(obs_infoe[i].direct);
		xsum+=obs_info[i].x_mag;
	}
	cout<<"\n\nxsum(after repulsion): "<<xsum;
	cout<<"\n\nysum(after repulsion): "<<ysum;

	goal_info.mag=attractive_magnitude(current,goal);
	goal_info.direct=attractive_direction(current, goal);
	goal_info.y_mag=goal_info.mag*sin(goal_info.direct);
	ysum+=goal_info.y_mag;
	goal_info.x_mag=goal_info.mag*cos(goal_info.direct);
	xsum+=goal_info.x_mag;

	cout<<"\n\nxsum: "<<xsum;
	cout<<"\n\nysum: "<<ysum;

	final_direction=atan2(ysum,xsum);
	cout<<"\n\nfinal direction: "<<final_direction;
	// cout<<xsum<<" "<<ysum<<"\n";
	// cout<<final_direction<<"\n";
	final_direction+=3.14;

	if(final_direction<0.3926)
	{
		current.x=current.x+1;
	}
		
	else if(final_direction>=0.3926 && final_direction<1.1780)
	{
		current.x=current.x+1;
		current.y=current.y+1;
		cout<<"done";
	}
	else if(final_direction>=1.1780 && final_direction<1.9634)
	{
		
		current.y=current.y+1;
	}
	else if(final_direction>=1.9634 && final_direction<2.7488)
	{
		current.x=current.x-1;
		current.y=current.y+1;
	}
	else if(final_direction>=2.7488 && final_direction<3.5342)
	{
		current.x=current.x-1;
		
	}
	else if(final_direction>=3.5342 && final_direction<4.3196)
	{
		current.x=current.x-1;
		current.y=current.y-1;
	}
	else if(final_direction>=3.5342 && final_direction<4.3196)
	{
		current.x=current.x-1;
		current.y=current.y-1;
	}
	else if(final_direction>=4.3196 && final_direction<5.1050)
	{
		
		current.y=current.y-1;
	}
	else if(final_direction>=5.1050 && final_direction<5.890)
	{
		current.x=current.x-1;
		current.y=current.y-1;
	}
	else if(final_direction>=5.890)
	{
		current.x=current.x-1;
		current.y=current.y-1;
	}
grid[current.x][current.y]=0;
cout<<"\ncurrent x: "<<current.x;
cout<<"\ncurrent y: "<<current.y;
cout<<"\n";



}


return 0;

}
