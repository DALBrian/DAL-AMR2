#!/usr/bin/env python3
### 20230411 ###
import sys
import numpy as np
sys.path.append('../include/motor_control')
import rospy
import driver_connection
import driver_read
import driver_setting
import chassic_kinematic_v2
import time
from geometry_msgs.msg import Twist
"""
Created on Mon Apr  2023

@author: Small Brian
@email: dalbobo3122@gmail.com

Half-done file. Abandon and left unmaintained.
"""

class command():
    def __init__(self):
        self.param = {}	
        self.param["vel_gain"] = float( rospy.get_param('~vel_gain', '90') ) 
        self.param["omg_gain"] = float( rospy.get_param('~omg_gain', '90') )
        self.param["car_size_x"] = float( rospy.get_param('~car_size_x', '1.7') )
        self.param["car_size_y"] = float( rospy.get_param('~car_size_y', '2.1') )
        self.param["wheel_radius"] = float( rospy.get_param('~wheel_radius', '0.34') )
        self.param["vel_cmd"] = "vel_cmd"
        self.veh_cmd = {"Vx":0, "Vy":0, "Omega":0}
        self.create_H()
        self.rotating_speed = np.zeros([4, 1])
        rospy.loginfo("Initiating Node")
        rospy.loginfo("Subscribing to \"" + self.param["vel_cmd"] + "\"")
    def command_callback(self, vel_cmd):
        self.veh_cmd["Vx"] = vel_cmd.linear.x * self.param["vel_gain"]
        self.veh_cmd["Vy"] = vel_cmd.linear.y * self.param["vel_gain"]
        self.veh_cmd["Omega"] = vel_cmd.angular.z * self.param["omg_gain"]
        self.no_cmd_received = False
        self.last_cmd_vel_time = time.time()
        command.control()
    def control(self):
        
    def create_H(self):
        AB = self.param["car_size_x"] * self.param["car_size_y"]
        self.H = np.zeros([4,3])
        self.H[0][0] = 1
        self.H[0][1] = -1
        self.H[0][2] = -1* AB
        self.H[1][0] = 1
        self.H[1][1] = 1
        self.H[1][2] = AB
        self.H[2][0] = 1
        self.H[2][1] = 1
        self.H[2][2] = -1* AB
        self.H[3][0] = 1
        self.H[3][1] = -1
        self.H[3][2] = AB

    def rotating_speed(self):
        self.velocity = np.zeros([3,1])
        self.velocity[0][0] = self.veh_cmd["Vx"]
        self.velocity[1][0] = self.veh_cmd["Vy"]
        self.velocity[2][0] = self.veh_cmd["Omega"]


        
if __name__ == '__main__':
    driver = driver_connection.driver_connection()
    move = chassic_kinematic_v2.chassic_kinematic(driver)
    com = command()
    rospy.init_node('motor_control', anonymous=True)
    rospy.Subscriber('/cmd_vel', Twist, com.command_callback)
    rospy.spin()