/* initial_obstacles == no of obstacles defined
   max_obstacles==maximum no. of obstacles posssible i.e. no of obstaclesC2
   temp=new obstacles added
   final_obstacles=initial obstacles + temp
 */
#include <iostream>									
#include <stdio.h>		
#include <float.h>		
#include <math.h>
#include <stdlib.h>
#include <opencv2/opencv.hpp>

using namespace std;

#define sizex 900
#define sizey 600
#define initial_obstacles 4
//#define no_of_obstacles 6
#define PI 3.14159265
#define k_repulsive 5
#define k_attractive 10
#define r_rho0 30
#define new_r_rho0 30
#define resolution 50

struct Point
{
	double x;
	double y;
};

struct O_point
{
	double x;
	double y;
	bool flag;
};

#define max_obstacles initial_obstacles*(initial_obstacles - 1)/2
int final_obstacles=initial_obstacles;
struct Point current,goal,start;
struct O_point obstacle[max_obstacles];

inline double radtodeg(double a)
{
	double b;
	 b=(a*180.0)/PI;
	 if(b>0)
	 {
	 	return b;
	 }
	 else
	 {
	 	return b+360.0;
	 }
}

#define deg2rad(x) (((x)*PI)/180.0)

struct Spokes
{
	Point p;
	bool flag;
	double potential;
};

struct Spokes spoints[resolution];

double attractive_potential(struct Spokes s, struct Point b) // a the the current struct passed and b is the goal position
{
	
	if(s.p.x<0 || s.p.x>sizex || s.p.y<0 || s.p.y>sizey)
	{
		return DBL_MAX;
	}

	if(s.p.x==b.x && s.p.y==b.y)
	{
		return -DBL_MAX;
	}
	else
	{
		return -0.5*k_attractive*1/(pow(s.p.x-b.x,2)+pow(s.p.y-b.y,2));
		
	}
}

double repulsive_potential(struct Spokes s, struct O_point b) 
{
	if(s.p.x<0 || s.p.x>sizex || s.p.y<0 || s.p.y>sizey)
	{
		return DBL_MAX;
	}
	double rho;
	rho=sqrt(pow((s.p.x-b.x),2)+pow((s.p.y-b.y),2));
	if(b.flag==true)
	{
		if(rho>new_r_rho0)
		{
			return 0;
		}
		else
		{
			return 0.5*k_repulsive*pow((1/rho-1/r_rho0),2);
		}
	}
	else
	{
		if(rho>r_rho0)
		{
			return 0;
		}
		else
		{
		return 0.5*k_repulsive*pow((1/rho-1/r_rho0),2);
		}
	}
}

void resetspoints_flag()
{
	for(int i=0;i<resolution;i++)
	{
		spoints[i].flag=false;
	}
}

void resetspoints_potential()
{
	for(int i=0;i<resolution;i++)
	{
		spoints[i].potential=0.0;
	}
}

