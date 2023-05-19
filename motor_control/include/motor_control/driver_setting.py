# -*- coding: utf-8 -*-
"""
Created on Mon Feb 13 19:39:43 2023

@author: Small Brian
@email: dalbobo3122@gmail.com
"""
class driver_setting():
    def __init__(self, driver_connection):
        self.master = driver_connection.master
        
    def _change_direction(self,motor1 = 1, motor2 = 0, motor3 = 1, motor4 = 0):
        """
        0: Counter-clock wise(from the view of load of motor, also as from the exterior of the wheel)
        1: Clock wise
        """
        self.master.execute(1, 6, 10, output_value = motor1)
        self.master.execute(2, 6, 10, output_value = motor2)
        self.master.execute(3, 6, 10, output_value = motor3)
        self.master.execute(4, 6, 10, output_value = motor4)
        
    def _DI0(self, option = 1):
        """
        Change the function of digital input 0, default as servo ON.
        Same documentation for DI0~DI5
        -------
        0: None
        1: servo ON
        2: Alarm clear
        3: Inverse rotation limit(if ON, reverse rotation is prohibited)
        4: Forward rotation limit(if ON, forward rotation is prohibited)
        5: None
        6: Direction reverse(if ON, forward and inverse rotation direction will exchange)
        7: SST(acceleration time choose, usage unclear)
        8: SPD1(choose pre-define speed, usage unclear)
        9: SPD2(choose pre-define speed, usage unclear)
        10: SPD3(choose pre-define speed, usage unclear)
        """
        for i in range(1, 5):
            self.master.execute(i, 6, 10, output_value = option)
            
    def _DI1(self, option = 2):
        """
        Change the function of digital input 1, default as 2(Alarm clear).
        """
        for i in range(1, 5):
            self.master.execute(i, 6, 11, output_value = option)
            
    def _DI2(self, option = 3):
        """
        Change the function of digital input 1, default as 3(Inverse rotation limit).
        """
        for i in range(1, 5):
            self.master.execute(i, 6, 12, output_value = option)
            
    def _DI3(self, option = 4):
        """
        Change the function of digital input 1, default as 4(Forward rotation limit).
        """
        for i in range(1, 5):
            self.master.execute(i, 6, 13, output_value = option)
            
    def _DI4(self, option = 8):
        """
        Change the function of digital input 1, default as 8(SPD1).
        """
        for i in range(1, 5):
            self.master.execute(i, 6, 14, output_value = option)
            
    def _DI5(self, option = 9):
        """
        Change the function of digital input 1, default as 9(SPD2).
        """
        for i in range(1, 5):
            self.master.execute(i, 6, 15, output_value = option)
    
    def _DO0(self, option = 0):
        """
        Change the function of digital output 0, default as 0(ALARM).
        Same documentation for DO0~DO2
        ----------
        0: Alarm
        1: Servo ready
        2: zero speed(reference user manual Pn222)
        3: None
        4: Reach specific rotation speed
        5: No this option
        6: Running
        7: Limiting torque
        8: Breaking
        9: Rotating direction(HIGH = CW, LOW = CCW)
        """
        for i in range(1, 5):
            self.master.execute(i, 6, 26, output_value = option)
    def _DO1(self, option = 0):
        """
        Change the function of digital output 1, default as 4(Reach specific rotation speed).
        """
        for i in range(1, 5):
            self.master.execute(i, 6, 27, output_value = option)
    def _DO2(self, option = 0):
        """
        Change the function of digital output 1, default as 6(Running).
        """
        for i in range(1, 5):
            self.master.execute(i, 6, 28, output_value = option)
    def _speed_limit(self, value = 50):
        """
        Change the value of motor rotation speed, default as 50(reduction ration = 10, so 5 round per min)
        """
        for i in range(1, 5):
            self.master.execute(i, 6, 201, output_value = value)
    def _torque_limit(self, value = 280):
        """
        Change the value of motor torque, default as 280(0~280%)
        """
        for i in range(1, 5):
            self.master.execute(i, 6, 224, output_value = value)#Pn224 = forward
            self.master.execute(i, 6, 225, output_value = value)#Pn225 = reverse
    def _driver_address(self, motor1 = 1, motor2 = 2, motor3 = 3, motor4 = 4):
        """
        Change the RS-485 address of each driver, each address cannot be the same(0~255)
        """
        self.master.execute(1, 6, 500, output_value = motor1)
        self.master.execute(2, 6, 500, output_value = motor2)
        self.master.execute(3, 6, 500, output_value = motor3)
        self.master.execute(4, 6, 500, output_value = motor4)      
        print("Restart driver after modification")
    def _driver_restore(self):
        """
        Restore all driver setting. Enter 0 to confirm
        """
        password = input("Restore all driver setting. Enter 0 to confirm, other value to deny")
        if password == 0:
            for i in range(1, 5):
                self.master.execute(i, 6, 601, output_value = 1)
        else:
            print("Restoration deny, remain setting")
