#!/usr/bin/env python3
import rospy
from geometry_msgs.msg import Twist

def sub(Twist):
    print("\n")
    print(Twist)

if __name__ == '__main__':
    rospy.init_node('fake_subscriber', anonymous=True)
    rospy.Subscriber('/cmd_vel', Twist, sub) ##!!!!! different, cmd_vel 
    rospy.spin()
