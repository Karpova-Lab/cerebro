:github_url: https://github.com/Karpova-Lab/cerebro

=======
Cerebro 
=======

.. include:: Characters/isogrk1.txt
.. include:: Characters/mmlalias.txt

.. image:: photos/cerebro_views.webp
  :align: center
  :width: 100 %

.. figure:: photos/PCB_photos/cerebro5.8.png
  :align: center
  :width: 100%

  `Click Here for Design Files <https://github.com/Karpova-Lab/cerebro/tree/master/PCB%20Files/Cerebro%205.8>`_ 

Bill of Materials
=================
+-----+---------------------------------+-------------------------------------------------------------------+-------------------------------------------------------------------------------------------------------------------------------------------+
| Qty | Description                     | Datasheet                                                         | Order Link                                                                                                                                | 
+=====+=================================+===================================================================+===========================================================================================================================================+
| 1   | Cerebro PCB                     |  :download:`Cerebro 5.8<Datasheets/cerebro5.8.pdf>`               | `OSH Park <https://oshpark.com/shared_projects/ASxhDBJv>`__                                                                               | 
+-----+---------------------------------+-------------------------------------------------------------------+-------------------------------------------------------------------------------------------------------------------------------------------+
| 1   | 400 mAh Battery                 |  :download:`3898<Datasheets/battery_400mah.pdf>`                  | `Adafruit <https://www.adafruit.com/product/3898>`__                                                                                      | 
+-----+---------------------------------+-------------------------------------------------------------------+-------------------------------------------------------------------------------------------------------------------------------------------+
| 1   | 915 MHz Radio                   |  :download:`RFM69HCW<Datasheets/radio.pdf>`                       | `LowPowerLab <https://lowpowerlab.com/shop/product/158>`__                                                                                | 
+-----+---------------------------------+-------------------------------------------------------------------+-------------------------------------------------------------------------------------------------------------------------------------------+
| 1   | Microcontroller                 |  :download:`ATMEGA32U4-MUR<Datasheets/MCU_32u4.pdf>`              | `Digi-Key <https://www.digikey.com/products/en?keywords=ATMEGA32U4-MURCT-ND>`__                                                           | 
+-----+---------------------------------+-------------------------------------------------------------------+-------------------------------------------------------------------------------------------------------------------------------------------+
| 1   | 8 MHz Resonator                 |  :download:`CSTCE8M00G55-R0<Datasheets/resonator_8mhz.pdf>`       | `Digi-Key <https://www.digikey.com/products/en?keywords=490-1195-1-ND>`__                                                                 | 
+-----+---------------------------------+-------------------------------------------------------------------+-------------------------------------------------------------------------------------------------------------------------------------------+
| 1   | 3.3V Linear Regulator           |  :download:`AP2112K-3.3TRG1<Datasheets/regulator_3v_linear.pdf>`  | `Digi-Key <https://www.digikey.com/products/en?keywords=%09AP2112K-3.3TRG1DICT-ND>`__                                                     | 
+-----+---------------------------------+-------------------------------------------------------------------+-------------------------------------------------------------------------------------------------------------------------------------------+
| 1   | Fuel Gauge                      |  :download:`BQ27441DRZR-G1B<Datasheets/fuel_gauge.pdf>`           | `Digi-Key <https://www.digikey.com/product-detail/en/texas-instruments/BQ27441DRZR-G1B/296-39942-1-ND/5177819>`__                         | 
+-----+---------------------------------+-------------------------------------------------------------------+-------------------------------------------------------------------------------------------------------------------------------------------+
| 2   | DAC (12-Bit)                    |  :download:`LTC2630ACSC6-LZ12#TRMPBF<Datasheets/DAC.pdf>`         | `Digi-Key <https://www.digikey.com/product-detail/en/linear-technology/LTC2630ACSC6-LZ12-TRMPBF/LTC2630ACSC6-LZ12-TRMPBFCT-ND/1643783>`__ | 
+-----+---------------------------------+-------------------------------------------------------------------+-------------------------------------------------------------------------------------------------------------------------------------------+
| 2   | Op Amp                          |  :download:`OPA237NA/3K<Datasheets/opamp.pdf>`                    | `Digi-Key <https://www.digikey.com/products/en?keywords=296-26265-1-ND>`__                                                                | 
+-----+---------------------------------+-------------------------------------------------------------------+-------------------------------------------------------------------------------------------------------------------------------------------+
| 2   | NPN Transistor                  |  :download:`FJX3904TF<Datasheets/npn_transistor.pdf>`             | `Digi-Key <https://www.digikey.com/products/en?keywords=FJX3904TFCT-ND>`__                                                                | 
+-----+---------------------------------+-------------------------------------------------------------------+-------------------------------------------------------------------------------------------------------------------------------------------+
| 1   | Boost Converter                 |  :download:`MIC2288YD5-TR<Datasheets/boost_converter.pdf>`        | `Digi-Key <https://www.digikey.com/products/en?keywords=576-1729-1-ND>`__                                                                 | 
+-----+---------------------------------+-------------------------------------------------------------------+-------------------------------------------------------------------------------------------------------------------------------------------+
| 1   | Schottky Diode                  |  :download:`MBRM140T3G<Datasheets/schottky.pdf>`                  | `Digi-Key <https://www.digikey.com/products/en?keywords=MBRM140T3GOSCT-ND>`__                                                             | 
+-----+---------------------------------+-------------------------------------------------------------------+-------------------------------------------------------------------------------------------------------------------------------------------+
| 1   | 10uH Inductor                   |  :download:`LQH43CN100K03L<Datasheets/inductor.pdf>`              | `Digi-Key <https://www.digikey.com/products/en?keywords=490-2519-1-ND>`__                                                                 | 
+-----+---------------------------------+-------------------------------------------------------------------+-------------------------------------------------------------------------------------------------------------------------------------------+
| 2   | 2mm Pitch Male Header           |  :download:`M22-2510805<Datasheets/header_male_2mm.pdf>`          | `Digi-Key <https://www.digikey.com/products/en?keywords=952-1992-nd>`__                                                                   | 
+-----+---------------------------------+-------------------------------------------------------------------+-------------------------------------------------------------------------------------------------------------------------------------------+
| 1   | Slide Switch                    |  :download:`PCM12SMTR<Datasheets/slide_switch2.pdf>`              | `Digi-Key <https://www.digikey.com/products/en?keywords=401-2016-1-nd>`__                                                                 | 
+-----+---------------------------------+-------------------------------------------------------------------+-------------------------------------------------------------------------------------------------------------------------------------------+
| 2   | Momentary Button                |  :download:`B3U-1000P<Datasheets/momentary.pdf>`                  | `Digi-Key <https://www.digikey.com/products/en?keywords=sw1020ct>`__                                                                      | 
+-----+---------------------------------+-------------------------------------------------------------------+-------------------------------------------------------------------------------------------------------------------------------------------+
| 1   | Micro USB vertical plug         |  :download:`ZX20-B-5S-UNIT(30)<Datasheets/usb_plug_vertical.pdf>` | `Digi-Key <https://www.digikey.com/products/en?keywords=H125237-ND>`__                                                                    | 
+-----+---------------------------------+-------------------------------------------------------------------+-------------------------------------------------------------------------------------------------------------------------------------------+
| 1   | Micro USB shielding             |  :download:`ZX20-B-SLDC<Datasheets/usb_shielding.pdf>`            | `Digi-Key <https://www.digikey.com/products/en?keywords=H11496CT-ND>`__                                                                   | 
+-----+---------------------------------+-------------------------------------------------------------------+-------------------------------------------------------------------------------------------------------------------------------------------+
| 1   | Micro USB horizontal socket     |  :download:`10118194-0001LF<Datasheets/usb_plug_horizontal.pdf>`  | `Digi-Key <https://www.digikey.com/products/en?keywords=609-4618-1-ND>`__                                                                 | 
+-----+---------------------------------+-------------------------------------------------------------------+-------------------------------------------------------------------------------------------------------------------------------------------+
| 1   | Molex Micro-Lock socket         |  :download:`5055670271<Datasheets/microlock_socket.pdf>`          | `Digi-Key <https://www.digikey.com/products/en?keywords=WM17038CT-ND>`__                                                                  | 
+-----+---------------------------------+-------------------------------------------------------------------+-------------------------------------------------------------------------------------------------------------------------------------------+
| 1   | Molex Micro-Lock plug with wire |  :download:`0151360206<Datasheets/microlock_plug.pdf>`            | `Digi-Key <https://www.digikey.com/products/en?keywords=WM17161-ND>`__                                                                    | 
+-----+---------------------------------+-------------------------------------------------------------------+-------------------------------------------------------------------------------------------------------------------------------------------+
| 1   | Red LED (0603)                  |  :download:`LTST-C191KRKT<Datasheets/led_red.pdf>`                | `Digi-Key <https://www.digikey.com/products/en?keywords=160-1447-1-ND>`__                                                                 | 
+-----+---------------------------------+-------------------------------------------------------------------+-------------------------------------------------------------------------------------------------------------------------------------------+
| 1   | Amber LED (0603)                |  :download:`LNJ437W84RA<Datasheets/led_amber.pdf>`                | `Digi-Key <https://www.digikey.com/products/en?keywords=%09LNJ437W84RACT-ND>`__                                                           | 
+-----+---------------------------------+-------------------------------------------------------------------+-------------------------------------------------------------------------------------------------------------------------------------------+
| 1   | Green LED (0603)                |  :download:`SML-D12M8WT86<Datasheets/led_green.pdf>`              | `Digi-Key <https://www.digikey.com/products/en?keywords=511-1578-1-ND>`__                                                                 | 
+-----+---------------------------------+-------------------------------------------------------------------+-------------------------------------------------------------------------------------------------------------------------------------------+
| 1   | 470 nF Capacitor (0805)         |                                                                   | `Digi-Key <https://www.digikey.com/product-detail/en/avx-corporation/08055C474KAT2A/478-5033-1-ND/1888244>`__                             | 
+-----+---------------------------------+-------------------------------------------------------------------+-------------------------------------------------------------------------------------------------------------------------------------------+
| 2   | 1 |mgr|\F Capacitor (0805)      |                                                                   | `Digi-Key <https://www.digikey.com/products/en?keywords=1276-1066-1-nd>`__                                                                | 
+-----+---------------------------------+-------------------------------------------------------------------+-------------------------------------------------------------------------------------------------------------------------------------------+
| 1   | 2.2 |mgr|\F Capacitor (0805)    |                                                                   | `Digi-Key <https://www.digikey.com/products/en?keywords=587-1286-1>`__                                                                    | 
+-----+---------------------------------+-------------------------------------------------------------------+-------------------------------------------------------------------------------------------------------------------------------------------+
| 3   | 10 |mgr|\F Capacitor (0805)     |                                                                   | `Digi-Key <https://www.digikey.com/product-detail/en/murata-electronics-north-america/GRM21BR61E106KA73L/490-5523-1-ND/2334919>`__        | 
+-----+---------------------------------+-------------------------------------------------------------------+-------------------------------------------------------------------------------------------------------------------------------------------+
| 1   | 0.01 |OHgr| Resistor (1206)     |                                                                   | `Digi-Key <https://www.digikey.com/products/en?keywords=P0.01BVCT-ND>`__                                                                  | 
+-----+---------------------------------+-------------------------------------------------------------------+-------------------------------------------------------------------------------------------------------------------------------------------+
| 2   | 12 |OHgr| Resistor (0603)       |                                                                   | `Digi-Key <https://www.digikey.com/products/en?keywords=P12.00BYCT-ND>`__                                                                 | 
+-----+---------------------------------+-------------------------------------------------------------------+-------------------------------------------------------------------------------------------------------------------------------------------+
| 4   | 27 |OHgr| Resistor (0603)       |                                                                   | `Digi-Key <https://www.digikey.com/products/en?keywords=541-27.0SCT-ND>`__                                                                | 
+-----+---------------------------------+-------------------------------------------------------------------+-------------------------------------------------------------------------------------------------------------------------------------------+
| 1   | 2 k\ |OHgr| Resistor (0603)     |                                                                   | `Digi-Key <https://www.digikey.com/products/en?keywords=RNCP0603FTD2K00CT-ND>`__                                                          | 
+-----+---------------------------------+-------------------------------------------------------------------+-------------------------------------------------------------------------------------------------------------------------------------------+
| 3   | 4.7 k\ |OHgr| Resistor (0603)   |                                                                   | `Digi-Key <https://www.digikey.com/products/en?keywords=311-4.70KHRCT-ND>`__                                                              | 
+-----+---------------------------------+-------------------------------------------------------------------+-------------------------------------------------------------------------------------------------------------------------------------------+
| 2   | 12 k\ |OHgr| Resistor (0805)    |                                                                   | `Digi-Key <https://www.digikey.com/products/en?keywords=311-12.0KCRCT-ND>`__                                                              | 
+-----+---------------------------------+-------------------------------------------------------------------+-------------------------------------------------------------------------------------------------------------------------------------------+
| 5   | 20 k\ |OHgr| Resistor (0603)    |                                                                   | `Digi-Key <https://www.digikey.com/products/en?keywords=RNCP0603FTD20K0CT-ND>`__                                                          | 
+-----+---------------------------------+-------------------------------------------------------------------+-------------------------------------------------------------------------------------------------------------------------------------------+
| 2   | 100 k\ |OHgr| Resistor (0603)   |                                                                   | `Digi-Key <https://www.digikey.com/products/en?keywords=%09311-100KHRCT-ND>`__                                                            | 
+-----+---------------------------------+-------------------------------------------------------------------+-------------------------------------------------------------------------------------------------------------------------------------------+


