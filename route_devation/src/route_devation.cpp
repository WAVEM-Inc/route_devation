#include"route_devation.hpp"

RouteDevation::RouteDevation():Node("route_devation_node"){
	sub_gps_ = this->create_subscription<GpsMSG>("/sensor/ublox/fix", 1, std::bind(&RouteDevation::gps_callback ,this ,std::placeholders::_1));
	sub_odom_ = this->create_subscription<OdomMSG>("/odom", 1, std::bind(&RouteDevation::odom_callback ,this ,std::placeholders::_1));
	sub_drive_ = this->create_subscription<DriveMSG>("/drive/info", 1, std::bind(&RouteDevation::drive_callback ,this ,std::placeholders::_1));

	cb_group_status_ = create_callback_group(rclcpp::CallbackGroupType::MutuallyExclusive);
	rclcpp::PublisherOptions pub_status_options;
	pub_status_options.callback_group = cb_group_status_;
	pub_status_ = this->create_publisher<StatusMSG>("/drive/route_deviation/status", 1,pub_status_options);
	lat_start= long_start= lat_end= long_end= lat_now= long_now=0;
}

void RouteDevation::drive_callback(const std::shared_ptr<DriveMSG> drive)
{
	StatusMSG status;
	resutlt_distance gps_distance;
	CalcDistance calc;
	//drive_status = drive->code;
	lat_start=drive->start_node.position.latitude;
	long_start=drive->start_node.position.longitude;
	lat_end=drive->end_node.position.latitude;
	long_end=drive->end_node.position.longitude;

	if(((drive->code.compare(std::string("straight"))==0)) || ((drive->code.compare(std::string("recovery"))==0)) )
	{
		if((fabs(lat_start - lat_end) > 0.000001) || (fabs(long_start - long_end) > 0.000001))
		{
			gps_distance = calc.getDistance(lat_start, long_start, lat_end, long_end,KTM);
			if((lat_end - lat_start) > 0)
			{
				tm_end_y=gps_distance.distance_y;
			}
			else
			{
				tm_end_y=-gps_distance.distance_y;
			}
			if((long_end - long_start) > 0)
			{
				tm_end_x=gps_distance.distance_x;
			}
			else
			{
				tm_end_x=-gps_distance.distance_x;
			}
			gps_distance = calc.getDistance(lat_start, long_start, lat_now, long_now,KTM);
			if((lat_now - lat_start) > 0)
			{
				tm_now_y=gps_distance.distance_y;
			}
			else
			{
				tm_now_y=-gps_distance.distance_y;
			}
			if((long_now - long_start) > 0)
			{
				tm_now_x=gps_distance.distance_x;
			}
			else
			{
				tm_now_x=-gps_distance.distance_x;
			}
			if(fabs(lat_start - lat_end) < 0.000001)
			{
				devation_dist = fabs(tm_now_y-tm_end_y);
			}
			else if(fabs(long_start - long_end) < 0.000001)
			{
				devation_dist = fabs(tm_now_x-tm_end_x);
			}
			else
			{
				devation_dist = fabs((tm_end_y)/(tm_end_x)*tm_now_x + (-1)*tm_now_y)/sqrt(pow((tm_end_y)/(tm_end_x),2) + 1);
			}
			if(devation_dist > DEVATION_RANGE)
			{
				route_flag++;
				if(route_flag >= 10)
				{
					route_flag=10;
					status.offcource_status=true;
				}
				else
				{
					status.offcource_status=false;
				}
			}
			else
			{
				if(route_flag>0)
				{
					route_flag=0;
				}
				status.offcource_status=false;
			}
			status.offcource_out_distance=devation_dist;
			status.offcource_goal_distance=devation_offset;
			status.offcource_start_lat=lat_start;
			status.offcource_start_lon=long_start;
			status.offcource_dest_lat=lat_end;
			status.offcource_dest_lon=long_end;
			status.offcource_goal_lat=lat_end;
			status.offcource_goal_lon=long_end;
			status.offcource_goal_x=lat_now;
			status.offcource_goal_y=long_now;
		}
	}
	else
	{
		route_flag=0;
		status.offcource_out_distance=0;
		status.offcource_goal_distance=test_lp;
		status.offcource_start_lat=lat_start;
		status.offcource_start_lon=long_start;
		status.offcource_dest_lat=lat_end;
		status.offcource_dest_lon=long_end;
		status.offcource_goal_lat=lat_end;
		status.offcource_goal_lon=long_end;
		status.offcource_goal_x=lat_now;
		status.offcource_goal_y=long_now;
	}
	pub_status_->publish(status);
}
void RouteDevation::odom_callback(const std::shared_ptr<OdomMSG> odom)
{

}
void RouteDevation::gps_callback(const std::shared_ptr<GpsMSG> gps)
{
	lat_now=gps->latitude;
	long_now=gps->longitude;
	test_lp++;
	usleep(10000);
}

