# Remote control
* When you are runing a g1 or g28 code, The remote modification will not take effect.
* Reason is M130 will be executed after current gcode is finished. The M130 is in the gcode_queue.