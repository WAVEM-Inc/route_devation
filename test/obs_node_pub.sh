#!bin/bash
ros2 topic pub /drive/info route_msgs/msg/DriveState "code: 'arrive'
speaker: 0
start_node:
  node_id: '201'
  position:
    latitude: 37.3060172327693
    longitude: 127.24018700061171
  type: 'work_node'
  kind: 'endpoint'
  heading: 148.0
  direction: ''
  driving_option: ''
  detection_range: []
end_node:
  node_id: '202'
  position:
    latitude: 37.3060473
    longitude: 127.2402091
  type: ''
  kind: 'waiting'
  heading: 148.0
  direction: ''
  driving_option: ''
  detection_range: [{offset: 0.5, width_left: 1.0, width_right: 1.0, height: 6.0, action_code: waiting}]" 

