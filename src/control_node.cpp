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
	
	// Include here the ".h" files corresponding to the topic types you use.
	#include "ros/ros.h"
	#include <baxter_core_msgs/JointCommand.h>
	#include <baxter_core_msgs/SolvePositionIK.h>
	#include <ecn_common/token_handle.h>
	#include <tf/transform_listener.h>
	
	
	
	int main (int argc, char** argv)
	{
		
	
		//ROS Initialization
	    ros::init(argc, argv, "control_node");
	    ecn::TokenHandle token("group 11");
	    // Define your node handles
	    ros::NodeHandle nh_("~");//ROS Handler - local namespace.
	    ros::NodeHandle nh;
	  //  tf::Transform transform;
	    ros::Publisher pub_command ;//The publisher of the command to move the joint
	    
	    ros::ServiceClient control_node = 
	    nh.serviceClient<baxter_core_msgs::SolvePositionIK>("/ExternalTools/right/PositionKinematicsNode/IKService");
	   
	    
	    
	    tf::TransformListener listener;
	    
	    geometry_msgs::PoseStamped pos;
	    
	    pub_command = nh_.advertise<baxter_core_msgs::JointCommand>("/robot/limb/right/joint_command", 1);
	    
	    ros::Rate rate(10);   // Or other rate.
	    
		while (ros::ok()){
			ros::spinOnce();
			tf::StampedTransform transform;
			token.update();
			
			try{
				listener.lookupTransform("/base", "/group11_rh_goal",  
										 ros::Time(0), transform);
			     // Create an geometry_msgs::PoseStamped object.
				 // fill its position and its orientation fields using "transform"
				 // then push the object into srvMessage.request.pose_stamp
					          							
			}catch (tf::TransformException ex){
				ROS_ERROR("%s",ex.what());
				ros::Duration(1.0).sleep();
			}
			if(!control_node.exists()) {
				ROS_INFO("Service does not exist.");
				continue ;
			}
			pos.header.stamp=ros::Time::now();
			pos.header.frame_id="/base";
			pos.pose.position.x=transform.getOrigin().x();
			pos.pose.position.y=transform.getOrigin().y();
			pos.pose.position.z=transform.getOrigin().z();
			pos.pose.orientation.x=transform.getRotation().getX();
			pos.pose.orientation.y=transform.getRotation().getY();
			pos.pose.orientation.z=transform.getRotation().getZ();
			pos.pose.orientation.w=transform.getRotation().getW();
			baxter_core_msgs::SolvePositionIK srvMessage ;
			srvMessage.request.seed_mode = srvMessage.request.SEED_CURRENT ;
			srvMessage.request.pose_stamp.push_back(pos); 
			
			if(! control_node.call(srvMessage)){
				 ROS_ERROR("Call to service failed");
				 continue ;
			 }
			
			baxter_core_msgs::JointCommand msg_command;
			msg_command.mode = msg_command.POSITION_MODE; 
			if(!srvMessage.response.isValid[0]){
				ROS_INFO("There is no inverse solution.");
			    continue ;
			}
				
			
	   
			for( int i=0 ; i < srvMessage.response.joints[0].name.size(); i++ )
			{
				msg_command.names.push_back(srvMessage.response.joints[0].name[i]);
				msg_command.command.push_back(srvMessage.response.joints[0].position[i]);//Increment the joint value
			
			}
			pub_command.publish(msg_command);
	
	     
	    
		}
		rate.sleep();

	}
	
