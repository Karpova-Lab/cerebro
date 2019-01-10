:github_url: https://github.com/Karpova-Lab/cerebro/tree/1.0
======
Xavier
======

.. image:: Xavier.png
    :align: center
    :scale: 80%

Installation
===================

System Requirements
-------------------
  - Windows XP or newer
  - Python 3.5 with numpy, pandas, and matplotlib libraries

.. tip::
  Download `Anaconda Python 3.5 Distribution <http://continuum.io/downloads>`_ , which has all of the required libraries included

  .. _setup default:

Download Instructions
---------------------
1. Download and unzip :download:`Xavier Deployment Folder<Deployment.zip>`
2. Open up Xavier.exe

.. figure:: photos/Install/Xavier_open.png
    :align: center
    :scale: 75%

3. Setup a :ref:`default directory<edit default directory>` that data will be saved to



Settings
========
Access Xavier Settings by navigating to **File->Settings..** or by pressing :kbd:`Ctrl` + :kbd:`,`

.. figure:: photos/Settings/settings_dialog.png
    :align: center
    :scale: 100%

.. note::
  Settings cannot be changed in the middle of a session

.. _edit default directory:

Edit Default Save Directory
-------------------------------

1.  Click ``Change Directory`` to setup a new save location.

.. figure:: photos/Settings/change_directory.png
    :align: center
    :scale: 100%

2. Navigate to an existing folder or create a new folder for future data.

.. figure:: photos/Settings/new_folder.png
    :align: center
    :scale: 60%

3. Finally, click **Select Folder**. The "Current Directory" will reflect updated save path

.. figure:: photos/Settings/rename_and_select.png
    :align: center
    :scale: 60%

.. figure:: photos/Settings/directory_changed.png
    :align: center
    :scale: 100%

.. _add items:

Edit Session Setup Lists
------------------------
Adding Items
````````````
Type new items into the textboxes and click ``Add Item`` (or press :kbd:`Enter`)

.. image:: photos/Settings/additem.gif
  :align: center
  :scale: 100%

Removing Items
``````````````
Select the item from list you want to remove and click ``Remove Selected``

.. image:: photos/Settings/removeitem.gif
  :align: center
  :scale: 100%

.. _label ports:

Labeling COM Ports
------------------
When multiple usb devices are connected to the computer, it can be hard to keep track of which COM Port belongs to which device. To minimize confusion when connecting
to serial ports, Xavier allows you to label COM Ports.

.. image:: photos/Settings/port_labeling.png
  :align: center
  :width: 100%

Adding Labels
`````````````
1. From the dropdown box, select the COM Port you want to add a label to
2. Type the new label into the textbox and click ``Add/Edit Label`` (or press :kbd:`Enter`)

.. image:: photos/Settings/addlabel.gif
  :align: center
  :scale: 100%

Editing Labels
``````````````
1. Double-click the labeled COM Port from the list that you want to edit. A label editing dialog will appear.
2. Type the new label into the textbox and click ``Change Label``

.. image:: photos/Settings/editlabel.gif
  :align: center
  :scale: 100%

Removing Labels
```````````````
Select the labeled COM Port from the list that you want to remove and click ``Remove Selected``

.. image:: photos/Settings/removelabel.gif
  :align: center
  :scale: 100%

.. _waveform parameters:

Waveform Parameters
===================
Five waveform parameters (Start Delay, On Time, Off Time, Train Duration, Ramp Down) can be wirelessly changed throughout the session.
The user can make changes using either a **Single Shot** mode with an optional ramp down or **Pulse Train** mode. Non-applicable parameters are automatically set to zero depending on
options selected. Once the parameters are sent to and received by Cerebro, they are saved to non-volatile memory and are therefore retained between sessions.

.. Parameter Definitions
.. ---------------------
.. Power Level
..   Level of light output. Each laser diode assembly is calibrated to provide a range of power level values that correspond with known light power (mW) outputs.
.. Start Delay (ms)
..   Time between receiving a trigger signal, and beginning the light pulse
.. On Time (ms)
..   Duration the laser is on at the given Power Level
.. Off Time (ms) [Pulse Train mode]
..   Duration the laser is off
.. Train Duration (ms) [Pulse Train mode]
..   Duration that the on-off cycles are repeated
.. Ramp down (ms) [Single Shot mode]
..   Time it takes for the laser to linearly reduce its output from Power Level to completely off. Must be a mulitple of 100 ms.

.. image:: photos/Edit_waveform/parameter_diagram.png
    :align: center
    :width: 100%