int main()
{
	srand(time(NULL)); //seeding for random numbers
	int z;

	IplImage* image = cvCreateImage(cvSize(1040, 740), 8, 3);
	cvZero(image);
	//the field has a size of exactly according to the rulebook. 900+70+70-> x-axis, 600+70+70->y-axis,
	cvRectangle(image,cvPoint(0,0),cvPoint(1040,740),cvScalar(50,200,50),-1); //green rectangle
	cvRectangle(image,cvPoint(70,70),cvPoint(970,670),cvScalar(255,255,255),5); //whit outer rectangle
	cvRectangle(image,cvPoint(10,240),cvPoint(70,500),cvScalar(0,255,255),5); //yellow left goalpost
	cvRectangle(image,cvPoint(970,240),cvPoint(1030,500),cvScalar(0,255,255),5); //yellow right goalpost
	cvRectangle(image,cvPoint(70,145),cvPoint(170,595),cvScalar(255,255,255),5); //left D
	cvRectangle(image,cvPoint(870,145),cvPoint(970,595),cvScalar(255,255,255),5); //right D
	cvLine(image,cvPoint(520,70),cvPoint(520,670),cvScalar(255,255,255),5); //mid line
	cvCircle(image,cvPoint(520,370),75,cvScalar(255,255,255),5); //mid circle

	goal.x=70+(rand()%901);
	goal.y=70+(rand()%601);
	cout<<"ball "<<goal.x<<" "<<goal.y;
	cvCircle(image,cvPoint(goal.x,goal.y),5,cvScalar(0,0,255)); //to reach position

	current.x=70+(rand()%901);
	current.y=70+(rand()%601);
	start=current;
	cout<<"current "<<current.x<<" "<<current.y;
	cvCircle(image,cvPoint(start.x,start.y),5,cvScalar(255,0,0)); //start position

	

	double angle,dist;

	for(int i=0;i<initial_obstacles;i++)    //assigining obstacles values to the cells
	{
		if(current.x<goal.x)
			obstacle[i].x=(int)current.x+rand() % int(fabs(goal.x-current.x+1));
		else
			obstacle[i].x=(int)goal.x+rand() % int(fabs(goal.x-current.x+1));
		if(current.y<goal.y)
			obstacle[i].y=(int)current.y+rand() % int(fabs(goal.y-current.y+1));
		else
			obstacle[i].y=(int)goal.y+rand() % int(fabs(goal.y-current.y+1));
		obstacle[i].flag=false;	
		//cout<<"obstacle "<<i<<": "<<obstacle[i];
		cvCircle(image, cvPoint(obstacle[i].x, obstacle[i].y), 10, cvScalar(0,0,0));

	}
	// obstacle[0].x=sizex/2;
	// obstacle[0].y=sizey/2;
	// cvCircle(image, cvPoint(obstacle[0].x, obstacle[0].y), 2, cvScalar(255,0,0));

	double obs_dist;
	int temp=initial_obstacles;
	for(int i=0;i<initial_obstacles;i++)
	{
		for(int j=i+1;j<initial_obstacles;j++)
		{
			obs_dist=sqrt(pow((obstacle[i].x - obstacle[j].x),2)+pow((obstacle[i].y - obstacle[j].y),2));
			if(obs_dist > 0.5*r_rho0 && obs_dist < 3*r_rho0)
			{	
				cout<<"aa gaya........";
				obstacle[temp].flag=true;
				obstacle[temp].x=(obstacle[i].x+obstacle[j].x)/2;
				obstacle[temp].y=(obstacle[i].y+obstacle[j].y)/2;
				cvCircle(image, cvPoint(obstacle[temp].x, obstacle[temp].y), 10, cvScalar(0,0,0));
				temp++;
				final_obstacles++;

			}
		}
	}

	// obstacle[0].x=100;
	// obstacle[0].y=100;
	// cvCircle(image, cvPoint(obstacle[0].x, obstacle[0].y), 2, cvScalar(255,0,0));

	double angle_center,angle_spoint;

	while((pow((current.y-goal.y),2)+pow((current.x-goal.x),2))>25) 
	{
		resetspoints_flag();
		for(int i=0;i<resolution;i++)
		{
			spoints[i].p.x=current.x+cos(deg2rad((360.0/resolution)*i));
			spoints[i].p.y=current.y+sin(deg2rad((360.0/resolution)*i));
		}

		for(int i=0;i<final_obstacles;i++)
		{
			dist=sqrt(pow(current.x - obstacle[i].x,2)+pow(current.y - obstacle[i].y,2));
			if(dist<new_r_rho0+10)
			{
				cout<<"\nenter distance less than r_rho0";
				angle_center=radtodeg(atan2((obstacle[i].y - current.y),(obstacle[i].x - current.x)));
				cout<<"Center angle: "<<angle_center<<endl;
				for(int j=0;j<resolution;j++)
				{	
					angle_spoint=radtodeg(atan2((spoints[j].p.y - current.y),(spoints[j].p.x - current.x)));
					if(fabs(angle_spoint - angle_center)<resolution)
					{
						spoints[j].flag=true;
						cout<<fabs(angle_spoint)<<" ";
					}
				}	
			}
		}

		resetspoints_potential();
		if(z+ resolution/2<resolution)
		{
			spoints[z+(resolution/2)].flag=true;
		}
		else
			spoints[z-(resolution/2)].flag=true;

		for(int i=0;i<resolution;i++)
		{
			for(int j=0;j<final_obstacles;j++)
			{
				spoints[i].potential+=repulsive_potential(spoints[i],obstacle[j]);				
			}
			spoints[i].potential+=attractive_potential(spoints[i],goal);
		}


		double min=DBL_MAX;
		
		for(int i=0;i<resolution;i++)
		{
			cout<<"\nspoints i= "<<i<<"potential "<<spoints[i].potential;
			if(spoints[i].flag==false)
			{
				if(min>spoints[i].potential)
				{
					min=spoints[i].potential;
					z=i;
				}

			}
		} 
		cout<<"\nz "<<z;
		current.x=spoints[z].p.x;
		current.y=spoints[z].p.y;
		cvCircle(image, cvPoint(current.x, current.y), 2, cvScalar(0,0,255));
		cvShowImage("Field", image);
		cvWaitKey(1);

		
		// if((cvWaitKey()&0xff)==27)
		// 	break;
	}
	// cvShowImage("Field", image);
	 cvWaitKey(0);
	 cvSaveImage("FIeld.bmp",image);

	return 0;
}


