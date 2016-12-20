============
Head Implant
============
.. include:: isogrk1.txt
.. include:: mmlalias.txt
.. include:: isopub.txt

.. figure:: photos/implant_cad.png
  :align: center
  :scale: 100%

.. csv-table::
  :header: "Item", "Description"
  :widths: 1,30

  1, "Micro USB Socket"
  2, "Printed Circuit Board (PCB)"
  3, "Laser Diode"
  4, "Acrylic ferrule holder"
  4, "Ferrule"
  5, "Optical Fiber"


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
| 2   | Laser Diodes                     | :download:`PL 520_B1<Datasheets/PL520_B1.pdf>`                  | `World Star Tech <http://worldstartech.com/products/description/25/index>`_                                                                                        |
+-----+----------------------------------+-----------------------------------------------------------------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| 1   | Optical Fiber                    | :download:`FP200URT<Datasheets/FP200URT.pdf>`                   | `ThorLabs <https://www.thorlabs.com/thorproduct.cfm?partnumber=FP200URT>`_                                                                                         |
+-----+----------------------------------+-----------------------------------------------------------------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| 1   | Norland Optical Adhesive 68      | `NOA 68 <https://www.norlandprod.com/adhesives/noa%2068.html>`_ | `Norland Products <https://www.norlandproducts2.com/adhesives/adproductsdetail_header_removed.asp?Prdid=68>`_                                                      |
+-----+----------------------------------+-----------------------------------------------------------------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| 2   | Ferrule                          | :download:`MM-FER2007C-2500<Datasheets/MM-FER2007C-2500.pdf>`   | `Precision Fiber Products <http://www.precisionfiberproducts.com/pfp-lc-1-25mm-od-multimode-ceramic-zirconia-ferrules/>`_                                          |
+-----+----------------------------------+-----------------------------------------------------------------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| 1   | Epoxy                            | :download:`T120-023-CX<Datasheets/T120-023-CX.pdf>`             | `FIS <http://www.fiberinstrumentsales.com/fis-room-cure-epoxy-2-grams.html>`_                                                                                      |
+-----+----------------------------------+-----------------------------------------------------------------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| 1   | Liquid electric tape             |                                                                 | `Amazon <http://www.amazon.com/Star-brite-Liquid-Electrical-Tape/dp/B000QUW4EE/ref=sr_1_1?ie=UTF8&qid=1463746324&sr=8-1&keywords=electrical+tape+liquid+tube>`_    |
+-----+----------------------------------+-----------------------------------------------------------------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| 1   | Photoresistor                    | :download:`PDV-P9005-1<Datasheets/PDV-P9005-1.pdf>`             | `Octopart <https://octopart.com/bom-lookup/HBO7aiJs>`_                                                                                                             |
+-----+----------------------------------+-----------------------------------------------------------------+                                                                                                                                                                    +
| 1   | Female micro USB vertical socket | :download:`105133-001<Datasheets/105133-001.pdf>`               |                                                                                                                                                                    |
+-----+----------------------------------+-----------------------------------------------------------------+                                                                                                                                                                    +
| 1   | 0805 6.8 k\ |OHgr| Resistor      |                                                                 |                                                                                                                                                                    |
+-----+----------------------------------+-----------------------------------------------------------------+--------------------------------------------------------------------------------------------------------------------------------------------------------------------+

Design Files
============
PCB
	- :download:`Implant 4.4.brd<../../../PCB Files/Implant 4.4/Implant 4.4.brd>`
	- :download:`Implant 4.4.sch<../../../PCB Files/Implant 4.4/Implant 4.4.sch>`
	- :download:`Schematic.pdf<../../../PCB Files/Implant 4.4/Implant 4.4 schematic.pdf>`

Lasercut
  - :download:`Fiber guide_1,5mm gap.dxf<../../../Lasercut/Fiber guide/Fiber guide_1,5mm gap.dxf>`
  - :download:`Fiber guide_1,5mm gap.ai<../../../Lasercut/Fiber guide/Fiber guide_1,5mm gap.ai>`

