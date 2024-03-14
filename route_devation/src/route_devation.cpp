#include"route_devation.hpp"

RouteDevation::RouteDevation():Node("route_devation_node"){
	sub_gps_ = this->create_subscription<GpsMSG>("/sensor/ublox/fix", 1, std::bind(&RouteDevation::gps_callback ,this ,std::placeholders::_1));
	sub_odom_ = this->create_subscription<OdomMSG>("/odom", 1, std::bind(&RouteDevation::odom_callback ,this ,std::placeholders::_1));
	sub_drive_ = this->create_subscription<DriveMSG>("/drive/info", 1, std::bind(&RouteDevation::drive_callback ,this ,std::placeholders::_1));

	cb_group_status_ = create_callback_group(rclcpp::CallbackGroupType::MutuallyExclusive);
	rclcpp::PublisherOptions pub_status_options;
	pub_status_options.callback_group = cb_group_status_;
	pub_status_ = this->create_publisher<StatusMSG>("/drive/route_devation/status", 1,pub_status_options);
}

void RouteDevation::drive_callback(const std::shared_ptr<DriveMSG> drive)
{
	resutlt_distance gps_distance;
	CalcDistance calc;
	//	drive_status = drive.code;
	lat_start=drive->start_node.position.latitude;
	long_start=drive->start_node.position.longitude;
	lat_end=drive->end_node.position.latitude;
	long_end=drive->end_node.position.longitude;

	if(drive->code.compare("driving"))
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
		devation_offset = tm_end_x - (tm_now_x - (tm_end_x*tm_now_y)/tm_end_x);
		devation_dist = fabs((tm_end_y)/(tm_end_x)*tm_now_x + (-1)*tm_now_y)/sqrt(pow((tm_end_y)/(tm_end_x),2) + 1);
	}
}
void RouteDevation::odom_callback(const std::shared_ptr<OdomMSG> odom)
{
		
}
void RouteDevation::gps_callback(const std::shared_ptr<GpsMSG> gps)
{
	lat_now=gps->latitude;
	long_now=gps->longitude;
}

