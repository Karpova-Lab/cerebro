============
Head Implant
============
.. include:: isogrk1.txt
.. include:: mmlalias.txt
.. include:: isopub.txt

.. _implantTools:

Required Tools
==============
  * Ruler
  * Soldering iron
  * Tweezers
  * `Fiber stripper <https://www.thorlabs.com/thorProduct.cfm?partNumber=T12S21>`_
  * `Fiber cleaver <http://www.fiberinstrumentsales.com/fis-lynx-precision-cleaver-with-fiber-basket.html>`_
  * `3-axis fiber launch system <http://www.thorlabs.com/thorproduct.cfm?partnumber=MBT612D/M>`_
  * `Power meter <https://www.thorlabs.com/thorproduct.cfm?partnumber=PM121D>`_ and `photodiode sensor <https://www.thorlabs.com/thorproduct.cfm?partnumber=S121C>`_
  * `Ultraviolet light adhesive curing gun <https://www.norlandprod.com/lightsourc.html#led200>`_

Bill of Materials
=================
+-----+----------------------------------+-----------------------------------------------------------------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| Qty | Description                      | Datasheet                                                       | Vendor                                                                                                                                                             |
+=====+==================================+=================================================================+====================================================================================================================================================================+
| 1   | Implant PCB                      |                                                                 | `OSH Park <https://oshpark.com/shared_projects/5K1XYmmy>`_                                                                                                         |
+-----+----------------------------------+-----------------------------------------------------------------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| 2   | Laser Diodes                     | :download:`PLT5 520<Datasheets/PLT5 520.pdf>`                   | `World Star Tech <http://worldstartech.com/products/description/25/index>`_                                                                                        |
+-----+----------------------------------+-----------------------------------------------------------------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| 1   | Fiber                            | :download:`FP200URT<Datasheets/FP200URT.pdf>`                   | `ThorLabs <https://www.thorlabs.com/thorproduct.cfm?partnumber=FP200URT>`_                                                                                         |
+-----+----------------------------------+-----------------------------------------------------------------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| 1   | Norland Optical Adhesive 68      | `NOA 68 <https://www.norlandprod.com/adhesives/noa%2068.html>`_ | `Norland Products <https://www.norlandproducts2.com/adhesives/adproductsdetail_header_removed.asp?Prdid=68>`_                                                      |
+-----+----------------------------------+-----------------------------------------------------------------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| 2   | Ferrule                          | :download:`MM-FER2007C-2500<Datasheets/MM-FER2007C-2500.pdf>`   | `Precision Fiber Products <http://www.precisionfiberproducts.com/pfp-lc-1-25mm-od-multimode-ceramic-zirconia-ferrules/>`_                                          |
+-----+----------------------------------+-----------------------------------------------------------------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| 1   | Epoxy                            | :download:`T120-023-CX<Datasheets/T120-023-CX.pdf>`             | `FIS <http://www.fiberinstrumentsales.com/fis-room-cure-epoxy-2-grams.html>`_                                                                                      |
+-----+----------------------------------+-----------------------------------------------------------------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| 1   | Liquid electric tape             |                                                                 | `Amazon <http://www.amazon.com/Star-brite-Liquid-Electrical-Tape/dp/B000QUW4EE/ref=sr_1_1?ie=UTF8&qid=1463746324&sr=8-1&keywords=electrical+tape+liquid+tube>`_    |
+-----+----------------------------------+-----------------------------------------------------------------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| 1   | Photocell                        | :download:`PDV-P9005-1<Datasheets/PDV-P9005-1.pdf>`             | `Octopart <https://octopart.com/bom-lookup/HBO7aiJs>`_                                                                                                             |
+-----+----------------------------------+-----------------------------------------------------------------+                                                                                                                                                                    +
| 1   | Female micro USB vertical socket | :download:`105133-001<Datasheets/105133-001.pdf>`               |                                                                                                                                                                    |
+-----+----------------------------------+-----------------------------------------------------------------+                                                                                                                                                                    +
| 1   | 0805 6.8 k\ |OHgr| Resistor      |                                                                 |                                                                                                                                                                    |
+-----+----------------------------------+-----------------------------------------------------------------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------+

