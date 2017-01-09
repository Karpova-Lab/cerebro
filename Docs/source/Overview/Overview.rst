================
System Overview
================
.. include:: isogrk1.txt
.. include:: mmlalias.txt

Features
--------

Open Source = Flexible
  All code and CAD is available so you can make changes and improvements to anything you want!

Low Latency
  Less than 3ms latency between a trigger command being sent and the laser diodes being powered on.

Wirelessly Adjustable Waveforms
  Cerebro can be produce :ref:`customizable waveforms<waveform parameters>` with 1ms resolution. The waveform parameters can be adjusted wirelessly, even while the animal is behaving.

Laser Diodes
  Laser diodes are more powerful than LEDs. Instead of using a LED coupled to a large fiber, you can use a laser diode coupled to smaller fiber and get equivalent light power output but with reduced brain tissue damage.
  Bilateral implants have been capable of up to 15 mW per fiber-coupled laser diode or 450 mW/mm\ :sup:`2` (tested with two 520 nm laser diodes coupled to 200 |mgr|\m diameter fibers and powered by two 105 mAh lipo batteries).

.. Keep in mind the limiting factor of the tested implant wasn't the laser diodes, but rather the peak current supply of the batteries.

.. This means you can substitute a LED coupled to a large fiber, with a laser diode coupled to smaller (less brain damaging) fiber.
.. By coupling high power laser diodes with small fibers, one can get the same amount of light power
.. as LEDs coupled with large fibers, but instead use a high power laser diode with a small fiber. Using smaller fibers results in less brain tissue damage.
..
.. One can get equal light power by coupling high power laser diodes with small fibers, as one

Low Material Cost
  Hardware BOMs are provided, with a total estimated cost of $300. The software is free.

.. csv-table::
  :header: "Subsystem", "Material Cost ($)", "Assembly Time (hours)"
  :widths: 1,1,1

  Cerebro,30 , 1
  Head Implant (bilateral),190,4
  Base Station,5,1
  IR Remote,40,1
  Arduino + Utility Shield,30,1
  Downloader Cord,5,1
  Xavier, FREE, 0
  ,
  **Total**,**$300**,**9 hours**

Commercial Alternatives
-----------------------
Here are some commercial products that also provide wireless optogenetics:

| `Eicom Teleopto <https://eicomusa.com/teleopto/>`_
| `Kendall Research Systems <http://www.kendallresearchsys.com/>`_
| `Neurolux <http://www.neurolux.org/products/>`_
| `Triangle Biosystems <http://www.trianglebiosystems.com/s-series-systems.html>`_
|

.. .. csv-table::
..   :header: "Product", "Cost ($)", "Features"
..   :widths: 1,1,1
..
..   `Eicom Teleopto <https://eicomusa.com/teleopto/>`_ , "4,500", "Very similar to Cerebro. Uses battery and IR triggering."
..   `Kendall Research Systems <http://www.kendallresearchsys.com/>`_ , "20,000", "Uses wireless power. Includes 3-axis accelerometer"
..   `Neurolux <http://www.neurolux.org/products/>`_,
..   `Triangle Biosystems <http://www.trianglebiosystems.com/s-series-systems.html>`_,

Cerebro
-------

.. image:: cerebro.jpg
  :align: center
  :scale: 20 %

Cerebro is a rechargable wireless receiver that connects to a head implant assembly. Cerebro receives commands from a :ref:`base station` and delivers power to
the laser diodes on the head implant, resulting in light being emitted through the optical fibers into the brain. The
intensity and waveform of the emitted light can be adjusted wirelessly during an experiment (:ref:`How to change Pulse Parameters <pulse change header>`).
All commands sent to Cerebro are saved along with timestamps to onboard non-volatile memory.

:doc:`More info... <../Hardware/cerebro>`

Head Implant
------------

.. image:: implant.jpg
  :align: center
  :scale: 10 %

The head implant assembly consists of two laser diodes coupled to optical fibers. Also included is a photoresistor that is used as feedback
to maintain a constant light output.

:doc:`More info... <../Hardware/implant>`

.. _base station:

Base Station
------------
.. image:: base_station.jpg
  :align: center
  :scale: 15 %

The Base Station is responsible for emitting infrared light signals to communicate wirelessly with Cerebro (exactly like a TV remote).
It is connected by USB to a Windows computer, where commands can be sent and events can be logged using :ref:`Xavier <xavier>`.
Base Station can also be controlled directly by `TTL <https://learn.sparkfun.com/tutorials/logic-levels#ttl-logic-levels>`_ signals going into
its RJ45 connector. A high TTL signal on pin3 of the RJ45 causes a trigger signal to be emitted, and a high TTL signal on pin2 causes
a stop signal to be emitted. Events will still be logged by Xavier even when triggered by a TTL.

:doc:`More info... <../Hardware/implant>`


.. _utility shield:

Utility Shield
--------------
.. image:: utility_shield.jpg
  :align: center
  :scale: 15 %

The Utility Shield connects to an `Arduino Uno <https://www.arduino.cc/en/main/arduinoBoardUno>`_ and is used to flash programs to a variety of AVR microcontrollers.
It can flash the ATtiny84 DIP used by the IR Remote and Charging Dock, the ATtiny84 SOIC used by Cerebro, the ATtiny85 DIP used by the Base Station, and the ATmega328P
used by the IR Remote. The shield also provides a charging port for Cerebro that has a green/red charging indicator light.

:doc:`More info... <../Hardware/utility shield>`

Charging Dock
-------------
.. image:: charging_dock.jpg
  :align: center
  :scale: 20 %

The Charging Dock provides ports for charging up to 4 Cerebro receivers at one time.
An OLED display shows the voltages of the batteries being charged.
A red indicator LED is lit while Cerebro is charging, and turns off once Cerebro is fully charged.

:doc:`More info... <../Hardware/charging dock>`

IR Remote
---------

.. image:: IR_remote.jpg
  :align: center
  :scale: 20 %

The IR Remote is a handheld remote that can be used to send commands to Cerebro.
Information including battery status, current parameters, and ID number can be checked by plugging a Cerebro Receiver into the IR Remote.

:doc:`More info... <../Hardware/remote>`

.. _cord:

Downloader Cord
---------------
.. image:: downloader.jpg
  :align: center
  :scale: 15 %

The Downloader Cord is used for serial communication between a computer and Cerebro. Once connected to Cerebro's
micro usb port, it can be used for downloading log data off of Cerebro's EEPROM, or it can be used to display
messages when Cerebro is in debugging mode.

:doc:`More info... <../Hardware/downloader cord>`

.. _xavier:

Xavier
------

.. image:: xavier_demo.png
    :align: center
    :scale: 80 %

Xavier is a GUI for sending and logging Base Station commands. Since communication between Cerebro and Base Station is unidirectional, in order to truly know what occurs throughout an experiment,
every event that is received by Cerebro is also saved to an onboard log.  After an experiment is complete, Cerebro's onboard logfile can be
downloaded into Xavier. The two log files can then be saved and analyzed to confirm that everything that was sent by Base Station was in fact received and executed by Cerebro.
Xavier provides an easy interface for sending parameter changes and can also be used a debugging tool.

:doc:`More info... <../Software/Xavier>`