Programming Instructions
========================

.. _burn boot:

Setup Arduino IDE
-----------------
Follow these instructions : https://learn.adafruit.com/adafruit-feather-32u4-basic-proto/arduino-ide-setup

Burning a Bootloader
--------------------

Cerebro is designed to be easily reprogrammed through the onboard micro USB. 
However when builing a Cerebro from scratch, you must first burn a `bootloader <https://www.arduino.cc/en/Tutorial/ArduinoISP>`_  onto the microcontroller in order to later upload programs through USB. 

To burn a bootloader you need an external programmer that will talk to the microntroller through the In-Circuit Serial Programmer (ISP). 
You can use an Arduino as your external ISP and temporarily solder wires to the Cerebro PCB. 
I recommmend using a `USB microISP <https://www.tindie.com/products/nsayer/usb-uisp/>`_ along with a :doc:`Pogo Connector <programmer>`.

1. The programmer needs to connect to the ISP thru-holes (MOSI, MISO, SCK, RST, GND and 3V) on the Cerebro PCB.

.. figure:: photos/Programmer/programmer.jpg
  :align: center
  :width: 100%

2. In the Arduino IDE, select Tools->Board->"Adafruit Feather 32u4" 
3. Select  Tools->Programmer->"USBtinyISP" or Tools->Programmer->"ArduinoISP" if using an Arduino as the programmer
4. Select Tools->Burn Bootloader