Assembly Instructions
=====================
#. Strip fiber and cut to length
#. Align fiber to laser diode
#. Apply Norland Optical Adhesive 68 and cure it with UV adhesive curing gun
#. Laser cut fiber guide
#. Sand down ferrule to width of acrylic
#. Insert ferrules into acrylic to create fiber guide
#. Thread coupled fibers through fiber guide
#. Surround assembly with epoxy
#. Place photocell in epoxy
#. Cover photocell/epoxy with liquid electrical tape
#. Solder laser diodes onto implant PCB


Characterization
================
We want to know and be able to control the amount of light that we deliver to the brain during experiments.
The light power produced by the laser diodes is a function of current and case temperature:

.. figure:: photos/Calibration_photos/laserDiodeResponse.png
  :align: center
  :scale: 30%

Cerebro has precise control of the current being delivered to the laser diode, and can monitor the voltage across the photocell.
We can use a feeback loop to maintain a constant photocell voltage, and therefore a constant fiber tip light power output
(assuming the light power at the fiber tip is proportional to the light hitting the photocell).

If the photocell had a linear response to light power, we could just make 1 measurement of the light output at a known photocell voltage
and find a proportionality constant. However, the photocell doesn't have a linear response:

.. figure:: photos/Calibration_photos/photcellResponse.png
  :align: center
  :scale: 25%

We therefore need to measure the light output at the fiber tip across a range of known photocell voltages. Overall, we are trying to
characterize the relationship between photocell voltage and fiber tip light power, resulting in a graph like the following:

.. figure:: photos/Calibration_photos/characterization.png
  :align: center
  :scale: 100%


Gathering Data
``````````````
1. Connect Cerebro to the Head Implant, and secure them in a test fixture with the fibers pointing into a :ref:`Light Power Meter<implantTools>`

.. figure:: photos/Calibration_photos/fixture.jpg
  :align: center
  :scale: 15%

2. Open up the `Thorlabs' Power Meter Software <https://www.thorlabs.com/software_pages/ViewSoftwarePage.cfm?Code=PM100x>`_ and change the following settings:

  - Range: 120mW
  - Wave: 520nm
  - Log Configuration

    - No of Samples: 150000
    - Averaging: 1
    - Fast logging to file: \ |check|

.. figure:: photos/Calibration_photos/settings_combined.png
  :align: center
  :scale: 100%

3. Open up Xavier and start a new session.  :ref:`How to start a session<start session>`
4. In the "Cerebro's Waveform Parameters" section, click ``Change Power Level``. A "Change Power Level" window will appear.

.. figure:: photos/Calibration_photos/openCalDialog.png
  :align: center
  :scale: 50%

5. Make sure Cerbero is turned on and then click ``Start Calibration Routine`` in the new window.
Cerebro's amber light will turn on to indicate that the calibration routine has begun.
Within 10 seconds, click ``Start Log`` in the Light Power Meter software to begin recording light power data.

.. figure:: photos/Calibration_photos/sendCalTrigger.png
  :align: center
  :scale: 50%

6. After about 15 minutes, the data will have been collected.


Design Files
============
PCB
	- :download:`Implant 4.4.brd<../../../PCB Files/Implant 4.4/Implant 4.4.brd>`
	- :download:`Implant 4.4.sch<../../../PCB Files/Implant 4.4/Implant 4.4.sch>`
	- :download:`Schematic.pdf<../../../PCB Files/Implant 4.4/Implant 4.4 schematic.pdf>`

Lasercut
  - :download:`Fiber guide_1,5mm gap.dxf<../../../Lasercut/Fiber guide/Fiber guide_1,5mm gap.dxf>`
  - :download:`Fiber guide_1,5mm gap.ai<../../../Lasercut/Fiber guide/Fiber guide_1,5mm gap.ai>`
