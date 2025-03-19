#ifndef RUNASGPU_GRAPHICALUNIT_HPP
#define RUNASGPU_GRAPHICALUNIT_HPP

#include <string>

struct GraphicalUnit {
    int vendor;
    int product;
    std::string vendorName;
    std::string productName;
    std::string fullName;
};

#endif //RUNASGPU_GRAPHICALUNIT_HPP
