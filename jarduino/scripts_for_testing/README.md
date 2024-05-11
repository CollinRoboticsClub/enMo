This directory contains various Arduino programs as well as Python scripts for testing!

./jarduino_tester.ino is for testing out motors- helps with ensuring pinout and direction are correct!

f310.py is a "utility" file Josh wrote a while back. It's just a wrapper around pygame's Joystick with mappings for a Logitech F310 controller.

serial_runner.py sends joystick values from a connected F310 controller, and feeds it over Serial to a connected Arduino. Useful for tethered driving, if you're crazy.