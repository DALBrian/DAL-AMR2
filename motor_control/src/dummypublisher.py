#!/usr/bin/env python3
import rospy
from std_msgs.msg import String
"""
Created on Thur Mar 9 20:41:43 2023
 For testing publish motor command line in ROS
@author: Small Brian
@email: dalbobo3122@gmail.com
"""
def publish_callback():
    rospy.init_node('talker', anonymous=True)
    pub = rospy.Publisher('motor_command', String, queue_size=10)
    rate = rospy.Rate(10)
    i = 0
    while not rospy.is_shutdown():
        print('i = ', i)
        direction = input('Enter direction: ')
        pub.publish(direction)
        rate.sleep()
        i += 1

if __name__ == '__main__':
    publish_callback()