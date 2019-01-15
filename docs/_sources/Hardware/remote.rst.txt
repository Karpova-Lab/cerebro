:github_url: https://github.com/Karpova-Lab/cerebro/tree/1.0

=========
IR Remote
=========
.. include:: isogrk1.txt

.. image:: ../Overview/IR_remote.jpg
	:align: center
	:width: 100 %

Required Tools
==============

Bill of Materials
=================
.. Tip::
	The pins on the vertical USB plug are very short and fail to protrude from standard thickness (~1.6mm) PCBS. Therefore I recommend ordering thinner boards (~0.8mm) to make soldering the connector easier.

+-----+------------------------------------+-------------------------------------------------------------------+-------------------------------------------------------------+
| Qty | Description                        | Datasheet                                                         | Vendor                                                      |
+=====+====================================+===================================================================+=============================================================+
| 1   | IR Remote PCB                      |                                                                   | `OSH Park <https://oshpark.com/shared_projects/uwKQoRvr>`_  |
+-----+------------------------------------+-------------------------------------------------------------------+-------------------------------------------------------------+
| 1   | Rotary encoder                     | :download:`A14-LC-TT<Datasheets/rotary_encoder.pdf>`              | `Sparkfun <https://www.sparkfun.com/products/10982>`_       |
+-----+------------------------------------+-------------------------------------------------------------------+-------------------------------------------------------------+
| 1   | OLED display                       |                                                                   | `Adafruit <https://www.adafruit.com/products/938>`_         |
+-----+------------------------------------+-------------------------------------------------------------------+-------------------------------------------------------------+
| 1   | ATmega328                          | :download:`ATMEGA328P-AU<Datasheets/ATMEGA328P-AU.pdf>`           | `Octopart <https://octopart.com/bom-lookup/KZkJVKbQ>`_      |
+-----+------------------------------------+-------------------------------------------------------------------+                                                             |
| 1   | 16 MHz Resonator                   | :download:`CSTLS16M0X53-A0<Datasheets/CSTLS16M0X53-A0.pdf>`       |                                                             |
+-----+------------------------------------+-------------------------------------------------------------------+                                                             |
| 1   | Male micro USB vertical plug       | :download:`ZX20-B-5S<Datasheets/ZX20-B-5S.pdf>`                   |                                                             |
+-----+------------------------------------+-------------------------------------------------------------------+                                                             |
| 1   | Male micro USB shielding           | :download:`ZX20-B-SLDC<Datasheets/ZX20-B-5S.pdf>`                 |                                                             |
+-----+------------------------------------+-------------------------------------------------------------------+                                                             |
| 1   | Female micro USB horizontal socket | :download:`10118193-0001LF<Datasheets/10118193-0001LF.pdf>`       |                                                             |
+-----+------------------------------------+-------------------------------------------------------------------+                                                             |
| 1   | Toggle switch                      | :download:`ATE1D-2M3-10-Z<Datasheets/ATE1D-2M3-10-Z.pdf>`         |                                                             |
+-----+------------------------------------+-------------------------------------------------------------------+                                                             |
| 1   | IR LED                             | :download:`OED-EL-1L2<Datasheets/OED-EL-1L2.pdf>`                 |                                                             |
+-----+------------------------------------+-------------------------------------------------------------------+                                                             |
| 1   | JST battery connector              | :download:`S2B-PH-K-S(LF)(SN)<Datasheets/S2B-PH-K-S(LF)(SN).pdf>` |                                                             |
+-----+------------------------------------+-------------------------------------------------------------------+-------------------------------------------------------------+

Design Files
============

PCB
	- :download:`IR remote 2.1.brd<../../../PCB Files/IR remote 2.1/IR remote 2.1.brd>`
	- :download:`IR remote 2.1.sch<../../../PCB Files/IR remote 2.1/IR remote 2.1.sch>`
	- :download:`Schematic.pdf<../../../PCB Files/IR remote 2.1/IR remote 2.1 schematic.pdf>`

Build Instructions
==================

.. _remote upload:

Uploading Firmware
------------------

.. Attention::
	If you have not yet setup the Arduino environment or the Cerebro Utility Shield, refer to :ref:`arduino setup` and/or :ref:`utility shield setup` before moving on.

1. With the battery connected, switch the remote on
2. Plug the remote into the Utility Shield via the micro usb connection

.. figure:: photos/remote_connect2shield.jpg
    :align: center
    :scale: 18%

3. Open up the Arduino application and make the following selections under the **Tools** menu:

| **Board:** "Arduino/Genuino Uno"
| **Port:** "COMXX (Arduino/Genuino Uno)"
| **Programmer**: "Arduino as ISP"
|

.. figure:: photos/tools_atmega328.png
    :align: center
    :scale: 100%

4. Burn the bootloader by selecting **Tools -> Burn Bootloader**

.. figure:: photos/remote_bootloader.png
    :align: center
    :scale: 100%

5. Select the firmware that will be uploaded onto the IR Remote **File -> Sketchbook -> Cerebro Sketches -> irRemote**

.. figure:: photos/remote_firmware.png
    :align: center
    :scale: 100%

6. Upload the firmware by selecting **Sketch --> Upload Using Programmer**

.. figure:: photos/upload_using_programmer.png
    :align: center
    :scale: 100%


User Guide
==========

Demonstration Video
-------------------

.. raw:: html

	<iframe src="https://player.vimeo.com/video/175858239?title=0&byline=0&portrait=0" width="640" height="360" frameborder="0" webkitallowfullscreen mozallowfullscreen allowfullscreen></iframe>

.. _remote check:

Checking Cerebro's Waveform Parameters
--------------------------------------

#. Turn on the IR Remote
#. With Cerebro turned off, plug it into the IR Remote
#. Switch on Cerebro. The current waveform parameters as well as the battery voltage will be displayed.

.. _remote send:

Editing and Sending Waveform Parameters
---------------------------------------

#. Use the rotary knob to highlight the parameter that you would like to edit
#. Click down the rotary knob to begin editing the parameters
#. Rotate the knob to make coarse adjustments (clockwise to increase, counterclockwise to decrease)
#. Press, hold and rotate to make fine adjustments.
#. Click the rotary knob again to return to highlighting parameters
#. When finished editing, Press and hold the "B" button to send the new parameters to Cerebro. The display will show "Parameters Sent"

Triggering a light pulse
------------------------

#. Press and release the "A" button. The display will show "Trigger Sent"

Stopping a light pulse
----------------------

#. Press and release the "B" button. The display will show "Stop Sent"

Starting a Calibration Routine
------------------------------

#. Press and hold the "A" button. The display will show "Calibrate Sent"
#. The yellow light on Cerebro will turn on, indicating that it has begun its calibration routine
