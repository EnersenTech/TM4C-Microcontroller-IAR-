<h2>Objective</h2>
Design a interrupt driven edge-triggered input that counts rising edges of PF0. 
<br>

<h2>Process</h2>
Main thread shares a global variable which has been processed in the <b>PortF_Handler</b> ISR (Interrupt Service Routine).
<br>
<br>
1) Pressing SW2 on PE0 triggers an event. <br>
2) Clear the I bit, which enables all interrupt on Microcontroller.  <br>
3) Perform an PortF action. <br>
4) Acknowledge the interrupt then clear the flag. <br>
<br>
<br>
<img src="https://user-images.githubusercontent.com/109411385/211511102-a61a30fb-3aa0-4377-8f98-e522af950bd1.png" width=500 height=200/>
Figure 1. Eight times event triggered


<h3>demo</h3>
<img src="https://user-images.githubusercontent.com/109411385/211510306-c2ab1ea5-ec2d-4115-a1cd-ecc522b8ffee.gif"/>
