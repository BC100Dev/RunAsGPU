#include <RunAsGPU/Shared/GraphicalUnit.hpp>
#include <RunAsGPU/Shared/Runner.hpp>

#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <sstream>

extern "C" {
#include <pci/pci.h>
#include <stdio.h>
}

void print_unicode(const char *str) {
    while (*str) {
        printf("U+%04X ", (unsigned char)*str);
        str++;
    }
    printf("\n");
}

namespace Runner {

    std::vector<GraphicalUnit> ListGraphicalUnits() {
        std::vector<GraphicalUnit> gpu_units;

        struct pci_access *pacc;
        struct pci_dev *dev;
        char vendor_name[1024], device_name[2048];

        pacc = pci_alloc();
        if (!pacc) {
            std::cerr << "Failed to allocate pci_alloc" << std::endl;
            return gpu_units;
        }

        pci_init(pacc);
        pci_scan_bus(pacc);

        for (dev = pacc->devices; dev; dev = dev->next) {
            pci_fill_info(dev, PCI_FILL_IDENT | PCI_FILL_CLASS);

            // 0x03?? = Display Controllers
            // 0x0300 = VGA compatible controller
            // 0x0301 = XGA compatible controller (old, no longer used)
            // 0x0302 = 3D controller
            // 0x0380 = Display Controller
            if (dev->device_class != 0x0300 && dev->device_class != 0x0301 && dev->device_class != 0x0302 &&
                dev->device_class != 0x0380)
                continue;

            pci_lookup_name(pacc, vendor_name, sizeof(vendor_name), PCI_LOOKUP_VENDOR, dev->vendor_id);
            pci_lookup_name(pacc, device_name, sizeof(device_name), PCI_LOOKUP_DEVICE, dev->vendor_id, dev->device_id);

            GraphicalUnit gpu{
                    .vendor = dev->vendor_id,
                    .product = dev->device_id,
                    .vendorName = std::string(vendor_name),
                    .productName = std::string(device_name),
                    .fullName = std::string(vendor_name) + " " + std::string(device_name)
            };

            gpu_units.push_back(gpu);
        }

        pci_cleanup(pacc);

        return gpu_units;
    }

}