#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>
#include "ball_chaser/DriveToTarget.h"

// Define a global client that can request services
ros::ServiceClient client;

// This function calls the command_robot service to drive the robot in the specified direction
void drive_robot(float lin_x, float ang_z)
{
    ROS_INFO_STREAM("passing velocity values");
    ball_chaser::DriveToTarget srv;
    srv.request.linear_x = lin_x;
    srv.request.angular_z= ang_z;
 
    // Call the safe_move service and pass the requested velocity commands
    if (!client.call(srv))
        ROS_ERROR("Failed to call service command_robot");
}

// This callback function continuously executes and reads the image data
void process_image_callback(const sensor_msgs::Image img)
{
    
    int white_pixel = 255;

   int pixels = img.height * img.step;
  // ROS_INFO_STREAM(pixels);
    // drive_robot(0.2,0.0);
   

  
 int flag=0;
 int i;
 int s=0;
 
  for (i = 0; i < pixels; i+=3){
        if(img.data[i]==white_pixel && img.data[i+1]==white_pixel && img.data[i+2]==white_pixel)
           {
            ROS_INFO_STREAM("Ball detected");          
            flag =1;
            s = i % img.step;
            break;
          }
 }
if(flag==1)
{
  if( s <= img.step * 0.3 && s>=0) drive_robot(0.0,2.0);
  else if(s > img.step*0.7 && s<=img.step) drive_robot(0.0,-2.0);
  else drive_robot(1.0,0.0);
}
else
  drive_robot(0.0,0.0);



 ros::Duration(0.1).sleep();


    // TODO: Loop through each pixel in the image and check if there's a bright white one
    // Then, identify if this pixel falls in the left, mid, or right side of the image
    // Depending on the white ball position, call the drive_bot function and pass velocities to it
    // Request a stop when there's no white ball seen by the camera
}

int main(int argc, char** argv)
{
    // Initialize the process_image node and create a handle to it
    ros::init(argc, argv, "process_image");
    ros::NodeHandle n;

    // Define a client service capable of requesting services from command_robot
    client = n.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command_robot");

    // Subscribe to /camera/rgb/image_raw topic to read the image data inside the process_image_callback function
    ros::Subscriber sub1 = n.subscribe("/camera/rgb/image_raw", 10, process_image_callback);

    // Handle ROS communication events
    ros::spin();

    return 0;
}
