# Workshop Heater Repair Using Arduino Nano
I fixed my dodgy workshop heater by replacing the heater's control PCB with an Arduino Nano
```
This video is sponsored by https://www.pcbway.com  
PCBWay will have Chinese New Year's holidays from Jan 30th to Feb 11th.  
First order free! 2-sided PCB just $5 for 10 pieces. Assembly for 10-pieces just $88.
PCB Prototyping the easy way - see my video #129 for Hands-On details.

Why Buy Your Printed Circuit Boards From PCBWay?  
  * Prototype Fab with 24 hours Express  
  * Team supported with over 50 professionals & technical  
  * 99% on-Time Shipping with great support from DHL  
  * Great quality approved by customers all over the world  
  * Trusted Experience  
  * 24 Hour Non-stop Tech Support and Production  
  * Low Minimums  
  * Factory Outlet PCBs with most competitive advantages  
  * Chief PCB Manufacturer Located in China 
```
## See my video at https://www.youtube/ralphbacon video #133  
(Direct link to video: https://youtu.be/TJHJeLOK6nk)  

My workshop under-workbench heater has been playing up for some time, so it was time to get it fixed - or throw it away, far too easy an option in our throwaway society!

I'm all for the Right To Repair so I dove right in and ripped out its innards (some kind of microcontroller circuit board) and replaced it with an Arduino Nano 168. There! Just like that!

Ok, there were a few things to consider along the way, and I was designing the hardware by the seat of my pants, seeing what might work and what didn't.

I had to work with the existing heating element, air fan and thermistor, but I replaced the mains relays and adjusted the actual logic of the µContoller to my liking! 

I even used the "dodgy" Tenstar Robot 5v PSU from video #105 in which we sort of branded it as a bit UNSAFE! Well, this was the perfect opportunity to use it in a Real World situation.

The end result was better than I expected - it works better now than it did when brand new! Yes, really. And I can always "tweak" the logic if I want as I ensured that I had access to the Nano's USB port.

Just goes to show that if you put your mind to it there's nothing that can stop you - not even (potentially) lethal, high current mains voltage!

The "final" (at time of writing) sketch can be found on my Github https://www.github.com/ralphbacon along with this description and anything else I find that is pertinent.

LINKS LINKS (some might be affiliate links but that doesn't mean I include or exclude or change my editorial at all)  

Typical 10K thermistor (easy to use):  
https://www.banggood.com/10pcs-NTC-Thermistor-Temperature-Sensor-10K-OHM-MF52-103-3435-1-p-932790.html?p=FQ040729393382015118&utm_campaign=25129675&utm_content=3897

Example of 10A 240v relay I used, de-rate to 5A and should be OK!  
https://www.banggood.com/Mini-5V-DC-Power-Relay-SRD5VDCSLC-5-Pin-PCB-Type-p-930170.html?p=FQ040729393382015118&utm_campaign=25129675&utm_content=3897

Or buy three for a better price:  
https://www.aliexpress.com/item/Power-Relays-SRE-05VDC-SL-2C-SRE-12VDC-SL-2C-SRE-24VDC-SL-2C-5V-12V/32824080667.html

Low speed (5-6rpm) synchronous motor 220-240V used to swing the heater left and right:  
https://www.banggood.com/Turntable-Synchronous-Motor-For-Cooker-AC-220V-240V-5-6RPM-5060hz-4W-CWCCW-p-982943.html?p=FQ040729393382015118&utm_campaign=25129675&utm_content=3897

---

If you like this video please give it a thumbs up, share it and if you're not already subscribed please consider doing so :)

My channel and blog are here:  
------------------------------------------------------------------  
https://www.youtube.com/RalphBacon  
https://ralphbacon.blog  
------------------------------------------------------------------

