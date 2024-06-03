#!bin/bash
ros2 topic pub /drive/info route_msgs/msg/DriveState "code: 'arrive'
speaker: 0
start_node:
  node_id: '201'
  position:
    latitude: 37.3061467
    longitude: 127.2401776
  type: ''
  kind: 'connecting'
  heading: 148.0
  direction: ''
  driving_option: ''
  detection_range: []
end_node:
  node_id: '202'
  position:
    latitude: 37.3061114
    longitude: 127.2401561
  type: ''
  kind: 'waiting'
  heading: 148.0
  direction: ''
  driving_option: ''
  detection_range: [{offset: 2.0, width_left: 5.0, width_right: 1.0, height: 6.0, action_code: cooperative}]" 

