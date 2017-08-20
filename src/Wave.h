#ifndef WAVE_H
#define WAVE_H

#include <Wire.h>
#include <Arduino.h>


class Wave {

// PRIVATE
// ------------------------------------------------------------------------------------------
private:


// PUBLIC
// ------------------------------------------------------------------------------------------
public:

Wave() {
    
}

~Wave() {

}

virtual long getSample() {
    return 0;
}


};

#endif