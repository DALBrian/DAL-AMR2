#!/usr/bin/env python3
### 20230411 ###
import sys
import numpy as np
# sys.path.append("../include/motor_control")
import rospy
import driver_connection
# import driver_read
# import driver_setting
import chassic_kinematic_v3
import time
from geometry_msgs.msg import Twist
"""
Created on April 16  2023
@author: Small Brian
@email: dalbobo3122@gmail.com

This file includes the correct kinematic and uses ROS parameter server for easily modification.
This is the file used for control robot. Candicate for the final version.
"""

class command():
    def __init__(self):
        self.param = {}	
        self.param["vel_gain"] = float( rospy.get_param('vel_gain', '1') ) 
        print("vel_gain: ", self.param["vel_gain"])
        self.param["omg_gain"] = float( rospy.get_param('omg_gain', '1') )
        print("omg_gain: ", self.param["omg_gain"])
        self.param["car_size_x"] = float( rospy.get_param('car_size_x', '1.7') )
        self.param["car_size_y"] = float( rospy.get_param('car_size_y', '2.1') )
        self.param["wheel_radius"] = float( rospy.get_param('wheel_radius', '0.34') )
        self.param["cmd_vel"] = rospy.get_param('~cmd_vel', 'cmd_vel')
        self.veh_cmd = {"Vx":0, "Vy":0, "Omega":0}
        print("param: ", self.param)
        self.create_H()
        self.cmd_received = False
        self.velocity = np.zeros([3,1]) #vehicle velocity( vx, vy, omega)
        self.rotating_speed = np.zeros([4, 1]) #
        rospy.loginfo("Initiating motor control & kinematic node")
        rospy.loginfo("Subscribing to \"" + self.param["cmd_vel"] + "\"")

    def create_H(self):
        XY = self.param["car_size_x"] * self.param["car_size_y"]
        self.H = np.zeros([4,3])
        self.H[0][0] = XY
        self.H[0][1] = 1
        self.H[0][2] = 1
        self.H[1][0] = -XY
        self.H[1][1] = 1
        self.H[1][2] = 1
        self.H[2][0] = XY
        self.H[2][1] = 1
        self.H[2][2] = -1
        self.H[3][0] = -XY
        self.H[3][1] = 1
        self.H[3][2] = -1
    def command_callback(self, vel_cmd):
        
        self.veh_cmd["Vx"] = vel_cmd.linear.x * self.param["vel_gain"] #consider put gain at velocity 
        self.veh_cmd["Vy"] = vel_cmd.linear.y * self.param["vel_gain"]
        self.veh_cmd["Omega"] = vel_cmd.angular.z * self.param["omg_gain"]
    
        self.velocity[0][0] = self.veh_cmd["Omega"]
        self.velocity[1][0] = self.veh_cmd["Vx"]
        self.velocity[2][0] = self.veh_cmd["Vy"]

        self.cmd_received = True

        self.rotating_speed = np.matmul(self.H , self.velocity) / self.param["wheel_radius"]
        if self.cmd_received:
            self.cmd_received = False
            self.control()
    def control(self):
        print("speed: ", self.rotating_speed)
        move.motor1(int(self.rotating_speed[0][0]))#re-check the motor number
        move.motor2(int(self.rotating_speed[1][0]))
        move.motor3(int(self.rotating_speed[2][0]))
        move.motor4(int(self.rotating_speed[3][0]))



if __name__ == '__main__':
    rospy.init_node('motor_control', anonymous=True)
    port =  rospy.get_param('port', '/dev/ttyUSB1')
    driver = driver_connection.driver_connection(PORT = port)
    move = chassic_kinematic_v3.chassic_kinematic(driver)
    com = command()
    rospy.Subscriber('/cmd_vel', Twist, com.command_callback)
    rospy.spin()
    