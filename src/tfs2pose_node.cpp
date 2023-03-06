#include <iostream>
#include <ros/ros.h>
#include <rosbag/bag.h>
#include <rosbag/view.h>
#include <boost/foreach.hpp>

#include <vector>
#include <string>

#include <tf2_msgs/TFMessage.h>

#include <sensor_msgs/Imu.h>
#include <geometry_msgs/TransformStamped.h>

#include <fstream>
#include <iomanip>

std::fstream myFile;
std::string bag_path = "/home/jerry/Dataset/opti/0301/edit/2023-03-01-21-17-18-edit.bag"; 
std::string output_file_path = "/home/jerry/gt.txt";

void RunBag()
{
    double bag_start = 0;
    double bag_durr = -1;
    
    std::vector<std::string> topics;
    // topics.push_back("/tf");
    topics.push_back("/marker_pose_in_optitrack");

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
    // std::cout << "[/tf size] " << view.size() << std::endl;
    std::cout << "[/marker_pose_in_optitrack size] " << view.size() << std::endl;
    if(view.size() == 0) 
    {
        ROS_ERROR("No messages to play on specified topics.  Exiting.");
        ros::shutdown();
        return;
    }

    for(const rosbag::MessageInstance& m : view)
    {
        // if (m.getTopic() == "/tf")
        // {
        //     tf2_msgs::TFMessage::ConstPtr tf_msg = m.instantiate<tf2_msgs::TFMessage>();
        //     // std::cout << tf_msg->transforms.size() << std::endl;
        //     double time = tf_msg->transforms[0].header.stamp.toSec();
        //     double x = tf_msg->transforms[0].transform.translation.x;
        //     double y = tf_msg->transforms[0].transform.translation.y;
        //     double z = tf_msg->transforms[0].transform.translation.z;
        //     double qx = tf_msg->transforms[0].transform.rotation.x;
        //     double qy = tf_msg->transforms[0].transform.rotation.y;
        //     double qz = tf_msg->transforms[0].transform.rotation.z;
        //     double qw = tf_msg->transforms[0].transform.rotation.w;

        //     myFile << time << " " << x << " " << y << " " << z << " " << qx << " " << qy << " " << qz << " " << qw << std::endl;
        // }
        if (m.getTopic() == "/marker_pose_in_optitrack")
        {
            geometry_msgs::TransformStamped::ConstPtr tfs_msg = m.instantiate<geometry_msgs::TransformStamped>();
            double time = tfs_msg->header.stamp.toSec();
            double x = tfs_msg->transform.translation.x;
            double y = tfs_msg->transform.translation.y;
            double z = tfs_msg->transform.translation.z;
            double qx = tfs_msg->transform.rotation.x;
            double qy = tfs_msg->transform.rotation.y;
            double qz = tfs_msg->transform.rotation.z;
            double qw = tfs_msg->transform.rotation.w;

            myFile << time << " " << x << " " << y << " " << z << " " << qx << " " << qy << " " << qz << " " << qw << std::endl;
        }
    }
}

int main(int argc, char** argv) 
{
    ros::init(argc, argv, "optitrack_node");
    ros::NodeHandle nh("~");

    myFile.open(output_file_path, std::ios_base::out | std::ios_base::app);
    myFile << std::setprecision(9) << std::fixed;

    RunBag();
    myFile.close();
    
    return 0;
}