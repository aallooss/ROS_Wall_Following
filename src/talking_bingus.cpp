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
  msg.linear.x = .5;
  msg.angular.z = angles::from_degrees(22);
  pub.publish(msg);
  usleep(10);
}

void move_right(){
  std::cout << "...MOVING RIGHT...\n";
  msg.linear.x = .2;
  msg.angular.z = angles::from_degrees(-22);
  pub.publish(msg);
  usleep(10);
}

void move_forward(){
  std::cout << "...MOVING FORWARD...\n";
  msg.linear.x = .1;
  msg.angular.z = 0;
  pub.publish(msg);
  usleep(10);
}

void move_back(){
  std::cout << "...MOVING BACK...\n";
  msg.linear.x = -.1;
  msg.angular.z = 0;
  pub.publish(msg);
  usleep(10);
}

bool once = true;
void laser_callback(const sensor_msgs::LaserScan::ConstPtr& scan)
{
	if (scan->ranges[90]>12 && scan->ranges[270]>12){	// move forward if the sensor detect nothing in front of it
		move_forward();
		std::cout << "bruh1\n";
	}
	else if (scan->ranges[225]>12 && scan->ranges[315]<12){		// move right if sensor detects wall to the right of the robot
		move_forward();
		std::cout << "bruh2\n";
	}
	else if (scan->ranges[135]<12 && scan->ranges[45]<12){		// move left if sensor detects wall to the left of the robot
		std::cout << "bruh3\n";
		move_right();
	}
	else if (scan->ranges[315]<1 && scan->ranges[225]>.7){		// follow aside the right wall
		std::cout << "bruh4\n";
		move_left();
	}
	else{
		if (scan->ranges[270]>.5){
			std::cout << "bruh5\n";
			move_right();
		}
		else if (scan->ranges[270]<1.5){
			std::cout << "bruh6\n";
			move_left();
		}
		else{
			move_forward();
			std::cout << "bruh0\n";
		}
	}
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "talking_bingus");
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