.. figure:: photos/Programmer/burn.png
  :align: center
  :scale: 100%

Uploading Test Program
----------------------
To test that the components on the PCB are soldered correctly and that all of the required connections are being made, we upload a test program. 

1. Open testBoard.ino in the Arduino IDE.
2. Select Tools->Board->"Adafruit Feather 32u4".
3. Under the Tools->Port menu look to see what Serial Ports are available. 
4. Connect a battery to Cerebro.
5. Connect Cerebro to your computer with a usb cord and switch on Cerebro. A red LED should be lit, indicating that Cerebro is on.
6. There should now be a new serial port under the Tools->Port menu that wasn't there in step 2. This is the Cerebro serial port, select it. If there a new serial port didn't appear in the menu, make sure Cerebro is turned on and connected via USB. If it still doesn't appear in the menu, the bootloader may have not been burned correctly, so try burning the bootloader again.
7. Click the upload button to upload the the testBoard firmware.
8. Open up Serial Monitor by click the magnifying glass icon.
9. Set the Serial Monitor Baud Rate to 115200 Baud.
10. Two yellow LEDs on Cerebro should be blinking and text should be appearing in the Serial Monitor.
11. Follow the instructions on the Serial Monitor. If everything is working properly then the battery monitor should be able to provide a Battery charge %.

