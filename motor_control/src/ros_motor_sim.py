#!/usr/bin/env python3
### 20230411 ###
import sys
sys.path.append('../include/motor_control')
import rospy
import chassic_kinematic_v2_sim
import time
from geometry_msgs.msg import Twist
# test driver_connection and driver_read
"""
Created on April 11  2023
@author: Small Brian
@email: dalbobo3122@gmail.com

No inverse kinematic here. Use basic if-loop to match the command from teleop_twist_keyboard.
Turns out teleop_twist_keyboard command is not identical with the path planner command.
Still usable but no kinematic.
"""
class command():
    def __init__(self):
        print("Waiting for command")
        self.param = {}	
        self.param["vel_gain"] = float( rospy.get_param('~vel_gain', '90') ) # to match physical world
        self.param["omg_gain"] = float( rospy.get_param('~omg_gain', '90') ) #
        self.param["vel_cmd"] = "vel_cmd"
        self.veh_cmd = {"Vx":0, "Vy":0, "Omega":0}
        rospy.loginfo("Initiating Node")
        rospy.loginfo("Subscribing to \"" + self.param["vel_cmd"] + "\"")
    def command_callback(self, vel_cmd):
        self.veh_cmd["Vx"] = vel_cmd.linear.x * self.param["vel_gain"]
        self.veh_cmd["Vy"] = vel_cmd.linear.y * self.param["vel_gain"]
        self.veh_cmd["Omega"] = vel_cmd.angular.z * self.param["omg_gain"]
        self.no_cmd_received = False
        self.last_cmd_vel_time = time.time()
        self.control()
    def control(self):
        if self.veh_cmd["Vy"] == 0:
            if self.veh_cmd["Vx"] == 0:
                move.stop()
            elif self.veh_cmd["Vx"] > 0:
                move.fwd(self.veh_cmd["Vx"])
            else:
                 move.bwd(abs(self.veh_cmd["Vx"]))
        if self.veh_cmd["Vx"] == 0:
            if self.veh_cmd["Vy"] > 0:
                move.left(self.veh_cmd["Vy"])
            elif self.veh_cmd["Vy"] < 0:
                 move.right(abs(self.veh_cmd["Vy"]))

        if self.veh_cmd["Vx"] > 0 and self.veh_cmd["Vy"] > 0:
            move.left_fwd(abs(self.veh_cmd["Vx"]))
        elif self.veh_cmd["Vx"] < 0 and self.veh_cmd["Vy"] > 0:
            move.left_bwd(abs(self.veh_cmd["Vx"]))
        elif self.veh_cmd["Vx"] > 0 and self.veh_cmd["Vy"] < 0:
            move.right_fwd(abs(self.veh_cmd["Vx"]))
        elif self.veh_cmd["Vx"] < 0 and self.veh_cmd["Vy"] < 0:
            move.right_bwd(abs(self.veh_cmd["Vx"]))
        self.no_cmd_received == True
if __name__ == '__main__':
    move = chassic_kinematic_v2_sim.driver_kinekatic()
    com = command()
    rospy.init_node('motor_control', anonymous=True)
    rospy.Subscriber('/cmd_vel', Twist, com.command_callback)
    rospy.spin()