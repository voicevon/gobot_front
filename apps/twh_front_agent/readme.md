# Breif

## Solution A:
* Can be run on user's computer.
* Barcode scanner as keyboard, Connect to SQL server with pyOdbc.
* Task 1:  A hardware barcode scanner is connected as a keyboard.
  * Scanner will read an order_number.
* Task 2: Query SQL server with order_number. 
  * will get order_items.
* Task 3: Generate a withdraw_items from order_items.
  * send the withdraw_items to mqtt-broker.

## Solution B:
* Can be run on user's computer
* Screen capture + OCR to detect data.
* Hardware Barcode scanner as keyboard, Triger the withdraw process.


## Solution C:
* This program will be run on Raspberry Pi.
* VGA-capture as camera1, Then OCR to detect data.
* USB-Camera as camera2, to read barcode.
* Task 1: Capture target screen from VGA-Capture-Camera.
  * Generate order_items from screen images.
  * Publish Json-data of order_items to mqtt broker.
* Task 2: pyzbar as barcode decoder.
  * Publish Json-data of barcode to mqtt broker.


# [How to query SQL server from Pi?](https://stackoverflow.com/questions/44969924/querying-mssql-server-2012-from-a-raspberry-pi-3-using-python-freetds-and-pyodb)
*  pyodbc
*  Microsoft does NOT support odbc-driver for linux on Arm

 

