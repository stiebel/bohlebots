<h1>eagle files</h1>
The software "eagle" is a very mature pcb creating software available as a free version (working for a lot off projects) and
educational version available for studenst for free. See the homepage of "eagle pcb".

We decided to use eagle an therefore our pcbs are in this format.

Creating own pcbs for robotics is an issue at the beginning. 
You do make mistakes even if you try hard and creating fully populated boards is rather expensive. Especially if you have to do this several times.
So we decided to create an modular system providing breakout boards for the frequently used features allowing to habe main boards without SMD components.
These pcbs can be purchased at companies worldwide, we get them from JLCPCB.
<br>
You will find four files of each project:<br>
XXX.sch : this is the schematics of the board<br>
XXX.brd : this is the layout of the board<br>
XXX.lbr : this is the library you can chose to use this board as a device on your mainboard<br>
XXX.jpg : a picture how it looks like.<br>
<br>
So these are the files provided:

<h2>can-breakout</h2>
We decided to use the can-bus as our major bus system to talk between different boards.
So we need a can-bus controller and soccets for the cable we use.
<br>This is a breakout board to plug in with standard 1/10 inch pins.
<image src=./images/can.jpg>

<h2>taster-LED</h2>
Most robotic projects need buttons and LEDs.
To provide maximum felxibilaty we designed a breakout board with 2 buttons and 2 RGB LEDs and an i2c port expander.
The i2c-address is choosen by short-circuiting pads on the bottom-side of the pcb.
<br>This is a breakout board to plug in with standard 1/10 inch pins.
<image src=./images/can.jpg>
  
<h2>kick</h2>
Most robotic projects need drivers for loads.
We distinguish between two issues: on/off and forward/reverse
This is the on/off board using a High Power FET with including CMOS driver to be controlled by 3.3V
We use this board amongst other things for a kicker of soccer playing robot, so we called this circuit board "kick"
<br>This is a breakout board to plug in with standard 1/10 inch pins.
<image src=./images/can.jpg>
  
<h2>driver</h2>
Most robotic projects need drivers for loads.
We distinguish between two issues: on/off and forward/reverse
This is forward/reverse board using a IFX9201 motor controller to be controlled by 3.3V
We use this board as a standard board to drive dc brushed motors up to 16V/5A.
It includes a overload-protection which switches of the controller before a damage accures.
The controller uses !enabled, direction and pwm to provide full functionallity to proportional
speed settings as well as stop/break and stop/float.
<br>This is a breakout board to plug in with standard 1/10 inch pins.
<image src=./images/can.jpg>


