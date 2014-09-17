/*to do:-
1) reset potential of all the points for every frame
2) make sure you check that the potential for the obstacle is DBL_MAX

*/

#include <iostream>									
#include <stdio.h>		
#include <float.h>		
#include <math.h>
#include <stdlib.h>

using namespace std;

#define sizex 50
#define sizey 50
#define no_of_obstacles 6
#define polar_map_region 1 //i.e. 1 squares from the current location of acyut
#define PI 3.14159265
#define k_repulsive 5
#define k_attractive 5
#define r_rho0 3

int grid[sizex][sizey];

struct Point
{
	int x;
	int y;
};

struct Point current,goal,obstacle[no_of_obstacles];

double radtodeg(double a)
{
	double b;
	 b=(a*180)/PI;
	 if(b>0)
	 {
	 	return b;
	 }
	 else
	 {
	 	return b+360;
	 }
}





double attractive_potential(int x,int y, struct Point b) // a the the current struct passed and b is the goal position
{
	
	if(x<0 || x>sizex || y<0 || y>sizey)
	{
		return DBL_MAX;
	}

	if(x==b.x && y==b.y)
	{
		return -DBL_MAX;
	}
	else
	{
		return -0.5*k_attractive*1/(pow(x-b.x,2)+pow(y-b.y,2));
	}
}


double repulsive_potential(int x,int y, struct Point b) // a the the current struct passed and b is the goal position
{
	
	if(x<0 || x>sizex || y<0 || y>sizey)
	{
		return DBL_MAX;
	}
	if(grid[x][y]==0)
		return DBL_MAX;
	double rho;
	rho=sqrt(pow((x-b.x),2)+pow((y-b.y),2));
	if(rho>r_rho0)
	{
		return 0;
	}
	else
	{
		//cout<<"\nrepulsive potential "<<0.5*k_repulsive*pow((1/rho-1/r_rho0),2);
		return 0.5*k_repulsive*pow((1/rho-1/r_rho0),2);
	}
}

void printgrid()
{
	//grid[2][7]=5;
	cout<<"\n\n\n\n\n\n";
	for(int y=0;y<sizey;y++)
	{
		for(int x=0;x<sizex;x++)
		{
			if(grid[x][y]==-1)
				cout<<"*"<<" "; //representing goal position

			else if(grid[x][y]==-2)
				cout<<"-"<<" "; //representing normal cells
			
			else 
				
				cout<<grid[x][y]<<" "; //whatever number is assigned; like 1 for an obstacle and 0 for the path traced
					
		}
		cout<<"\n";
	}
}

struct Surrounding_points
	{
		bool flag;
		double potential;
	};

struct Surrounding_points spoints[8];

void resetspoints_flag()
{
	for(int i=0;i<8;i++)
	{
		spoints[i].flag=false;
	}
}


void resetspoints_potential()
{
	for(int i=0;i<8;i++)
	{
		spoints[i].potential=0;
	}
}