.. _pulse change header:

Checking Current Parameters
---------------------------
1. Select the appropriate Downloader Serial Port and click ``Connect to Downloader``

.. image:: photos/check_params/connect_downloader.png
  :align: center
  :scale: 100%

2. With Cerebro turned off, plug the :ref:`cord` into Cerebro's micro usb port
3. Turn on Cerebro.

.. image:: photos/check_params/connect_plug.jpg
  :align: center
  :scale: 20%

4. A dialog box with all of Cerebro's parameters will appear. Additionally, the "Cerebro's Parameters" section will automatically be updated to match the parameters currently on Cerebro.

.. image:: photos/check_params/parameters_popup.png
  :align: center
  :scale: 110%

Changing Parameters
-------------------
1. In the "Cerebro's Waveform Parameters" section, select desired pulse options and modify the parameters

.. image:: photos/Edit_waveform/parameter_change.png
  :align: center
  :scale: 40%

2. Click ``Send New Waveform Parameters``.

3. A confirmation dialog will appear.

.. figure:: photos/Edit_waveform/send_confirm.png
  :align: center
  :scale: 100%

4. Once confirmed, the settings will be sent and the last sent parameters will be displayed at the bottom

.. figure:: photos/Edit_waveform/last_sent.png
  :align: center
  :scale: 135%

.. seealso::
  The IR Remote can be used as an alternative to :ref:`send new parameters <remote send>`. Additionally, it is capable of :ref:`checking the current parameters <remote check>`  saved to Cerebro's memory.

Implant Construction Tools
==========================

Diode Characterization
----------------------

:ref:`Intructions here<diode char>`

Implant Characterization
------------------------

:ref:`Intructions here<implant char>`


.. _send power:

Send Power Values to Cerebro
----------------------------
Each implant has different response curves to power levels provided by Cerebro.
To output a desired light level, Cerebro must know how much current to provide.
To implement a fade, Cerebro must know multiple current levels to step through such that  the implant's light output linearly decreases from a desired light output down to 0 mW.
We therefore send Cerebro a vector of values that correspond to our desired light output with a linear fade to 0 mW.

1. Select "Debug Mode" and start a new session.
2. In the "Implant Construction Tools" section click ``Send Power Values to Cerebro``
3. Assuming you've previously :ref:`characterized an implant<implant char>`, click ``Create fade vector from power meter file``
4. Input a target power into the text box and then click ``Select Power Meter File``
5. After choosing a file, a implant characterization graph will appear, a summary file will be saved to the same directory as the selected power meter file.
6. The fade vector is automatically copied from the summary file into the fade vector input.
7. Fill in the the information for Cerebro # and implant # then click ``Send Values``
8. Confirm you want to send new power levels to Cerbro and the values will be sent.


Running an Experiment
=====================

.. _start session:

Starting a New Session
----------------------
1. Fill out experimental setup information.

.. image:: photos/Start_session/rat_info.png
    :align: center
    :scale: 100%

2. Select a Base Station Serial Port.

.. image:: photos/Start_session/com_port.png
    :align: center
    :scale: 100%

3. Click ``Start Session``

.. image:: photos/Start_session/start_session.png
    :align: center
    :scale: 100%

.. seealso::
  The dropdown menus can be :ref:`edited <add items>` and COM Ports that appear in the dropdown list can be :ref:`labeled<label ports>`

.. _upload header:

Ending a Session and Saving Data
--------------------------------

.. important::
  1. Before opening the rig and removing the rat, click the ``End Session`` button. "Save Sent" will be printed to the Base Station Monitor.

.. image:: photos/End_session/end_session.png
  :align: center
  :scale: 100%

2. Turn off Cerebro and disconnect it from the rat's head implant
3. Make sure the :ref:`cord` is plugged into the computer
4. Select the appropriate Serial Port and click ``Connect to Downloader``

.. image:: photos/End_session/connect_downloader.png
  :align: center
  :scale: 100%

5. Plug the :ref:`cord` into Cerebro's micro usb port
6. Press and hold the download button
7. While still holding down the download button, switch Cerebro on. The event log will be printed to the Download Monitor.
   You may release the download button once events have begun being printed to the Cerebro Monitor.

.. image:: photos/End_session/upload.png
  :align: center
  :scale: 20%

8. Click ``Save Session`` button.

.. image:: photos/End_session/save_session.png
  :align: center
  :scale: 100%

9. After saving, a dialog will appear with a summary of the session

.. image:: photos/End_session/summary.png
  :align: center
  :scale: 100%
