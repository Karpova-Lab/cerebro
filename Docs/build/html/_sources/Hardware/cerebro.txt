=======
Cerebro
=======
.. include:: isogrk1.txt
.. include:: mmlalias.txt

Specifications
==============

.. csv-table::
  :header: "Parameter", "Test Condition", "Value", "Unit"
  :widths: 3,3,3,3

  Weight,,13.2, g
  Dimensions, Receiver, 0.68 x 0.74 x 0.062 , in
            , Implant, 1.05 x 0.40 x 0.75, in
  :download:`Battery Life <photos/battery new.html>`, "520nm, 8mW 3s pulse, 6 pulses/min ", 4.25 (1700), "hours (pulses)"
  Recharge time, ,1 ,hour
  Light Wavelength, ":download:`HL63603TG<Datasheets/HL63603TG.pdf>` (Red)", "638", "nm"
                  , ":download:`PLT5 520<Datasheets/PL520_B1.pdf>` (Green)", "520","nm"
  Memory, , 8192, bytes
  :download:`Trigger Latency <photos/Signal_Breakdown.png>`, , 3.06 |pm|\  0.05, ms
  Rise Time, , 40 |pm|\  10, ms
  Fiber Diameter, , 200,|mgr|\m
  Light Output (per fiber) ,520 nm diode coupled to 200 |mgr|\m fiber, 0 to 15 |pm|\  0.5, mW
  Input Voltage, , 2.7 to 4.2, V
  Idle Current, ,"11","mA"
  Peak Current,Green Laser ,"370","mA"

.. Battery Estimator
.. -----------------
.. .. raw:: html
..    :file: home.html

Bill of Materials
=================

+-----+------------------------------------+--------------------------------------------------------------+-------------------------------------------------------------+
| Qty | Description                        | Datasheet                                                    | Vendor                                                      |
+=====+====================================+==============================================================+=============================================================+
| 1   | Cerebro PCB                        |                                                              | `OSH Park <https://oshpark.com/shared_projects/o9AxFZcx>`_  |
+-----+------------------------------------+--------------------------------------------------------------+-------------------------------------------------------------+
| 2   | 105 mAh Battery                    | :download:`1570<Datasheets/lipo battery.pdf>`                | `Adafruit <https://www.adafruit.com/products/1570>`_        |
+-----+------------------------------------+--------------------------------------------------------------+-------------------------------------------------------------+
| 1   | Microcontroller                    | :download:`ATTINY84-20SSU<Datasheets/ATtiny84.pdf>`          | `Octopart <https://octopart.com/bom-lookup/biKef905>`_      |
+-----+------------------------------------+--------------------------------------------------------------+                                                             +
| 1   | IR Sensor                          | :download:`TSOP6138TR<Datasheets/TSOP61.pdf>`                |                                                             |
+-----+------------------------------------+--------------------------------------------------------------+                                                             +
| 1   | DAC (12-Bit)                       | :download:`LTC2630ACSC6-LZ12<Datasheets/LTC2630.pdf>`        |                                                             |
+-----+------------------------------------+--------------------------------------------------------------+                                                             +
| 1   | EEPROM IC (64K)                    | :download:`FT24C64A-ULR-T<Datasheets/FT24C64A-ULR-T.pdf>`    |                                                             |
+-----+------------------------------------+--------------------------------------------------------------+                                                             +
| 1   | Op Amp                             | :download:`OPA237NA/3K<Datasheets/OPA237NA_3K.pdf>`          |                                                             |
+-----+------------------------------------+--------------------------------------------------------------+                                                             +
| 1   | NPN Transistor                     | :download:`FJX3904TF<Datasheets/FJX3904TF.pdf>`              |                                                             |
+-----+------------------------------------+--------------------------------------------------------------+                                                             +
| 1   | Boost Converter                    | :download:`MIC2288YD5 TR<Datasheets/MIC2288YD5 TR.pdf>`      |                                                             |
+-----+------------------------------------+--------------------------------------------------------------+                                                             +
| 1   | Slide Switch                       | :download:`AYZ0102AGRLC<Datasheets/AYZ0102AGRLC.pdf>`        |                                                             |
+-----+------------------------------------+--------------------------------------------------------------+                                                             +
| 1   | Momentary Button                   | :download:`B3U-1000P<Datasheets/B3U-1000P.pdf>`              |                                                             |
+-----+------------------------------------+--------------------------------------------------------------+                                                             +
| 1   | Male micro USB vertical plug       | :download:`ZX20-B-5S-UNIT(30)<Datasheets/ZX20-B-5S.pdf>`     |                                                             |
+-----+------------------------------------+--------------------------------------------------------------+                                                             +
| 1   | Male micro USB shielding           | :download:`ZX20-B-SLDC<Datasheets/ZX20-B-5S.pdf>`            |                                                             |
+-----+------------------------------------+--------------------------------------------------------------+                                                             +
| 1   | Female micro USB horizontal socket | :download:`10118193-0001LF<Datasheets/10118193-0001LF.pdf>`  |                                                             |
+-----+------------------------------------+--------------------------------------------------------------+                                                             +
| 1   | Schottky Diode                     | :download:`MBRM140T3G<Datasheets/MBRM140T3G.pdf>`            |                                                             |
+-----+------------------------------------+--------------------------------------------------------------+                                                             +
| 1   | 10uH Inductor                      | :download:`LQH43CN100K03L<Datasheets/LQH43CN100K03L.pdf>`    |                                                             |
+-----+------------------------------------+--------------------------------------------------------------+                                                             +
| 1   | 0603 Red LED                       | :download:`LTST-C191KRKT<Datasheets/LTST-C191KRKT.pdf>`      |                                                             |
+-----+------------------------------------+--------------------------------------------------------------+                                                             +
| 1   | 0603 Amber LED                     | :download:`LNJ437W84RA<Datasheets/LNJ437W84RA_E.pdf>`        |                                                             |
+-----+------------------------------------+--------------------------------------------------------------+                                                             +
| 1   | 0603 10 |mgr|\F Capacitor          |                                                              |                                                             |
+-----+------------------------------------+--------------------------------------------------------------+                                                             +
| 1   | 0603 2.2 |mgr|\F Capacitor         |                                                              |                                                             |
+-----+------------------------------------+--------------------------------------------------------------+                                                             +
| 1   | 0603 6.8 |OHgr| Resistor           |                                                              |                                                             |
+-----+------------------------------------+--------------------------------------------------------------+                                                             +
| 1   | 0603 27 |OHgr| Resistor            |                                                              |                                                             |
+-----+------------------------------------+--------------------------------------------------------------+                                                             +
| 1   | 0603 2 k\ |OHgr| Resistor          |                                                              |                                                             |
+-----+------------------------------------+--------------------------------------------------------------+                                                             +
| 3   | 0603 6.2 k\ |OHgr| Resisor         |                                                              |                                                             |
+-----+------------------------------------+--------------------------------------------------------------+                                                             +
| 2   | 0603 12 k\ |OHgr| Resistor         |                                                              |                                                             |
+-----+------------------------------------+--------------------------------------------------------------+                                                             +
| 3   | 0603 20 k\ |OHgr| Resistor         |                                                              |                                                             |
+-----+------------------------------------+--------------------------------------------------------------+-------------------------------------------------------------+

