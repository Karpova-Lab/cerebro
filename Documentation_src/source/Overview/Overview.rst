================
System Overview
================
.. include:: isogrk1.txt
.. include:: mmlalias.txt

Features
========
Open Source
  * **Flexible** - All code and CAD is available so you can make changes and improvements to anything you want!
  * **Low Cost** - Hardware BOMs are provided, with a total estimated material cost of ~$200 (~$40 per Cerebro, ~$120 per bilateral head implant, ~$40 per Base Station). The software is free.
..    .. csv-table::
..      :header: "Subsystem", "Material Cost ($)", "Assembly Time (hours)"
..      :widths: 1,1,1
..    
..      Cerebro,40 , 2
..      Head Implant (bilateral),120,4
..      Base Station,40,.5
..      Xavier, FREE, 0
..      **Total**,**$145**,**6.5 hours**

Wireless Radio
  * **Low Latency** -  Less than 1.5 ms latency between a trigger command being sent and the laser diodes being powered on.
  * **2-Way Communication** - Base Station can send messages to start, extend, or interrupt light stimulation on Cerebro. Cerebro can send acknowledgment messages that commands are received, as well as battery status updates.
  * **Range** - {x}+ range. Doesn't need line of sight.
  * **Wirelessly Adjustable Waveforms** - Cerebro can be produce :ref:`customizable waveforms<waveform parameters>` with 1ms resolution. The waveform parameters can be adjusted wirelessly, even while the animal is behaving.

Laser Diodes
  * **High Power** - Laser diodes are more powerful than LEDs. Instead of using a LED coupled to a large fiber, you can use a laser diode coupled to smaller fiber and get equivalent light power output but with reduced brain tissue damage.
  * **Independent Bilateral Control** -  Each laser diode has its own control circuitry. 

.. **Technical Specifications**
.. 
.. .. csv-table::
..   :header: "Parameter", "Test Condition", "Value", "Unit"
..   :widths: 3,3,3,3
.. 
..   Weight,,13.2, g
..   Dimensions, Receiver, 0.68 x 0.74 x 0.062 , in
..             , Implant, 1.05 x 0.40 x 0.75, in
..   :download:`Battery Life <photos/battery new.html>`, "520nm, 8mW 3s pulse, 6 pulses/min ", 4.25 (1700), "hours (pulses)"
..   Recharge time, ,1 ,hour
..   Light Wavelength, ":download:`PLT5 520<Datasheets/PL520_B1.pdf>` (Green)", "520","nm"
..   :download:`Trigger Latency <photos/Signal_Breakdown.png>`, , 3.5 |pm|\  0.1, ms
..   Rise Time, , 10 , ms
..   Fiber Diameter, , 200,|mgr|\m
..   Light Output (per fiber) ,520 nm diode coupled to 200 |mgr|\m fiber, 0 to 15 |pm|\  0.5, mW
..   Input Voltage, , 3.5 to 4.2, V
..   Idle Current, ,"28","mA"

Commercial Alternatives
=======================
Here are some commercial products that also provide wireless optogenetics:

| `Eicom Teleopto <https://eicomusa.com/teleopto/>`_
| `Kendall Research Systems <http://www.kendallresearchsys.com/>`_
| `Neurolux <http://www.neurolux.org/products/>`_
| `Triangle Biosystems <http://www.trianglebiosystems.com/s-series-systems.html>`_
|

System Components
=================

Cerebro
-------
.. image:: cerebro.jpg
  :align: center
  :scale: 20 %

Cerebro is a rechargable wireless receiver that connects to a head implant assembly. Cerebro receives commands from a :ref:`base station` and delivers power to
the laser diodes on the head implant, resulting in light being emitted through the optical fibers into the brain. The
intensity and waveform of the emitted light can be adjusted wirelessly during an experiment (:ref:`Edit Waveform Parameters <waveform parameters>`).

:doc:`More info... <../Hardware/cerebro>`

Head Implant
------------
.. .. image:: implant.jpg
..   :align: center
..   :scale: 20 %

The head implant assembly consists of two laser diodes coupled to optical fibers. Both laser diodes are accompanied by a phototranistor that is used as feedback
to maintain a constant light output. The laser diodes can be independently controlled and can output arbitrary waveforms with millisecond resolution.

:doc:`More info... <../Hardware/implant>`

.. _base station:

Base Station
------------
.. image:: base.jpg
  :align: center
  :scale: 50 %

The Base Station is responsible for communicating wirelessly with Cerebro via radio.
It is connected by USB to a Windows computer, where commands can be sent and events can be logged using serial communication with Xavier.
Base Station can also be controlled directly by `TTL <https://learn.sparkfun.com/tutorials/logic-levels#ttl-logic-levels>`_ signals going into
its RJ45 connector. A high TTL signal on pin3 of the RJ45 causes a trigger signal to be emitted, and a high TTL signal on pin2 causes
a stop signal to be emitted.

:doc:`More info... <../Hardware/base station>`


Charging Dock
-------------

The Charging dock has the battery connectors as Cerebro and can charge up to 6 Cerebro batteries at once.

:doc:`More info... <../Hardware/charging dock>`

Xavier
------
.. .. image:: xavier_demo.png
..     :align: center
..     :scale: 80 %

Xavier is a GUI for sending and logging Base Station commands. 
Xavier provides an easy interface for sending parameter changes and can also be used a debugging tool.

:doc:`More info... <../Software/Xavier>`
