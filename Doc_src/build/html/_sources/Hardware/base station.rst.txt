:github_url: https://github.com/Karpova-Lab/cerebro/tree/1.0
============
Base Station
============
.. include:: isogrk1.txt
.. include:: mmlalias.txt

.. image:: ../Overview/base_station.jpg
  :align: center
  :scale: 15 %

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
| 1            | Base Station PCB                   |                                                             | `OSH Park <https://oshpark.com/projects/DpBv9BeJ>`_        |
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
  - :download:`Cerebro.h<../../../Arduino/libraries/Cerebro/Cerebro.h>`
  - :download:`Cerebro.cpp<../../../Arduino/libraries/Cerebro/Cerebro.cpp>`

Build Instructions
==================

Required Tools
--------------
  * Soldering iron
  * Tweezers

.. _base upload:

FTDI USB to Serial Converter Setup
----------------------------------
The FTDI USB to serial converter should be setup such that it is powered from the Base Station PCB, not powered over USB.
In order to implement this, the settings on the chip's EEPROM need to be modified.
FTDI provides a utility to do this, which you should download `here <http://www.ftdichip.com/Support/Utilities.htm#FT_PROG>`_

1. Open up Device Manager and reveal the Ports (COM & LPT) list
2. Connect the Base Station to the computer over USB. It should Appear under ports as "USB Serial Port (COMxx)"

.. Error::
  If the device doesn't appear int he Ports list at all, you many need to connect power to the Base Station via the RJ45 jack or DC input jack

.. Error::
  If the device appears in the Ports list as an "Unknown device", then you can manually download and install the `FTDI FT230X Driver <http://www.ftdichip.com/Drivers/VCP.htm>`_

3. Open up FT Prog and go to **DEVICES-->Scan and Parse**

.. image:: photos/FTprog/scanandparse.png
  :align: center
  :scale: 100 %

4. Select "USB Config Descriptor" in the Device Tree. Select "Self Powered" and change the "Max Bus Power" to 0.

.. image:: photos/FTprog/USBconfig.png
  :align: center
  :scale: 100 %

5. Go to **DEVICES-->Program**

.. image:: photos/FTprog/program.png
  :align: center
  :scale: 100 %

6. Click "Program"

.. image:: photos/FTprog/flash.png
  :align: center
  :scale: 100 %

Uploading Firmware
------------------
.. Attention::
  If you have not yet setup the Arduino environment or the Cerebro Utility Shield, refer to :ref:`arduino setup` and/or :ref:`utility shield setup` before moving on.

1. With the notch facing down, place the chip in the lower portion of the :ref:`utility shield`'s ZIF socket.

.. figure:: photos/Setup_photos/85_upload.png
    :align: center
    :scale: 15%

2. Open up the Arduino application and make the following selections under the **Tools** menu:

| **Board:** "Base Station (ATtiny85)"
| **Port:** "COMXX (Arduino/Genuino Uno)"
| **Programmer**: "Arduino as ISP"
|

.. figure:: photos/tools_attiny84.png
    :align: center
    :scale: 100%

3. Burn the bootloader by selecting **Tools -> Burn Bootloader**

.. figure:: photos/basestation_bootloader.png
    :align: center
    :scale: 100%

4. Choose the Base Station firmware by going to **File -> Sketchbook -> Cerebro Sketches -> baseStation**

.. figure:: photos/basestation_firmware.png
    :align: center
    :scale: 100%

5. Upload the firmware by clicking the upload arrow

.. figure:: photos/upload.png
    :align: center
    :scale: 100%
