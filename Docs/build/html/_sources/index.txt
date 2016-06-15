Cerebro Wireless Optogenetics System
====================================
.. include:: isogrk1.txt

Welcome! This is the hardware and software documentation of a system used for wireless `optogenetic <https://en.wikipedia.org/wiki/Optogenetics>`_ stimulation.
As an `open-source <http://www.oshwa.org/definition/>`_ project, this documentation is intended to provide exhaustive resources and detailed instructions to enable anyone to
study, replicate, share, improve and contribute back to the Cerebro Wireless Optogenetics System.

This project was developed by Andy Lustig in the `Karpova Lab at Janelia Research Campus <https://www.janelia.org/lab/karpova-lab>`_.
If you have any questions you can contact him at lustiga@janelia.hhmi.org

.. Note::
  This documentation is very much a work in progress. Please understand that there will porbably be typos and missing

System Highlights
-----------------
Open Source = Flexible
  All code and CAD is available so you can make changes and improvements to anything you want!

Low Latency
  Less than 3ms latency between a trigger command being sent and the laser diodes being powered on.

Wirelessly Adjustable aveforms
  Cerebro can be produce :ref:`customizable waveforms<waveform parameters>` with 1ms resolution. The waveform parameters can be adjusted wirelessly, even while the animal is behaving.

High Power Laser Diodes
  Up to 15 mW per laser diode or 450 mW/mm\ :sup:`2` (tested with 520 nm laser diodes coupled to 200 |mgr|\m fibers)

Low Material Cost
  Hardware BOMs are provided, at total estimated cost of $300. The software is free.

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
..   `Cambridge NeuroTech <http://www.cambridgeneurotech.com/wireless.htm>`_,, ""
..   `Eicom Teleopto <https://eicomusa.com/teleopto/>`_ , "4,500", "Very similar to Cerebro. Uses battery and IR triggering."
..   `Kendall Research Systems <http://www.kendallresearchsys.com/>`_ , "20,000", "Uses wireless power. Includes 3-axis accelerometer"
..   `Neurolux <http://www.neurolux.org/products/>`_,




Documentation Contents:
=======================

.. toctree::
  :maxdepth: 1

  Overview/Overview.rst
  Hardware/cerebro.rst
  Hardware/implant.rst
  Hardware/base station.rst
  Hardware/downloader cord.rst
  Hardware/utility shield.rst
  Hardware/charging dock.rst
  Hardware/remote.rst
  Software/Xavier.rst

Licenses
========
The MIT License (MIT)

Copyright (c) 2015-2016 Andy Lustig

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

.. image:: oshw_logo.png
  :align: left
  :scale: 7%

.. raw:: html

      <a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/">
      <img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by-sa/4.0/88x31.png" /></a>
      <br /><span xmlns:dct="http://purl.org/dc/terms/" property="dct:title">Cerebro Wireless Optogenetics System</span> by
      <a xmlns:cc="http://creativecommons.org/ns#" href="https://www.janelia.org/lab/karpova-lab" property="cc:attributionName" rel="cc:attributionURL">
      Andy Lustig </a> is licensed under a  <a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/">Creative Commons Attribution-ShareAlike 4.0 International License</a>
