# VCU-IRT
Vehicule Control Unit code fully writen in C++

So far, there is only a basic function to calculate the throttle,located in throttle.cpp, based on the value gotten for the APPS and a brake pedal input. The variables.cpp file is used to store usefull variables(speed, direction...).  for throtel login look into utils.cpp and throttle.cpp 

The code compiles now, to test diiferent result depending on apps values, in the vcu.cpp file, change the first value in calcThrottle function call.


TODO: replace int and float with int16_t in variables