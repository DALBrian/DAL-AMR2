###Modified to cater Ubuntu. 20230315###
import sys
import rospy
from geometry_msgs.msg import Twist
sys.path.append('../include')


if __name__ == '__main__':
    # test driver_connection and driver_read
    rospy.init_node("Twist_publisher", anonymous=True)
    rate = rospy.Rate(10)
    pub = rospy.Publisher("/cmd_vel", Twist, queue_size=1)
    x = 0
    y = 0
    omg = 0 
    twist = Twist()
    while(True):
        direction = input("Enter direction: ")
        if direction == "w":
            print("Moving forward")
            x = 0.5
            y = 0.0
            omg = 0 
        elif direction == "x":
            print("Moving backward")
            x = -0.5
            y = 0
            omg = 0 
        elif direction == "a":
            print("Moving left")
            x = 0
            y = 0.5
            omg = 0 
        elif direction == "d":
            print("Moving right")
            x = 0
            y = -0.5
            omg = 0 
        elif direction == "r":
            print("Rotating CCW ")
            x = 0
            y = 0
            omg = 1 
        elif direction == "t":
            print("Rotating CW ")
            x = 0
            y = 0
            omg = -1 
        elif direction == "s":
            print("Stopping")
            x = 0
            y = 0
            omg = 0 
        elif direction == "q":
            print("left forward")
            x = 0.5
            y = 0.5
            omg = 0 
        elif direction == "e":
            x = 0.5
            y = -0.5
            omg = 0 
        elif direction == "z":
            print("left back")
            x = -0.5
            y = 0.5
            omg = 0 
        elif direction == "c":
            print("right back")
            x = -0.5
            y = -0.5
            omg = 0 
        elif direction =="exit":
            break
        else:
            print("Stopping")
            x = 0
            y = 0
            omg = 0
        twist.linear.x = x
        twist.linear.y = y
        twist.linear.z = 0
        twist.angular.x = 0
        twist.angular.y = 0
        twist.angular.z = omg
        pub.publish(twist)
    