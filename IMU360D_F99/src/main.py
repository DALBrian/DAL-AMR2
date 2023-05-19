import sys
import time
import tty, termios
import modbus_tk
import modbus_tk.defines as cst
import modbus_tk.modbus_tcp as modbus_tcp
import modbus_tk.pymodbusclient as modbus_client


if __name__ == "__main__":
    master = modbus_tcp.TcpMaster(host="192.168.1.149")
    # server = modbus_tcp.TcpServer(address="192.168.1.149")
    # client = modbus_client.PyModbusClientTCP(port=502, host="192.168.1.149", timeout=1.0)
    # client.open()
    # print(client.is_open())
    # A = client.read_input_registers(register = 30045)
    # print("data: ", A)
    # info1 = master.execute(1, cst.READ_HOLDING_REGISTERS, 30045, 1)
    # print(info1)
    i = 1
    # info2 = master.execute(1, cst.READ_INPUT_REGISTERS, 16, 8)
    # print(info2)
    while i<11:
        try:
            
            # master.execute(1, cst.READ_INPUT_REGISTERS, i, 8)
            info2 = master.execute(1, cst.READ_INPUT_REGISTERS, i, 4)
            if info2[0]!= 0:
                print("i: ", i)
                print(info2)
            i+=1
            # if i > 10:
            #     i = 1
            #     print("  ")
            #     print("  ")
        except modbus_tk.modbus.ModbusError as e:
            print(e)
            i+=1

    master._do_close()