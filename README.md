# gester
Gester is a gesture interpreting program for  touchscreens, that I wrote for my Lenovo Thinkpad L380 Yoga.
The gesture interpretation is being done directly in the program, not relying on other programs, which does allow for easy addition of gestures.
Right now there are 1-finger swipes from the screen edges, multi-finger swipes in up/down/left/right and rotations.
Each gesture triggers a syscall, which can be set in the config.h file, and rotational gestures will have a value depending on the angle (scaled by a factor) appended.
This allows for turning volume or screen brightness in a way that is supposed to feel more like turning a knob.

# issues
I do expect issues when running on different hardware, but do not have anything to test this on right now.
