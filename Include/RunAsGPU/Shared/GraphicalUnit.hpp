#ifndef RUNASGPU_GRAPHICALUNIT_HPP
#define RUNASGPU_GRAPHICALUNIT_HPP

#include <string>

enum GraphicalUnitConnector {
    // The specific GPU is powered by the internal graphical processing unit (iGPU)
    // through the CPU
    INTERNAL = 0,

    // The specific GPU is powered by its own dedicated graphical processing unit (dGPU)
    DEDICATED = 1,

    // The specific GPU is connected via USB-C (most likely Thunderbolt)
    EXTERNAL = 2
};

struct GraphicalUnit {
    int vendor;
    int product;
    std::string vendorName;
    std::string productName;
    std::string fullName;
    GraphicalUnitConnector connector;
};

#endif //RUNASGPU_GRAPHICALUNIT_HPP
