:github_url: https://github.com/Karpova-Lab/cerebro/tree/1.0

.. include:: isogrk1.txt

.. Warning::
  You are currently viewing documentation for the **infrared** controlled Version 1 system.  To view documentation for the **radio** controlled Version 2 system, click the button below.

  .. raw:: html

    <div style="text-align:center; margin-bottom:24px">
      <a href="https://karpova-lab.github.io/cerebro/" style="
      background-color: #efb282;
      border: none;
      color: white;
      padding: 10px 10px;
      text-align:center;
      text-decoration: none;
      display: inline-block;
      font-size: 40px;
      font-weight: bold;
      line-height: 1.1;
      border-radius:15px">
      New Version 2 Documentation</a>
    </div>

========================================
Cerebro Wireless Optogenetics System v1
========================================

Welcome! This is the hardware and software documentation of a system used for wireless `optogenetic <https://en.wikipedia.org/wiki/Optogenetics>`_ stimulation.
As an `open-source <http://www.oshwa.org/definition/>`_ project, this documentation is intended to provide exhaustive resources and detailed instructions to enable anyone to
study, replicate, share, improve and contribute back to the Cerebro Wireless Optogenetics System.

.. raw:: html

  <div style="text-align:center; margin-bottom:24px">
    <a href="https://github.com/Karpova-Lab/cerebro/tree/1.0" style="
    background-color: #2980b9;
    border: none;color: white;
    padding: 15px 15px;
    text-align:center;
    text-decoration: none;
    display: inline-block;
    font-size: 18px;
    border-radius:15px">
    Version 1 (infrared system) files on Github</a>
  </div>

This project was developed by Andy Lustig in the `Karpova Lab at Janelia Research Campus <https://www.janelia.org/lab/karpova-lab>`_.
If you have any questions you can contact him at lustiga@janelia.hhmi.org

.. toctree::
  :maxdepth: 1
  :hidden:

  Overview/Overview.rst
  Hardware/cerebro.rst
  Hardware/implant.rst
  Hardware/base station.rst
  Hardware/utility shield.rst
  Hardware/charging dock.rst
  Hardware/remote.rst
  Hardware/downloader cord.rst
  Software/Xavier.rst
  Notes/notes.rst


Hardware License
++++++++++++++++
Janelia Open-Source Hardware (Non-commercial)

By downloading, using and/or viewing these designs, documentation and related data and information, and all content in the foregoing (collectively, the “Designs”), you agree to the following:

You may use, copy, modify, display and distribute the Designs for any non-commercial research or educational purpose.

THE DESIGNS ARE PROTOTYPES AND ARE EXPERIMENTAL IN NATURE AND SHOULD BE USED WITH PRUDENCE AND APPROPRIATE CAUTION, AS NOT ALL OF THE CHARACTERISTICS ARE KNOWN. THE DESIGNS ARE PROVIDED “AS-IS” WITHOUT ANY EXPRESS OR IMPLIED WARRANTIES OF ANY KIND (INCLUDING, BUT NOT LIMITED TO, ANY IMPLIED WARRANTIES OF MERCHANTABILITY, NON-INFRINGEMENT, TITLE, ACCURACY, OR FITNESS FOR A PARTICULAR PURPOSE, AND ANY WARRANTIES ARISING FROM COURSE OF PERFORMANCE, COURSE OF DEALING, COURSE OF CONDUCT, OR USAGE OF TRADE) OR CONDITIONS OF ANY KIND, WHETHER EXPRESS OR IMPLIED OR STATUTORY. NO OPINION, ADVICE OR STATEMENT OF THE HOWARD HUGHES MEDICAL INSTITUTE (“HHMI”) OR ITS TRUSTEES, EMPLOYEES, AGENTS, LICENSORS OR SUPPLIERS, WHETHER MADE WITHIN THE DESIGNS OR OTHERWISE, SHALL CREATE ANY WARRANTY. HHMI MAKES NO WARRANTY THAT THE DESIGNS WILL BE ACCURATE, CURRENT OR RELIABLE. SOME JURISDICTIONS DO NOT ALLOW THE EXCLUSION OF CERTAIN WARRANTIES OR CONDITIONS, AND AS A CONSEQUENCE SOME OF THE ABOVE DISCLAIMERS MAY NOT APPLY TO YOU. 

IN NO EVENT SHALL HHMI OR ITS TRUSTEES, EMPLOYEES, AGENTS, LICENSORS OR SUPPLIERS (THE “HHMI PARTIES”) BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, CONSEQUENTIAL, OR PUNITIVE DAMAGES OR LOSSES OF ANY KIND (INCLUDING, BUT NOT LIMITED TO, ANY PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, BUSINESS, GOODWILL, DATA, REVENUE, OR PROFITS; REASONABLE ROYALTIES; BUSINESS INTERRUPTION; OR OTHER INTANGIBLE LOSSES) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, TORT (INCLUDING, WITHOUT LIMITATION, NEGLIGENCE) OR OTHERWISE, ARISING IN ANY WAY OUT OF THE USE OF, OR INABILITY TO USE, THE DESIGNS, OR FOR ANY OTHER CLAIM RELATED IN ANY WAY TO YOUR USE OF THE DESIGNS OR YOUR INTERACTIONS WITH HHMI, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE OR LOSS, AND NOTWITHSTANDING THE FAILURE OF ESSENTIAL PURPOSE OF ANY LIMITED REMEDY. YOU ACKNOWLEDGE THAT THESE LIMITATIONS ARE REASONABLE, THAT THEY ARE AN ESSENTIAL ELEMENT HEREOF, AND THAT ABSENT SUCH LIMITATIONS, HHMI WOULD NOT MAKE THE DESIGNS AVAILABLE TO YOU OR ENTER INTO THESE TERMS AND CONDITIONS. YOU REMAIN RESPONSIBLE FOR ALL LEGAL COMPLIANCE WITH RESPECT TO YOUR USAGE OF THE DESIGNS. SOME JURISDICTIONS DO NOT ALLOW THE LIMITATION OF LIABILITY FOR INCIDENTAL OR CONSEQUENTIAL DAMAGES, AND AS A CONSEQUENCE SOME OF THE ABOVE LIMITATIONS MAY NOT APPLY TO YOU; IN SUCH JURISDICTIONS, THE LIABILITY OF THE HHMI PARTIES SHALL BE LIMITED TO THE FULLEST EXTENT PERMITTED BY LAW.

Any redistribution of the Designs must contain or reference these terms.

Software License
++++++++++++++++
Janelia Open-Source Software

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

- Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
    
- Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

- Neither the name of HHMI nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS “AS IS” AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.