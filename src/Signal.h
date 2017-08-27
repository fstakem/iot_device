#ifndef SIGNAL_H
#define SIGNAL_H

#include <Wire.h>
#include <Arduino.h>


class Signal {

// PRIVATE
// ------------------------------------------------------------------------------------------
private:


// PUBLIC
// ------------------------------------------------------------------------------------------
public:

Signal() {
    
}

~Signal() {

}

virtual long getSample() {
    return 0;
}


};

#endif