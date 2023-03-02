#include <iostream>
#include <ros/ros.h>
#include <rosbag/bag.h>
#include <rosbag/view.h>
#include <boost/foreach.hpp>

#include <vector>
#include <string>

#include <tf2_msgs/TFMessage.h>
#include <geometry_msgs/TransformStamped.h>
#include <sensor_msgs/Imu.h>

#include <fstream>
#include <iomanip>

void rosbag_message_resort()
{
  std::string bag_path = "/home/jerry/Dataset/opti/0301/raw/2023-03-01-21-17-18.bag";
  std::string imu_topic = "/imu/data";
  std::string tf_topic = "/tf";
  std::string tfs_topic = "/marker_pose";

  rosbag::Bag bag;
  rosbag::Bag newbag;
  bag.open(bag_path, rosbag::bagmode::Read);
  newbag.open("/home/jerry/Dataset/opti/0301/edit/2023-03-01-21-17-18-edit.bag", rosbag::bagmode::Write);

  std::vector<std::string> topics;
  topics.push_back(imu_topic);
  topics.push_back(tf_topic);
  topics.push_back(tfs_topic);

  rosbag::View view;
  rosbag::View view_full;
  view_full.addQuery(bag);
  ros::Time time_start = view_full.getBeginTime();
  ros::Time time_end = view_full.getEndTime();
  view.addQuery(bag, rosbag::TopicQuery(topics), time_start, time_end);

  for (const rosbag::MessageInstance& m : view)
  {
    if (m.getTopic() == imu_topic)
    {
      sensor_msgs::Imu::ConstPtr imu_msg = m.instantiate<sensor_msgs::Imu>();
      newbag.write(imu_topic, imu_msg->header.stamp, imu_msg); 
    }

    
    if (m.getTopic() == tf_topic)
    {
      tf2_msgs::TFMessage::ConstPtr tf_msg = m.instantiate<tf2_msgs::TFMessage>();
      newbag.write(tf_topic, tf_msg->transforms[0].header.stamp, tf_msg); 

      #if 1
      geometry_msgs::TransformStamped::Ptr tfs_msg(new geometry_msgs::TransformStamped);  //智能指针必须初始化
      tfs_msg->header.frame_id = "/mocap_world";
      tfs_msg->header.stamp = tf_msg->transforms[0].header.stamp;
      tfs_msg->child_frame_id = "/marker";
      tfs_msg->transform.rotation = tf_msg->transforms[0].transform.rotation;
      tfs_msg->transform.translation = tf_msg->transforms[0].transform.translation;
      newbag.write(tfs_topic, tf_msg->transforms[0].header.stamp, tfs_msg); 
      #endif
    }
  }

  newbag.close();
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "tool");
  ros::NodeHandle nh("~");
  rosbag_message_resort();
  return 0;
}