Assembly Instructions
=====================
#. Strip fiber and cut to length
#. Align fiber to laser diode
#. Apply Norland Optical Adhesive 68 and cure it with UV adhesive curing gun
#. Laser cut fiber guide
#. Insert ferrules into acrylic to create fiber guide
#. Thread coupled fibers through fiber guide
#. Surround assembly with epoxy
#. Place photoresistor in epoxy
#. Cover photoresistor/epoxy with liquid electrical tape
#. Solder laser diodes onto implant PCB

.. _diode char:

Diode Characterization and Pairing (for bilateral implants only)
================================================================
Laser diodes are are not perfectly manufactured to have identical electrical characteristics.
At a given current and voltage, there is some variation in light output from laser diode to laser diode.
This can cause problems when making a bilateral implant because the current is split evenly between the two laser diodes.
If the two laser diodes have drastically different electrical characteristics, this will result in one diode being brighter than the other.
To try and prevent this unbalanced light ouput, we attempt to characterize each individual laser diode, and pair similar diodes together in each bilateral implant.

Instructions here.

.. 1. Make sure the :ref:`cord` is plugged into the computer
.. 2. Select the appropriate Serial Port and click ``Connect to Downloader``
..
.. .. image:: ../Software/photos/End_session/connect_downloader.png
..   :align: center
..   :scale: 100%
..
.. 3. Plug the :ref:`cord` into Cerebro's micro usb port
.. 4. Connect :ref:`characterizer module<>` to Cerebro
.. #. Connect the laser diode to the power wires and insert into the characterizer module's hole
.. #. Use Xavier or an IR remote to start the diode characterization routine
.. #. Copy the Cerebro Monitor output into an excel file
.. #. Repeat for each diode
.. #. Create a graph to compare response curves. You can us plot.ly to edit the example graph below with your own values.
..
.. .. raw:: html
..
..   <iframe width="900" height="500" frameborder="0" scrolling="no" style="max-width:100%"src="https://plot.ly/~AndyLustig/24.embed"></iframe>
..
.. The y-axis is the DAC input which is directly proportional to current being provided to the laser diodes.
.. The x-axis is the photoresistor voltage which corresponds to the light power that the laser diodes are outputting.
.. From this example graph we can see that laser diode A10 is weaker than A11 because it is clearly above it at all points,
.. meaning at each power level more current is required for the same level of light output.
.. In this case we would likely pair A10 with A4 and A11 with A6.

.. _implant char:

Implant Characterization
========================
We want to know and be able to control the amount of light that we deliver to the brain during experiments.
The light power produced by the laser diodes is a function of current and case temperature:

.. figure:: photos/Calibration_photos/laserDiodeResponse.png
  :align: center
  :scale: 30%

Cerebro has precise control of the current being delivered to the laser diode, and can monitor the voltage across the photoresistor.
We can use a feeback loop to maintain a constant photoresistor voltage, and therefore a constant fiber tip light power output
(assuming the light power at the fiber tip is proportional to the light hitting the photoresistor).

If the photoresistor had a linear response to light power, we could just make 1 measurement of the light output at a known photoresistor voltage
and find a proportionality constant. However, the photoresistor doesn't have a linear response:

.. figure:: photos/Calibration_photos/photcellResponse.png
  :align: center
  :scale: 25%

We therefore need to measure the light output at the fiber tip across a range of known photoresistor voltages. Overall, we are trying to
characterize the relationship between photoresistor voltage and fiber tip light power, resulting in a graph like the following:

.. figure:: photos/Calibration_photos/characterization.png
  :align: center
  :scale: 100%

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

3. Make sure Cerbero is turned on.
4. Open up Xavier and start a new session.  :ref:`How to start a session<start session>`
5. In the "Cerebro's Construction Tools" section click ``Start Implant Characterization``. Cerebro's amber light will turn on to indicate that the characterization routine has begun.

.. figure:: photos/Calibration_photos/sendCalTrigger.png
  :align: center
  :scale: 100%

6. Within 10 seconds, click ``Start Log`` in the Light Power Meter software to begin recording light power data.
7. After about 15 minutes, the data will have been collected.
8. When you are ready to setup a Cerebro for use with this implant, follow the instructions for :ref:`sending power values to Cerebro<send power>`
