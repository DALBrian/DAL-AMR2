#!/usr/bin/env python3
### Import ROS library ###
import rospy
from std_msgs.msg import String
### Import self-build library ###
import sys
sys.path.append('../include/motor_control')
import driver_connection
import chassic_kinematic
"""
Created on Thur Mar 9 20:41:43 2023

@author: Small Brian
@email: dalbobo3122@gmail.com
Use basic text command to control robot.

"""
class communication():
    def __init__(self):
        print("Entering __init__") #Add this line for testing without driver connection
        # self.driver = driver_connection.driver_connection()
        # self.kinematic = chassic_kinematic.chassic_kinematic(self.driver)

    def command_callback(self, data):
        # print("enterning commandd_callback")
        if data.data == 'FWD' or data.data == 'w':
            print("forward")
            # self.kinematic.fwd()
        elif data.data == 'BWD' or data.data == 's':
            print("backward")
            # self.kinematic.bwd()
        elif data.data == 'LEFT' or data.data == 'a':
            print("left")
            # self.kinematic.left()
        elif data.data == 'RIGHT' or data.data == 'd':
            print("right")
            # self.kinematic.right()
        elif data.data == 'CW' or data.data == 'e':
            print("rotating clock wise")
            # self.kinematic.rot_cw()
        elif data.data == 'CCW' or data.data == 'q':
            print("rotating counter clock wise")
            # self.kinematic.rot_ccw()
        elif data.data == 'STOP':
            print("stop")
            # self.kinematic.stop()
        else:
            print("emergency stop")
            # self.kinematic.stop() # stop when reading unknown command in case of emergency



if __name__ == '__main__':
    com = communication()
    rospy.init_node('motor_control', anonymous=True)
    rospy.Subscriber('motor_command', String, com.command_callback)
    rospy.spin()
