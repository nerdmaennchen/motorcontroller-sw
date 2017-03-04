# motorcontroller-sw

Software to easily interface to the BLDC board.
There is an example.cpp which shold explain everything there is to know.
Simply build a handle to map local memory to the memory of the uC on the other side of USB and use right and left assignments as setters and getters.
Basically those handles can be treated as local memory since they implement the underlying protocoll transparently.