.. Caution::
  The Adafruit battery linked above has been confirmed to work with Cerebro. Other Li-poly batteries may be used to accommodate different
  weight or session length constraints, just make sure the battery capacity and `C rating <https://learn.adafruit.com/all-about-batteries/power-capacity-and-power-capability>`_
  are capable of handling a peak current of 350 mA.


CAD Files
=========
PCB
	- :download:`Cerebro 4.8.brd<../../../PCB Files/Cerebro 4.8/Cerebro 4.8.brd>`
	- :download:`Cerebro 4.8.sch<../../../PCB Files/Cerebro 4.8/Cerebro 4.8.sch>`
	- :download:`Schematic.pdf<../../../PCB Files/Cerebro 4.8/Cerebro 4.8 schematic.pdf>`


Code
====
  - :download:`Cerebro.ino<../../../Arduino/Cerebro_Sketches/cerebro/cerebro.ino>`

Build Instructions
==================

Required Tools
--------------
  * Soldering iron
  * Tweezers
  * Flux pen
  * Solder wick
  * Wire cutters/strippers
  * Crazy glue
  * Hot glue gun

.. _Flux pen: http://www.amazon.com/Kester-951-Soldering-Low-Solids-No-Clean/dp/B004X4KOWS

PCB Assembly
------------

.. _cerebro upload:

Uploading Firmware
------------------
.. Unless there are updates to the code, you should only need to upload Cerebro firmware once.
.. Hardware parameters such as the Cerebro ID #, paired implant# can be setup wirelessly (link).
.. Waveform parameters such as power level, on time, and off time can be changed wirelessly as well (link)

1. Setup Arduino environment and Cerebro Utility Shield if you haven't already done so. :ref:`utility shield setup`
2. Turn on Cerebro and plug it into the Utility Shield as shown below.

.. figure:: photos/Setup_photos/cerebro_upload.png
    :align: center
    :scale: 15%

3. Open the Arduino Software and select **Tools -> Board: -> Cerebro/Charging Dock/IR Remote (ATtiny84)**

.. figure:: photos/Setup_photos/board_select.png
    :align: center
    :scale: 80%

4. Choose the COM port that your Arduino Uno is connected to by selecting **Tools -> Port: ->**

5. Select **Tools -> Programmer -> Arduino as ISP**

6. Burn the bootloader by selecting **Tools -> Burn Bootloader**

.. figure:: photos/Setup_photos/burn_bootloader.png
    :align: center
    :scale: 80%

7. Choose the firmware that you'd like to upload under **File -> Sketchbook -> Cerebro Sketches ->**

.. figure:: photos/Setup_photos/select_sketch.png
    :align: center
    :scale: 80%

8. Upload the firmware by clicking the upload arrow

.. figure:: photos/Setup_photos/upload.png
    :align: center
    :scale: 100%
