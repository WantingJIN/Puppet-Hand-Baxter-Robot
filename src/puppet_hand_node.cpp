/**
 * \file
 * \brief 
 * \author 
 * \version 0.1
 * \date 
 * 
 * \param[in] 
 * 
 * Subscribes to: <BR>
 *    ° 
 * 
 * Publishes to: <BR>
 *    ° 
 *
 * Description
 *
 */


//Cpp
#include <sstream>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <sstream>
#include <stdio.h>
#include <vector>
#include <iostream>

//Namespaces
using namespace std;

//ROS
#include "ros/ros.h"

// Include here the ".h" files corresponding to the topic types you use.
#include <std_msgs/Char.h>
#include <std_msgs/Int16.h>
#include <std_msgs/Float64.h>
#include <sensor_msgs/JointState.h>
#include <baxter_core_msgs/JointCommand.h>
#include <ecn_common/token_handle.h>
#include <tf/transform_broadcaster.h>

int main (int argc, char** argv)
{

	//ROS Initialization
    ros::init(argc, argv, "puppet_hand_node");

    // Define your node handles
    ros::NodeHandle nh_("~");//ROS Handler - local namespace.
    ros::NodeHandle nh;
    
    tf::TransformBroadcaster br;
    tf::Transform transform;
    transform.setOrigin( tf::Vector3(0.0, 0.0, 0.3) );
    tf::Quaternion q;
    q.setRPY(0, M_PI, 0);
    transform.setRotation(q);
    ros::Rate rate(10);   // Or other rate.
	while (ros::ok()){
	 ros::spinOnce();
     //std::cout << "Br11 running" << std::endl ;
     br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "/left_hand", "/group11_rh_goal"));
     // Your node's code goes here.

     rate.sleep();
    }
}
