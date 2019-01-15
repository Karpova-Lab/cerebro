:github_url: https://github.com/Karpova-Lab/cerebro/tree/1.0

Utility Shield
===============
.. include:: isogrk1.txt

.. image:: ../Overview/utility_shield.jpg
  :align: center
  :scale: 20 %

Required Tools
``````````````

Bill of Materials
``````````````````
.. Tip::
  The pins on the vertical USB plug are very short and fail to protrude from standard thickness (~1.6mm) PCBS. Therefore I recommend ordering thinner boards (~0.8mm) to make soldering the connector easier.

+-----+----------------------------------+-----------------------------------------------------------------+-------------------------------------------------------------+
| Qty | Description                      | Datasheet                                                       | Vendor                                                      |
+=====+==================================+=================================================================+=============================================================+
| 1   | Utility Shield PCB               |                                                                 | `OSH Park <https://oshpark.com/shared_projects/NacBaAQJ>`_  |
+-----+----------------------------------+-----------------------------------------------------------------+-------------------------------------------------------------+
| 1   | Arduino Uno                      | :download:`ATmega328<Datasheets/ATmega328.pdf>`                 | `Octopart <https://octopart.com/bom-lookup/n5svTbJz>`_      |
+-----+----------------------------------+-----------------------------------------------------------------+                                                             +
| 1   | 22 pin ZIF socket                | :download:`222-3343-00-0602J<Datasheets/222-3343-00-0602J.pdf>` |                                                             |
+-----+----------------------------------+-----------------------------------------------------------------+                                                             +
| 1   | Male micro USB vertical plug     | :download:`ZX20-B-5S-UNIT(30)<Datasheets/ZX20-B-5S.pdf>`        |                                                             |
+-----+----------------------------------+-----------------------------------------------------------------+                                                             +
| 1   | Male micro USB shielding         | :download:`ZX20-B-SLDC<Datasheets/ZX20-B-5S.pdf>`               |                                                             |
+-----+----------------------------------+-----------------------------------------------------------------+                                                             +
| 1   | Female micro USB vertical socket | :download:`1051330011<Datasheets/1051330011.pdf>`               |                                                             |
+-----+----------------------------------+-----------------------------------------------------------------+                                                             +
| 1   | Lipo charger IC                  | :download:`MCP73831T<Datasheets/MCP73831T.pdf>`                 |                                                             |
+-----+----------------------------------+-----------------------------------------------------------------+                                                             +
| 1   | 1206 red/green LED               | :download:`5977703607F<Datasheets/5977703607F.pdf>`             |                                                             |
+-----+----------------------------------+-----------------------------------------------------------------+                                                             +
| 1   | 0603 Red LED                     | :download:`LTST-C191KRKT<Datasheets/LTST-C191KRKT.pdf>`         |                                                             |
+-----+----------------------------------+-----------------------------------------------------------------+                                                             +
| 1   | Breakaway 0.1" male pins         | :download:`68001-420HLF<Datasheets/68001-420HLF.pdf>`           |                                                             |
+-----+----------------------------------+-----------------------------------------------------------------+                                                             +
| 1   | Tactile Switch                   | :download:`B3FS-1010P<Datasheets/B3FS-1010P.pdf>`               |                                                             |
+-----+----------------------------------+-----------------------------------------------------------------+                                                             +
| 2   | 0805 100 |OHgr| Resistor         |                                                                 |                                                             |
+-----+----------------------------------+-----------------------------------------------------------------+                                                             +
| 1   | 0805 27 k\ |OHgr| Resistor       |                                                                 |                                                             |
+-----+----------------------------------+-----------------------------------------------------------------+                                                             +
| 2   | 0805 4.7 |mgr|\F Capacitor       |                                                                 |                                                             |
+-----+----------------------------------+-----------------------------------------------------------------+                                                             +
| 1   | 0805 10 |mgr|\F Capacitor        |                                                                 |                                                             |
+-----+----------------------------------+-----------------------------------------------------------------+-------------------------------------------------------------+

Design Files
````````````

PCB
	- :download:`Utility Shield 1.4.brd<../../../PCB Files/Utility Shield 1.4/Utility Shield 1.4.brd>`
	- :download:`Utility Shield 1.4.sch<../../../PCB Files/Utility Shield 1.4/Utility Shield 1.4.sch>`
	- :download:`Schematic.pdf<../../../PCB Files/Utility Shield 1.4/Utility Shield 1.4 schematic.pdf>`


.. _arduino setup:

Setting up Arduino IDE
``````````````````````

1. Download and install `Arduino Software <https://www.arduino.cc/en/Main/Software>`_

2. Open up your Arduino Sketchbook folder. It should be located in your main "Documents" folder with the name "Arduino". If you cannot find it, open the Arduino Application and go to **File -> Preferences**. The location of your Sketchbook folder is shown at the top of the preferences window. Your Sketchbook folder will have your current projects and a "libraries" folder.

 .. figure:: photos/Setup_photos/sketchbook_location.png
     :align: center
     :scale: 100%

 .. figure:: photos/Setup_photos/sketchbook.png
     :align: center
     :scale: 75%

3. Download and unzip :download:`Arduino_cerebro.zip<Arduino_cerebro.zip>`

4. Transfer the "Cerebro_sketches" and "Hardware" folders into your Arduino Sketchbook folder

.. figure:: photos/Setup_photos/transfer_sketches_hardware.png
    :align: center
    :width: 100%

5. Transfer the remaining library folders into the "libraries" folder

.. figure:: photos/Setup_photos/transfer_libraries.png
    :align: center
    :width: 100%

.. _utility shield setup:

Setting up Utility Shield
`````````````````````````
To upload firmware, we will be using an Arduino Uno as an In-System Programmer (ISP).
The Utility Shield connects to the Arduino Uno to route the programming signals to the
correct pins on various AVR microcontrollers.

1. Connect an Arduino Uno to your computer via USB
2. Open the Arduino application and go to **File -> Examples -> ArduinoISP**

.. figure:: photos/Setup_photos/arduinoISP.png
    :align: center
    :scale: 80%

3. Make the following selections under the **Tools** menu:

| **Board:** "Arduino/Genuino Uno"
| **Port:** "COMXX (Arduino/Genuino Uno)"
|

.. figure:: photos/Setup_photos/ISP_config.png
    :align: center
    :scale: 80%

5. Upload the ArduinoISP firmware by clicking the upload arrow

.. figure:: photos/upload.png
    :align: center
    :width: 100%

6. Connect the :ref:`utility shield` to your Arduino Uno

.. figure:: photos/Setup_photos/shield_on.jpg
    :align: center
    :width: 100%

7. You are ready to start programming :ref:`Cerebro <cerebro upload>` , :ref:`Base Station <base upload>` , :ref:`IR Remote <remote upload>` and :ref:`Charging Dock <dock upload>`
