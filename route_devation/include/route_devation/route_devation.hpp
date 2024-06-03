#ifndef ROUTE_DEVATION_LISTENER_HPP__
#define ROUTE_DEVATION_LISTENER_HPP__
#define DEVATION_RANGE 0.5

#include <iostream>
#include <fstream>
#include <fcntl.h>
#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/nav_sat_fix.hpp"
#include "routedevation_msgs/msg/status.hpp"
#include "nav_msgs/msg/odometry.hpp"
#include "route_msgs/msg/drive_state.hpp"
#include "distance/geo_trans.hpp"
#include "distance/calc_distance.hpp"
#include <unistd.h>
using GpsMSG = sensor_msgs::msg::NavSatFix;
using StatusMSG = routedevation_msgs::msg::Status;
using DriveMSG = route_msgs::msg::DriveState;
using OdomMSG = nav_msgs::msg::Odometry;
class RouteDevation : public rclcpp::Node{
    private :
        rclcpp::CallbackGroup::SharedPtr cb_group_status_;
        rclcpp::Publisher<StatusMSG>::SharedPtr pub_status_;

        rclcpp::Subscription<DriveMSG>::SharedPtr sub_drive_;
        rclcpp::Subscription<OdomMSG>::SharedPtr sub_odom_;
        rclcpp::Subscription<GpsMSG>::SharedPtr sub_gps_;
        void gps_callback(const std::shared_ptr<GpsMSG> gps);
	void odom_callback(const std::shared_ptr<OdomMSG> odom);
	void drive_callback(const std::shared_ptr<DriveMSG> drive);

	int drive_status;
	float lat_start, long_start, lat_end, long_end, lat_now, long_now;
	float tm_end_x,tm_end_y, tm_now_x,tm_now_y,tm_contact_x,tm_contact_y;
	float devation_dist, route_angle, contact_angle,devation_offset;
	int route_flag=0;
	int test_lp=0;
    public :
        RouteDevation();
	

};
#endif
