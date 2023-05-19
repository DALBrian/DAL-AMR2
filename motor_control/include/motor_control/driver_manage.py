import serial
import modbus_tk
import modbus_tk.defines as cst
import modbus_tk.modbus_rtu as modbus_rtu

class driver_manage():
    def __init__(self, PORT = 'COM7'):
        self.master = modbus_rtu.RtuMaster(serial.Serial(port=PORT, baudrate=9600, bytesize=8, parity='N', stopbits=1, xonxoff=0))
        self.master.set_timeout(1.0)
        self.master.set_verbose(True)
        print("Driver connection establish successfully!")
    def __del__(self):
        driver_manage.driver_close()
    
    def driver_close(self):
        if (self.master._do_close()):
            print("Driver connection closed")
    
    def input_source(self, option = 3):
        #value: 0 = use variable resistor as speed input, 1 = space reserve
        #value: 2 = use preset value(Pn212-219), 3 = use RS485-MODBUS as source
        for i in range(1,5):
            self.master.execute(i, 6, 200, output_value=option)
    def Modbus_number(self, start_value):
        #value = 0~255, restart after modification.
        self.master.execute(1, 6, 200, output_value = start_value)
        self.master.execute(2, 6, 200, output_value = start_value + 1)
        self.master.execute(3, 6, 200, output_value = start_value + 2)
        self.master.execute(4, 6, 200, output_value = start_value + 3)
        print("Restart driver after modification")
    def restore(self):
        #motor drivers will be restored
        password = input("WARMING: All setting will be restored, enter 0 to confirm, other value to deny")
        if password == 0:
            for i in range(1,5):
                self.master.execute(i, 6, 200, output_value=1)
    def get_motor_speed(self):
        speed = dict()
        for i in range(1, 5):
            print("Allocating motor no.", i)
            index = str(i)
            speed['motor' + index + ' speed(RPM)'] = self.master.execute(i, cst.READ_HOLDING_REGISTERS, 701, 1)[0]
        return speed
        
    def get_motor_current(self):
        current = dict()
        for i in range(1, 5):
            print("Allocating current of motor no.", i)
            index = str(i)
            current['motor' + index + ' current(A)'] = self.master.execute(i, cst.READ_HOLDING_REGISTERS, 701, 1)[0] * 0.01
        return current
    def comm_error(self):
        comm_error = dict()
        for i in range(1, 5):
            index = str(i)
            error = self.master.execute(i, cst.READ_HOLDING_REGISTERS, 701, 1)
            if error is not None:
                comm_error['motor' + index + ' current'] = error
        return comm_error        
    def show_error(self):
        error = dict()
        for i in range(1, 5):
            index = str(i)
            for j in range(708, 712):
                command = str(j)
                error = self.master.execute(i, cst.READ_HOLDING_REGISTERS, j, 1)
                if error is not None:
                    error['motor' + index + ' error' + command] = error
        return error
