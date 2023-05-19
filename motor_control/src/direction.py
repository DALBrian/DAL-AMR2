import driver_connection
driver = driver_connection.driver_connection(PORT = "/dev/ttyUSB1")
master = driver.master
for i in [2, 4]:
    master.execute(i, 6, 5, output_value = 0)
