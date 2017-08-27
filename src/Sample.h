#ifndef SAMPLE_H
#define SAMPLE_H

struct Sample {
    unsigned long timestamp;
    long value;
};

typedef Sample* SamplePtr;


#endif