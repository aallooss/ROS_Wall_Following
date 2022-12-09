#include "ros/ros.h"
#include <algorithm>
#include <cmath>
#include "angles/angles.h"
#include "ros/duration.h"
#include "geometry_msgs/Twist.h"
#include "sensor_msgs/LaserScan.h"
#include "tf/transform_listener.h"
#include <iostream>

ros::Publisher pub;
ros::Subscriber sub;
geometry_msgs::Twist msg;

void move_left(){
  std::cout << "...MOVING LEFT...\n";
  msg.linear.x = .25;
  msg.angular.z = angles::from_degrees(22);
  pub.publish(msg);
  usleep(10);
}

void move_right(){
  std::cout << "...MOVING RIGHT...\n";
  msg.linear.x = .25;
  msg.angular.z = angles::from_degrees(-22);
  pub.publish(msg);
  usleep(10);
}


void desired_path()
{
    float f = 1;        // when robot following desired path f(x,y) = 1 therefore we will intialize as such 
    float theta = 22.5;
    float u1t = 0.3;    // obtained from the paper's pretuned figure 2
    float fx = u1t * std::cos(theta);
    float fy = u1t * std::sin(theta);
    f = std::sqrt(fx*fx + fy*fy);       // > 0

    float K1 = 15;      // K1 > 0
    float K2 = 2;       // 0 > K2 <= 4

    float sigmoid = ((K2 * f) / sqrt(1 + f*f));

    float r = K1 * (-1 * u * sigmoid - fx*u*std::cos(theta) - fy*u*std::sin(theta));


    msg.linear.x = r;
    msg.angular.z = .01;
    pub.publish(msg);
    usleep(10);
}

// float obstacle_avoidance(xj, yj)
// {   
//     float xj = NULL;    // x j , y j represent the position of Obstacle
//     float yj = NULL;    
//     int Aj = 1;         // Aj is the height of the Gaussian curve
//     int sigma = 1;      // represents the standard deviation

//     float Oj = Aj * exp( (x + xj)**2  +  (y + yj)**2 / sigma**2);
    
//     return Oj;
// }

void path_equation()
{
    float velocity =  1;
}

bool once = true;
void laser_callback(const sensor_msgs::LaserScan::ConstPtr& scan)
{
	if (scan->ranges[135]>12 && scan->ranges[225]>12){	// measuring points on the front left section
		desired_path();
		std::cout << "bruh1\n";
	}
	// else if (scan->ranges[225]>12 && scan->ranges[315]<12){		// move right if sensor detects wall to the right of the robot
		
	// 	std::cout << "bruh2\n";
	// }
	// else if (scan->ranges[135]<12 && scan->ranges[45]<12){		// move left if sensor detects wall to the left of the robot
	// 	std::cout << "bruh3\n";
	// 	move_right();
	// }
	// else if (scan->ranges[315]<1 && scan->ranges[225]>.7){		// follow aside the right wall
	// 	std::cout << "bruh4\n";
	// 	move_left();
	// }
	// else{
	// 	if (scan->ranges[270]>.5){
	// 		std::cout << "bruh5\n";
	// 		move_right();
	// 	}
	// 	else if (scan->ranges[270]<.75){
	// 		std::cout << "bruh6\n";
	// 		move_left();
	// 	}
	// 	else{
			
	// 		std::cout << "bruh0\n";
	// 	}
	// }
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "gaussian_talker");
	ros::NodeHandle n;

	pub = n.advertise<geometry_msgs::Twist>("cmd_vel", 1000);
	sub = n.subscribe("/scan", 1000, laser_callback);

	ros::Duration time_between_ros_wakeups(0.001);

	while (ros::ok())
	{
		ros::spinOnce();
		time_between_ros_wakeups.sleep();
	}
	return 0;
}