//
// Created by abhinavroy on 4/7/20.
//

#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include "sensor_msgs/Image.h"

ros::ServiceClient client;

void drive_robot(float linear_x, float angular_z)
{
    ROS_INFO("Driving Robot linear_x:%1.2f, angular_z:%1.2f", linear_x, angular_z);

    ball_chaser::DriveToTarget srv;
    srv.request.linear_x = linear_x;
    srv.request.angular_z = angular_z;

    if (!client.call(srv))
        ROS_ERROR("Failed to call service command_robot");
}

void process_image_callback(const sensor_msgs::Image img)
{
    int white_pixel = 255;
    int width = img.width;
    int step = img.step;
    int height = img.height;
    int ball_column = 0;
    int colors = step/width;
    bool found = false;

    for (ball_column=0; ball_column <= step * height; ball_column+=colors ){
        if(img.data[ball_column]==white_pixel &&
	   img.data[ball_column+1]==white_pixel &&
	   img.data[ball_column+2]==white_pixel){
            found = true;
            ball_column = ball_column % step;
            break;
        }
    }

    if (found == true){
        if(ball_column < step*1/3){
            drive_robot(0.0,0.5);
        }
        else if(ball_column < step*2/3){
            drive_robot(0.3,0.0);
        } else{
            drive_robot(0.0,-0.5);
        }
    } else {
        drive_robot(0.0,0.0);
    }
}

int main(int argc,char** argv)
{
    ros::init(argc, argv, "process_image");

    ros::NodeHandle n;

    client = n.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command_robot");
    ros::Subscriber sub1 = n.subscribe("/camera/rgb/image_raw", 10, process_image_callback);

    ros::spin();

    return 0;
}
