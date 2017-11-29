======================
Head Implant Resources
======================
.. include:: Characters/isogrk1.txt
.. include:: Characters/mmlalias.txt
.. include:: Characters/isopub.txt

.. .. figure:: photos/implant_cad.png
..   :align: center
..   :scale: 100%

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
  * `Ultraviolet light <https://www.amazon.com/TaoTronics-TT-FL001-Flashlight-Blacklight-Ultraviolet/dp/B00RV8PREI/ref=sr_1_1?ie=UTF8&qid=1484071557&sr=8-1&keywords=B00RV8PREI>`_


Bill of Materials
=================
+-----+------------------------------+----------------------------------------------------------------+------------------------------------------------------------------------------------------------------------------------+
| Qty | Description                  | Datasheet                                                      | Order Link                                                                                                             | 
+=====+==============================+================================================================+========================================================================================================================+
| 1   | Implant PCB                  |  :download:`Implant 5.3<Datasheets/implant_5.3.pdf>`           | `OSH Park <https://oshpark.com/shared_projects/k8Ikpli6>`_                                                             | 
+-----+------------------------------+----------------------------------------------------------------+------------------------------------------------------------------------------------------------------------------------+
| 1   | Laser Diode                  |  :download:`PLT5 520_B1_2_3<Datasheets/green_laser_diode.pdf>` | `World Star Tech <http://www.worldstartech.com/products/laser-diodes/green-laser-diode-osram/>`_                       | 
+-----+------------------------------+----------------------------------------------------------------+------------------------------------------------------------------------------------------------------------------------+
| 1   | Fiber                        | F-MBB                                                          | `Newport <https://www.newport.com/p/F-MBB>`_                                                                           | 
+-----+------------------------------+----------------------------------------------------------------+------------------------------------------------------------------------------------------------------------------------+
| 1   | UV curing adhesive           |  :download:`NOA 68<Datasheets/uv_adhesive.pdf>`                | `Norland Products <https://www.norlandproducts2.com/adhesives/adproductsdetail_header_removed.asp?Prdid=68>`_          | 
+-----+------------------------------+----------------------------------------------------------------+------------------------------------------------------------------------------------------------------------------------+
| 1   | Ferrule                      |  :download:`MM-FER2007C<Datasheets/ferrule.pdf>`               | `Precision Fiber Products <https://precisionfiberproducts.com/pfp-lc-1-25mm-od-multimode-ceramic-zirconia-ferrules/>`_ | 
+-----+------------------------------+----------------------------------------------------------------+------------------------------------------------------------------------------------------------------------------------+
| 1   | Epoxy                        |  :download:`T120023C<Datasheets/epoxy.pdf>`                    | `Fiber Instrument Sales <http://www.fiberinstrumentsales.com/fis-room-cure-epoxy-2-grams.html>`_                       | 
+-----+------------------------------+----------------------------------------------------------------+------------------------------------------------------------------------------------------------------------------------+
| 2   | Micro USB vertical socket    |  :download:`1051330011<Datasheets/usb_socket_vertical.pdf>`    | `Digi-Key <https://www.digikey.com/products/en?keywords=WM10134CT-ND>`_                                                | 
+-----+------------------------------+----------------------------------------------------------------+------------------------------------------------------------------------------------------------------------------------+
| 2   | 0603 10 k\ |mgr|\F Capacitor |                                                                |                                                                                                                        | 
+-----+------------------------------+----------------------------------------------------------------+------------------------------------------------------------------------------------------------------------------------+
| 2   | 0603 20 k\ |mgr|\F Capacitor |                                                                |                                                                                                                        | 
+-----+------------------------------+----------------------------------------------------------------+------------------------------------------------------------------------------------------------------------------------+

Assembly Instructions
=====================
#. Strip fiber and cut to length
#. Align fiber to laser diode
#. Apply Norland Optical Adhesive 68 and cure it with UV light
#. Solder diodes at a slight angle to the implant PCB
#. Laser cut fiber guide
#. Insert ferrules into acrylic to create fiber guide
#. Thread coupled fibers through fiber guide
#. Incase laser diodes in epoxy
#. Surround the implant with heat shrink tubing

Implant Characterization
========================
