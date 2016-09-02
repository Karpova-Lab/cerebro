============
Base Station
============
.. include:: isogrk1.txt
.. include:: mmlalias.txt

Specifications
==============
.. csv-table::
  :header: "Parameter", "Test Condition", "Value", "Unit"
  :widths: 3,3,3,3

  Supply Voltage, , 5, V
  Current, , 97, mA
  Transmission distance, , 5, m

Bill of Materials
=================

+--------------+------------------------------------+-------------------------------------------------------------+------------------------------------------------------------+
| Qty          | Description                        | Datasheet                                                   | Vendor                                                     |
+==============+====================================+=============================================================+============================================================+
| 1            | Base Station PCB                   |                                                             | `OSH Park <https://oshpark.com/shared_projects/Vdjb4Wi1>`_ |
+--------------+------------------------------------+-------------------------------------------------------------+------------------------------------------------------------+
| 1            | Microcontroller                    | :download:`ATTINY85<Datasheets/ATtiny85.pdf>`               | `Octopart <https://octopart.com/bom-lookup/0zhqz2yT>`_     |
+--------------+------------------------------------+-------------------------------------------------------------+                                                            |
| 1            | FTDI USB to Serial Converter       | :download:`FT230X<Datasheets/FT230X.pdf>`                   |                                                            |
+--------------+------------------------------------+-------------------------------------------------------------+                                                            |
| 1            | Infrared LED                       | :download:`VSMS3700<Datasheets/VSMS3700.pdf>`               |                                                            |
+--------------+------------------------------------+-------------------------------------------------------------+                                                            |
| 1            | RJ45 Horizontal connector          | :download:`54602-908LF<Datasheets/54602-908LF.pdf>`         |                                                            |
+--------------+------------------------------------+-------------------------------------------------------------+                                                            |
| 1            | Female micro USB horizontal socket | :download:`10118193-0001LF<Datasheets/10118193-0001LF.pdf>` |                                                            |
+--------------+------------------------------------+-------------------------------------------------------------+                                                            |
| 1            | NPN Transistor                     | :download:`PN2222A<Datasheets/PN2222A.pdf>`                 |                                                            |
+--------------+------------------------------------+-------------------------------------------------------------+                                                            |
| 2            | 0805 27 |OHgr| Resistor            |                                                             |                                                            |
+--------------+------------------------------------+-------------------------------------------------------------+                                                            |
| 1            | 1/4W 39 |OHgr| Resistor            |                                                             |                                                            |
+--------------+------------------------------------+-------------------------------------------------------------+                                                            |
| 1            | 0805 4.7 k\ |OHgr| Resistor        |                                                             |                                                            |
+--------------+------------------------------------+-------------------------------------------------------------+                                                            |
| 3            | 0805 10 k\ |OHgr| Resistor         |                                                             |                                                            |
+--------------+------------------------------------+-------------------------------------------------------------+                                                            |
| 1            | 0805 100 |OHgr| Resistor           |                                                             |                                                            |
+--------------+------------------------------------+-------------------------------------------------------------+                                                            |
| 2            | 0805 4.7 |mgr|\F Capacitor         |                                                             |                                                            |
+--------------+------------------------------------+-------------------------------------------------------------+                                                            |
| 2            | 0805 100 nF Capacitor              |                                                             |                                                            |
+--------------+------------------------------------+-------------------------------------------------------------+------------------------------------------------------------+

Design Files
============
PCB
	- :download:`Base Station 1.8.brd<../../../PCB Files/Base Station 1.8/Base Station 1.8.brd>`
	- :download:`Base Station 1.8.sch<../../../PCB Files/Base Station 1.8/Base Station 1.8.sch>`
	- :download:`Schematic.pdf<../../../PCB Files/Base Station 1.8/Base Station 1.8 schematic.pdf>`

Lasercut
  - :download:`Behavior box top.dxf<../../../Lasercut/Behavior box top/Behavior box top.dxf>`
  - :download:`Behavior box top.ai<../../../Lasercut/Behavior box top/Behavior box top.ai>`

Code
====
  - :download:`baseStation.ino<../../../Arduino/Cerebro_Sketches/baseStation/baseStation.ino>`
  - :download:`Cerebro.h<../../../Arduino/Cerebro/Cerebro.h>`
  - :download:`Cerebro.cpp<../../../Arduino/Cerebro/Cerebro.cpp>`

Build Instructions
==================

Required Tools
--------------
  * Soldering iron
  * Tweezers

Uploading Firmware
------------------

1. With the notch facing down, place the chip in the lower portion of the Uploader Shield's ZIF socket.

.. figure:: photos/Setup_photos/85_upload.png
    :align: center
    :scale: 15%

2. Open the Arduino Software and select **Tools -> Board: -> Base Station (ATtiny85)**

.. figure:: photos/Setup_photos/board_select.png
    :align: center
    :scale: 100%

3. Choose the COM port that your Arduino Uno is connected to by selecting **Tools -> Port: ->**
4. Select **Tools -> Programmer -> Arduino as ISP**
5. Burn the bootloader by selecting **Tools -> Burn Bootloader**

.. figure:: photos/Setup_photos/burn_bootloader.png
    :align: center
    :scale: 100%

6. Choose the Base Station firmware by going to **File -> Sketchbook -> Cerebro Sketches -> baseStation**

.. figure:: photos/Setup_photos/select_sketch.png
    :align: center
    :scale: 100%

7. Upload the firmware by clicking the upload arrow

.. figure:: photos/Setup_photos/upload.png
    :align: center
    :scale: 100%
