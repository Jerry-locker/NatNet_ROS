#include <iostream>
#include <ros/ros.h>
#include <rosbag/bag.h>
#include <rosbag/view.h>
#include <boost/foreach.hpp>

#include <vector>
#include <string>

#include <tf2_msgs/TFMessage.h>
#include <sensor_msgs/Imu.h>

#include <fstream>
#include <iomanip>

// rosbag::View myview;
std::fstream myFile;

void RunBag()
{
    // std::string bag_path = "/home/jerry/Dataset/opti/2023-02-28-18-02-05.bag";  //1
    // std::string bag_path = "/home/jerry/Dataset/opti/2023-02-28-18-02-57.bag";  //2
    std::string bag_path = "/home/jerry/Dataset/opti/2023-02-28-18-03-26.bag";  //3
    double bag_start = 0;
    double bag_durr = -1;
    
    std::vector<std::string> topics;
    topics.push_back("/tf");

    rosbag::Bag bag;
    bag.open(bag_path, rosbag::bagmode::Read);
    
    rosbag::View view_full;
    view_full.addQuery(bag);
    
    //起始时间
    ros::Time time_start = view_full.getBeginTime();
    time_start += ros::Duration(bag_start);
    //终止时间
    ros::Time time_finish = (bag_durr < 0) ? view_full.getEndTime() : time_start + ros::Duration(bag_durr);
    
    rosbag::View view;
    view.addQuery(bag, rosbag::TopicQuery(topics), time_start, time_finish);
    std::cout << "[/tf size] " << view.size() << std::endl;
    if(view.size() == 0) 
    {
        ROS_ERROR("No messages to play on specified topics.  Exiting.");
        ros::shutdown();
        return;
    }

    for(const rosbag::MessageInstance& m : view)
    {
        if (m.getTopic() == "/tf")
        {
            tf2_msgs::TFMessage::ConstPtr tf_msg = m.instantiate<tf2_msgs::TFMessage>();
            // std::cout << tf_msg->transforms.size() << std::endl;
            double time = tf_msg->transforms[0].header.stamp.toSec();
            double x = tf_msg->transforms[0].transform.translation.x;
            double y = tf_msg->transforms[0].transform.translation.y;
            double z = tf_msg->transforms[0].transform.translation.z;
            double qx = tf_msg->transforms[0].transform.rotation.x;
            double qy = tf_msg->transforms[0].transform.rotation.y;
            double qz = tf_msg->transforms[0].transform.rotation.z;
            double qw = tf_msg->transforms[0].transform.rotation.w;

            myFile << time << " " << x << " " << y << " " << z << " " << qx << " " << qy << " " << qz << " " << qw << std::endl;
        }
    }
}

int main(int argc, char** argv) 
{
    ros::init(argc, argv, "optitrack_node");
    ros::NodeHandle nh("~");

    // myFile.open("/home/jerry/Dataset/opti/opt-pose1.txt", std::ios_base::out | std::ios_base::app);
    // myFile.open("/home/jerry/Dataset/opti/opt-pose2.txt", std::ios_base::out | std::ios_base::app);
    myFile.open("/home/jerry/Dataset/opti/opt-pose3.txt", std::ios_base::out | std::ios_base::app);
    myFile << std::setprecision(9) << std::fixed;

    RunBag();
    myFile.close();
    
    return 0;
}