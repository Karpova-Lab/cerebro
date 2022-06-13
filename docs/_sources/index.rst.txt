:github_url: https://github.com/Karpova-Lab/cerebro

.. include:: isogrk1.txt
.. include:: mmlalias.txt

========================================
Cerebro Wireless Optogenetics System v2
========================================

Welcome! This is the hardware and software documentation of a system used for wireless `optogenetic <https://en.wikipedia.org/wiki/Optogenetics>`_ stimulation.
As an `open-source <http://www.oshwa.org/definition/>`_ project, this documentation is intended to provide exhaustive resources and detailed instructions to enable anyone to
study, replicate, share, improve and contribute back to the Cerebro Wireless Optogenetics System. 


This project was developed by Andy Lustig in the `Karpova Lab <https://www.janelia.org/lab/karpova-lab>`_ at HHMI's Janelia Research Campus. If you have questions or comments, you can post them on the `discussions page <https://github.com/Karpova-Lab/cerebro/discussions>`_.

.. Note::
  You are currently viewing documentation for the **radio controlled Version 2 system**. 
  To view documentation for the **infrared controlled Version 1 system**, click the button below.

  .. raw:: html

    <div style="text-align:center; margin-bottom:24px">
      <a href="https://cerebro.readthedocs.io/en/1.0/" style="
      background-color: #6ab0de;
      border: none;
      color: white;
      padding: 10px 10px;
      text-align:center;
      text-decoration: none;
      display: inline-block;
      font-size: 20px;
      font-weight: bold;
      line-height: 1.1;
      border-radius:15px">
      Version 1 (infrared) documentation</a>
    </div>

.. _overview:

Features
========

Wireless Radio
  - **Low Latency** -  Less than 1.5 ms latency between a trigger command being sent and the laser diodes being powered on.
  - **2-Way Communication** - Base Station can send messages to start, extend, or interrupt light stimulation on Cerebro. Cerebro can send acknowledgment messages that commands are received, as well as battery status updates.
  - **Good Signal Range** - At least 20m, doesn't need line of sight. (The radio module that Cerebro uses has been shown to have ranges greater than 500m with the right antennas https://lowpowerlab.com/forum/rf-range-antennas-rfm69-library/rfm69hw-range-test!/)
  - **Wirelessly Adjustable Waveforms** - Cerebro can produce :ref:`customizable waveforms<waveform parameters>` with 1ms resolution. The waveform parameters can be adjusted wirelessly, even while the animal is behaving.

Laser Diodes
  - **High Power** - Laser diodes are more powerful than LEDs. Instead of using an LED coupled to a large fiber, you can use a laser diode coupled to smaller fiber and get equivalent light power output but with reduced brain tissue damage.
  - **Independent Bilateral Control of Intensity** -  Each laser diode has its own control circuitry, enabling you to turn on either diode at different times and/or different intensities. Intensity ramp-downs can be used to prevent post illumination rebounds in neural activity (https://www.cell.com/neuron/pdf/S0896-6273(17)30602-5.pdf).

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


Block Diagram
=============

.. image:: Hardware/photos/Block_diagram/diagram.jpg
  :align: center
  :width: 100 %

System Components
=================
Cerebro
-------
.. image:: Hardware/photos/cerebro.jpg
  :align: center
  :width: 100 %

Cerebro is a rechargable battery powered wireless receiver that connects to a head implant assembly. Cerebro receives commands from a :ref:`base station` and delivers power to
the laser diodes on the head implant, resulting in light being emitted through the optical fibers into the brain. The
intensity and waveform of the emitted light can be adjusted wirelessly during an experiment (:ref:`Edit Waveform Parameters <waveform parameters>`).

:doc:`More info... <Hardware/cerebro>`

Head Implant
------------
.. image:: Hardware/photos/implant_cad.jpg
  :align: center
  :width: 100 %

The head implant assembly consists of two laser diodes coupled to optical fibers. Both laser diodes include an integrated phototranistor that is used as feedback
to maintain a constant light output. The laser diodes can be independently controlled and can output arbitrary waveforms with millisecond resolution.

:doc:`More info... <Hardware/implant>`

.. _base station:

Base Station
------------
.. image:: Hardware/photos/base.jpg
  :align: center
  :width: 100 %

The Base Station is responsible for communicating wirelessly with Cerebro via radio.
It is connected by USB to a Windows computer, where commands can be sent and events can be logged using serial communication with Xavier.
Base Station can also be controlled directly by `TTL <https://learn.sparkfun.com/tutorials/logic-levels#ttl-logic-levels>`_ signals going into
its RJ45 connector. A high TTL signal on pin3 of the RJ45 causes a trigger command to be sent, and a high TTL signal on pin2 causes
a stop command to be sent.

:doc:`More info... <Hardware/base station>`


Charging Dock
-------------
.. image:: Hardware/photos/charging.jpg
  :align: center
  :width: 100 %

The Charging Dock has the same battery connectors as Cerebro and can charge up to 6 Cerebro lipo batteries at once. 

:doc:`More info... <Hardware/charging dock>`

Xavier Software
---------------
.. image:: Software/Xavier.png
  :align: center
  :scale: 100 %

Xavier is a GUI for sending and logging Base Station commands. 
Xavier provides an easy interface for sending parameter changes and can also be used as a debugging tool.

:doc:`More info... <../Software/Xavier>`

Alternative Commerical Systems
==============================

Here are some commercial products that also provide wireless optogenetics:

- `Doric OSE System <https://neuro.doriclenses.com/collections/wireless-recording>`_ 
- `Eicom Teleopto <https://eicomusa.com/teleopto/>`_
- `Neurolux <http://www.neurolux.org/>`_
- `Multichannel Systems <https://www.multichannelsystems.com/products/wireless-systems>`_

Documentation Outline
=====================
.. toctree::
  :caption: Hardware

  Hardware/cerebro.rst
  Hardware/implant.rst
  Hardware/base station.rst
  Hardware/charging dock.rst
  Hardware/programmer.rst

.. toctree::
  :caption: Software

  Software/Xavier.rst