int main()
{
	srand(time(NULL)); //seeding for random numbers

	goal.x=49;
	goal.y=49;

	current.x=0;
	current.y=0;


	double angle,dist;

	for (int y=0;y<sizey;y++)        //assigining normal values to the cells
	{
		for(int x=0;x<sizex;x++)
		{
			grid[x][y]=-2;

		}
	}

	// for(int i=0;i<no_of_obstacles;i++)    //assigining obstacles values to the cells
	// {
	// 	obstacle[i].x=rand() % sizex;
	// 	obstacle[i].y=rand() % sizey;
	// 	grid[obstacle[i].x][obstacle[i].y]=1;		
	// }

	obstacle[1].x=25;
	obstacle[1].y=25;
	grid[obstacle[1].x][obstacle[1].y]=1;	

	obstacle[2].x=26;
	obstacle[2].y=24;
	grid[obstacle[2].x][obstacle[2].y]=1;	

	obstacle[3].x=27;
	obstacle[3].y=23;
	grid[obstacle[3].x][obstacle[3].y]=1;

	obstacle[4].x=24;
	obstacle[4].y=26;
	grid[obstacle[4].x][obstacle[4].y]=1;		

	obstacle[5].x=23;
	obstacle[5].y=27;
	grid[obstacle[5].x][obstacle[5].y]=1;	

	obstacle[0].x=12;
	obstacle[0].y=12;
	grid[obstacle[0].x][obstacle[0].y]=1;	

	grid[goal.x][goal.y]=-1;		//assigining goal values to the cells
	
	grid[current.x][current.y]=0; 	//assigining current start position to the cell


	while(current.x!=goal.x || current.y!=goal.y)
	{
		resetspoints_flag();
		for(int i=0;i<no_of_obstacles;i++)
		{
			dist=sqrt(pow(current.x - obstacle[i].x,2)+pow(current.y - obstacle[i].y,2));
			cout<<"\ndist "<<dist;
			if(dist < 5)
			{
				cout<<"\nEntered sector checking";
				angle=radtodeg(atan2((obstacle[i].y - current.y),(obstacle[i].x - current.x))); 
				cout<<"\nangle:- "<<angle;
				
				if (angle>0 && angle <=22.5)
				{
					spoints[0].flag=true;
				}
				if (angle>22.5 && angle <=67.5)
				{
					spoints[1].flag=true;
				}
				if (angle>67.5 && angle <=112.5)
				{
					spoints[2].flag=true;
				}
				if (angle>112.5 && angle <=157.5)
				{
					spoints[3].flag=true;
				}
				if (angle>157.5 && angle <=205.5)
				{
					spoints[4].flag=true;
				}
				if (angle>202.5 && angle <=247.5)
				{
					spoints[5].flag=true;
				}
				if (angle>247.5 && angle <=292.5)
				{
					spoints[6].flag=true;
				}
				if (angle>292.5 && angle <=337.5)
				{
					spoints[7].flag=true;
				}
				if (angle>337.5 && angle <=0)
				{
					spoints[8].flag=true;
				}

			}
		}

			
		resetspoints_potential();

		// for(int i=current.y - 1; i<=current.y+1;i++)
		// {
		// 	for(int j=current.x-1;j<=current.x+1;j++)
		// 	{
		// 		if( i!=current.y && j!=current.x)
		// 			{
		// 				for(int k=0;k<8;k++)
		// 				{
		// 					for(int l=0;l<no_of_obstacles;l++)
		// 					{
		// 						spoints[k].potential+=repulsive_potential(j,i,obstacle[l]);
		// 					}
		// 					cout<<"\nj:- "<<j;
		// 					spoints[k].potential+=attractive_potential(j,i,goal);
		// 				}
		// 			}
		// 	}
		// }

		for(int i=0;i<no_of_obstacles;i++)
		{
			spoints[0].potential+=repulsive_potential(current.x+1,current.y,obstacle[i]);
			spoints[1].potential+=repulsive_potential(current.x+1,current.y+1,obstacle[i]);
			spoints[2].potential+=repulsive_potential(current.x,current.y+1,obstacle[i]);
			spoints[3].potential+=repulsive_potential(current.x-1,current.y+1,obstacle[i]);
			spoints[4].potential+=repulsive_potential(current.x-1,current.y,obstacle[i]);
			spoints[5].potential+=repulsive_potential(current.x-1,current.y-1,obstacle[i]);
			spoints[6].potential+=repulsive_potential(current.x,current.y-1,obstacle[i]);
			spoints[7].potential+=repulsive_potential(current.x+1,current.y-1,obstacle[i]);

		}

			spoints[0].potential+=attractive_potential(current.x+1,current.y,goal);
			spoints[1].potential+=attractive_potential(current.x+1,current.y+1,goal);
			spoints[2].potential+=attractive_potential(current.x,current.y+1,goal);
			spoints[3].potential+=attractive_potential(current.x-1,current.y+1,goal);
			spoints[4].potential+=attractive_potential(current.x-1,current.y,goal);
			spoints[5].potential+=attractive_potential(current.x-1,current.y-1,goal);
			spoints[6].potential+=attractive_potential(current.x,current.y-1,goal);
			spoints[7].potential+=attractive_potential(current.x+1,current.y-1,goal);




		//printing
		for(int i=0;i<8;i++)
		{
			cout<<"\n spoints.potential for"<<i<<spoints[i].potential;
			cout<<"\n spoints.flag for"<<i<<spoints[i].flag;
		}



		double min=DBL_MAX;
		int z;
		for(int i=0;i<8;i++)
		{
			if(spoints[i].flag==false)
			{
				if(min>spoints[i].potential)
				{
					min=spoints[i].potential;
					z=i;
				}
			}
		} 
		cout<<"\nz:- "<<z;

		switch(z)
		{
			case 0:
			{
				current.x=current.x+1;
				break;
			}

			case 1:
			{
				current.x=current.x+1;
				current.y=current.y+1;
				break;
			}

			case 2:
			{
				current.y=current.y+1;
				break;
			}

			case 3:
			{
				current.x=current.x-1;
				current.y=current.y+1;
				break;
			}

			case 4:
			{
				current.x=current.x-1;
				break;
			}

			case 5:
			{
				current.x=current.x-1;
				current.y=current.y-1;
				break;
			}

			case 6:
			{
				current.y=current.y-1;
				break;
			}

			case 7:
			{
				current.x=current.x+1;
				current.y=current.y-1;
				break;
			}

		}

		grid[current.x][current.y]=0;
		printgrid();
		getchar();
		
	}

	return 0;
}