Uploading Firmware
------------------
1. Open cerebro.ino in the Arduino IDE.
2. Select Tools->Board->"Adafruit Feather 32u4".
3. Click the upload button to upload the the Cerebro firmware.

Assigning a Serial Number
=========================
A serial number is assigned to each Cerebro. When a Base Station connects to Cerebro, it communicates on the radio channel corresponding to Cerebro's serial number. This prevents crosstalk between Base Stations and Cerebros in the case where multiple behavior sessions are being run at the same time in the same room. Each Cerebro being used should setup a unique serial number between 1 and 255 using the process below. 

1. Start an Xavier session in Debug Mode. The Base Station should be connnected, and have a green check mark.

.. image:: photos/serial_number/base_connected.png
  :align: center
  :scale: 100 %

2. Click ``Setup New Cerebro`` 

.. image:: photos/serial_number/click_new_cerebro.png
  :align: center
  :scale: 100 %

3. Hold down the button labeled "BTN" on Cerebro until the nearby LED blinks twice. A green checkmark should now be next to the "Cerebro Wireless Connection".

.. image:: photos/serial_number/hold.png
  :align: center
  :width: 100 %

4. Fill in the "Serial Number" box with the serial number you want to give to Cerebro and then click ``Set Serial Number``.

.. image:: photos/serial_number/give_number.png
  :align: center
  :scale: 100 %

5. After a few seconds, click the ``Retry Connection`` button to confirm everything worked. All three indicators should have green check marks. Cerebro now has a serial number saved to its memory, and whenever it is turned on it will try to communicate with a Base Station on a radio channel of the same number.

.. image:: photos/serial_number/good.png
  :align: center
  :scale: 100 %