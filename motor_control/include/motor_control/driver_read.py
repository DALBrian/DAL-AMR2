# -*- coding: utf-8 -*-
"""
Created on Mon Feb 13 17:02:00 2023

@author: Small Brian
@email: dalbobo3122@gmail.com
"""
import modbus_tk.defines as cst
class driver_read():
    def __init__(self, driver_connection):
        if (driver_connection.active == True):
            print("Driver connection established")
            self.master = driver_connection.master
    def _get_speed(self):
        speed = dict()
        for i in range(1, 5):
            print("Allocating speed of motor no.", i)
            index = str(i)
            spd = self.master.execute(i, cst.READ_HOLDING_REGISTERS, 700, 1)[0]
            if spd is not None:
                speed['motor' + index + ' speed(RPM)'] = spd
        return speed
    def _get_current(self):
        current = dict()
        for i in range(1, 5):
            print("Allocating current of motor no.", i)
            index = str(i)
            cur = self.master.execute(i, cst.READ_HOLDING_REGISTERS, 702, 1)[0] * 0.01
            if cur is not None:
                current['motor' + index + ' current(A)'] = cur
        return current
    def _get_DI_status(self):
        """
        Show the status from DI0 to DI5, you may use this function to check input status in case of switch failure.
        Returns 7 binary numbers, 1 means specific digital input is "ON"
        -------
        Example : 0100100 = DI2 and DI5 are ON
        """
        DI_status = dict()
        for i in range(1, 5):
            print("Allocating DI status of motor no.", i)
            index = str(i)
            sta = self.master.execute(i, cst.READ_HOLDING_REGISTERS, 705, 1)[0]
            if sta is not None:
                DI_status['motor' + index + ' input status'] = sta
        return DI_status
    def _get_DO_status(self):
        """
        Show the status from DO0 to DO2, you may use this function to check output status in case of LED failure.
        Returns 3 binary numbers, 1 means specific digital input is "LOW"
        -------
        Example : 101 = DO0 and DO2 are "LOW"
        """
        DO_status = dict()
        for i in range(1, 5):
            print("Allocating DI status of motor no.", i)
            index = str(i)
            sta = self.master.execute(i, cst.READ_HOLDING_REGISTERS, 706, 1)[0]
            if sta is not None:
                DO_status['motor' + index + ' output status'] = sta
        return DO_status
    def _get_ERROR(self):
        """
        Show error code in the driver.

        Returns a dictionary of error codes of each driver.
        -------
        Error code:
            1: EEPROM storage error
            2: ADC failure
            3: high voltage 
            4: low voltage
            5: high current
            6: overload
            7: High rotation speed 
            8: Feedback components failure
            9:High driver temperature
        """
        errorcode = dict()
        for i in range(1, 5):
            print("Allocating error code of motor no.", i)
            index = str(i)
            for j in range(707, 712):
                times = str(j-706)
                code = self.master.execute(i, cst.READ_HOLDING_REGISTERS, j, 1)[0]
                sen = ""
                if code == 1:
                    sen = "EEPROM storage error"
                elif code == 2:
                    sen = "ADC failure"
                elif code == 3:
                    sen = "High voltage"
                elif code == 4:
                    sen = "Low voltage"
                elif code == 5:
                    sen = "High current"
                elif code == 6:
                    sen = "Overload"
                elif code == 7:
                    sen = "High rotation speed"
                elif code == 8:
                    sen = "Feedback components failure"
                elif code == 9:
                    sen = "High driver temperature"
                elif code == None:
                    sen = ""
                else:
                    sen = ""
                    print("Return value: ", code," does not match")
                errorcode['motor' + index + ' errorcode' + times] = sen
        return errorcode

