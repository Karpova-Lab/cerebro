:github_url: https://github.com/Karpova-Lab/cerebro/tree/1.0
=============
Charging Dock
=============
.. include:: isogrk1.txt
.. include:: mmlalias.txt

.. image:: ../Overview/charging_dock.jpg
  :align: center
  :scale: 20 %

Required Tools
==============
  * Soldering iron

Bill of Materials
=================

+-----+------------------------------------+-------------------------------------------------------------+--------------------------------------------------------------+
| Qty | Description                        | Datasheet                                                   | Vendor                                                       |
+=====+====================================+=============================================================+==============================================================+
| 1   | Charging Dock PCB                  |                                                             | `OSH Park <https://oshpark.com/shared_projects/796c3lzQ>`_   |
+-----+------------------------------------+-------------------------------------------------------------+--------------------------------------------------------------+
| 1   | OLED display                       |                                                             | `Adafruit <https://www.adafruit.com/products/938>`_          |
+-----+------------------------------------+-------------------------------------------------------------+--------------------------------------------------------------+
| 1   | Microcontroller                    | :download:`ATMEGA328P-AU<Datasheets/ATMEGA328P-AU.pdf>`     | `Octopart <https://octopart.com/bom-lookup/sGmVuHz7>`_       |
+-----+------------------------------------+-------------------------------------------------------------+                                                              |
| 1   | 16 MHz Resonator                   | :download:`CSTLS16M0X53-A0<Datasheets/CSTLS16M0X53-A0.pdf>` |                                                              |
+-----+------------------------------------+-------------------------------------------------------------+                                                              |
| 4   | Multiplexer                        | :download:`MAX4674<Datasheets/MAX4674.pdf>`                 |                                                              |
+-----+------------------------------------+-------------------------------------------------------------+                                                              |
| 4   | 0805 Red LED                       | :download:`LTST-C171KRKT<Datasheets/LTST-C171KRKT.pdf>`     |                                                              |
+-----+------------------------------------+-------------------------------------------------------------+                                                              |
| 1   | Momentary Button                   | :download:`B3U-1100P<Datasheets/B3U-1000P.pdf>`             |                                                              |
+-----+------------------------------------+-------------------------------------------------------------+                                                              |
| 4   | Female micro USB vertical socket   | :download:`1051330011<Datasheets/1051330011.pdf>`           |                                                              |
+-----+------------------------------------+-------------------------------------------------------------+                                                              |
| 1   | Female micro USB horizontal socket | :download:`10118193-0001LF<Datasheets/10118193-0001LF.pdf>` |                                                              |
+-----+------------------------------------+-------------------------------------------------------------+                                                              |
| 4   | Lipo charger IC                    | :download:`MCP73831T<Datasheets/MCP73831T.pdf>`             |                                                              |
+-----+------------------------------------+-------------------------------------------------------------+                                                              |
| 1   | 0805 10 k\ |OHgr| Resistor         |                                                             |                                                              |
+-----+------------------------------------+-------------------------------------------------------------+                                                              |
| 4   | 0805 27 k\ |OHgr| Resistor         |                                                             |                                                              |
+-----+------------------------------------+-------------------------------------------------------------+                                                              |
| 8   | 0805 100 |OHgr| Resistor           |                                                             |                                                              |
+-----+------------------------------------+-------------------------------------------------------------+                                                              |
| 9   | 0805 10 |mgr|\F Capacitor          |                                                             |                                                              |
+-----+------------------------------------+-------------------------------------------------------------+--------------------------------------------------------------+

Design Files
============
PCB
	- :download:`Charging Dock 1.8.brd<../../../PCB Files/Charging Dock 1.8/Charging Dock 1.8.brd>`
	- :download:`Charging Dock 1.8.sch<../../../PCB Files/Charging Dock 1.8/Charging Dock 1.8.sch>`
	- :download:`Schematic.pdf<../../../PCB Files/Charging Dock 1.8/Charging Dock 1.8 schematic.pdf>`

Code
====
  - :download:`chargingDock.ino<../../../Arduino/Cerebro_Sketches/chargingDock/chargingDock.ino>`

Build Instructions
==================

.. _dock upload:

Uploading Firmware
------------------
.. Attention::
  If you have not yet setup the Arduino environment or the Cerebro Utility Shield, refer to :ref:`arduino setup` and/or :ref:`utility shield setup` before moving on.

.. todo::
	How to upload firmware
