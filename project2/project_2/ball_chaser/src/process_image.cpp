#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>

// Define a global client that can request services
ros::ServiceClient client;

// This function calls the command_robot service to drive the robot in the specified direction
void drive_robot(float lin_x, float ang_z)
{
    // TODO: Request a service and pass the velocities to it to drive the robot
	ball_chaser::DriveToTarget service;
	service.request.linear_x=lin_x;
	service.request.angular_z=ang_z;
	 
	if (!client.call(service)) {
        ROS_ERROR("Failed to call service command_robot");
	}
}

// This callback function continuously executes and reads the image data
void process_image_callback(const sensor_msgs::Image img)
{

    int white_pixel = 255;

    // TODO: Loop through each pixel in the image and check if there's a bright white one
    // Then, identify if this pixel falls in the left, mid, or right side of the image
    // Depending on the white ball position, call the drive_bot function and pass velocities to it
    // Request a stop when there's no white ball seen by the camera
	

	bool is_ball_found = false;
	int rows = 0;
	int count;
	for(int i=0; i<img.height*img.step ; i+=3)	
		{
			if(img.data[i]==white_pixel && img.data[i+1]==white_pixel && img.data[i+2]==white_pixel)
			{
				is_ball_found = true;		     
				rows=(i/img.step);
				 count=i;
				break ;
			}
			
		}	
	std::cout << count << "," << rows <<std::endl;  
	int start= rows*img.step;
	int end= start + (img.step-1);
	int m= (end -start)/3;

	if (is_ball_found == false)
	{
	
		drive_robot(0,0.0);
	std::cout << "stop" <<std::endl;
	}
	else if(count>start && count<(start +m))
	{
		drive_robot(0.1,0.3);
		std::cout << "left" <<std::endl;
	}
	else if(count>(start+2*m) && count< end)
	{
		drive_robot(0.1,-0.3);
		std::cout << "right" <<std::endl;
	}
	else 
	{
		drive_robot(0.1,0);
		std::cout << "fwd" <<std::endl;
	}	
	
	//if(rows<(img.step/3) && is_ball_found == true)
	//		{ 		//turn left
	//			
	//		drive_robot(0,0.2);					
	//		}		
	//else if(rows > (img.step*2/3) && rows<=img.step)
	//		{	//turn right
					
	//		drive_robot(0,-0.2);
	//		}			
	//else
	//		{		//move forward
		
	//		drive_robot(0.5,0);
	//		}
			
	//if(is_ball_found == false)
	//	{drive_robot(0,0);}
		
		
	//drive_robot(lin_x,ang_z);
	
  // drive_robot(lin_x,ang_z);
}

int main(int argc, char** argv)
{
    // Initialize the process_image node and create a handle to it
    ros::init(argc, argv, "process_image");
    ros::NodeHandle n;

    // Define a client service capable of requesting services from command_robot
    client = n.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command_robot");

    // Subscribe to /camera/rgb/image_raw topic to read the image data inside the process_image_callback function
    ros::Subscriber sub1 = n.subscribe("/camera/rgb/image_raw", 1, process_image_callback);

    // Handle ROS communication events
    ros::spin();

    return 0;
}
