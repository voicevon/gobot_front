# Breif
* This programe will be run on Raspberry Pi.
## Solution A:
* Task 1:  A barcode scanner is connected as a keyboard.
  * Scanner will read a order_number.
* Task 2: Query SQL server with order_number. 
  * will get order_items.
* Task 3: Generate a withdraw_items from order_items.
  * send the withdraw_items to mqtt-broker.

## Solution B:
* Task 1: Capture target screen from VGA-Capture-Camera.
  * Generate order_items from screen images.
  * Publish Json-data of order_items to mqtt broker.
* Task 2: A barcode scanner is connected as a keyboard.
  * Publish Json-data of barcode to mqtt broker.


# [How to query SQL server from Pi?](https://stackoverflow.com/questions/44969924/querying-mssql-server-2012-from-a-raspberry-pi-3-using-python-freetds-and-pyodb)
*  pyodbc
*  Microsoft does NOT support odbc-driver for linux on Arm

 

