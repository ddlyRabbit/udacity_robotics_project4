//
// Created by abhinavroy on 4/7/20.
//

#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include "geometry_msgs/Twist.h"

ros::Publisher twist_pub;

bool handle_drive_request(ball_chaser::DriveToTarget::Request& req,
        ball_chaser::DriveToTarget::Response& res)
{
    ROS_INFO("DriveToTarget request received - linear_x:%1.2f, angular_z:%1.2f", req.linear_x, req.angular_z);

    geometry_msgs::Twist twist_msg;
    twist_msg.linear.x = req.linear_x;
    twist_msg.angular.z = req.angular_z;
    twist_pub.publish(twist_msg);

    res.msg_feedback = ("Twist set - linear_x:%1.2f angular_z:%1.2f",twist_msg.linear.x,twist_msg.angular.z);
    ROS_INFO_STREAM(res.msg_feedback);

    return true;
}

int main(int argc,char** argv)
{
    ros::init(argc, argv, "drive_bot");

    ros::NodeHandle n;

    twist_pub = n.advertise<geometry_msgs::Twist>("/cmd_vel", 10);
    ros::ServiceServer service = n.advertiseService("/ball_chaser/command_robot", handle_drive_request);

    ros::spin();

    return 0;
}
