#include <RunAsGPU/Shared/GraphicalUnit.hpp>
#include <RunAsGPU/Shared/Runner.hpp>

#include <iostream>
#include <string>
#include <cstring>
#include <fstream>

extern "C" {
#include <pci/pci.h>
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
            if (dev->device_class != 0x0300 && dev->device_class != 0x0301 && dev->device_class != 0x0302 && dev->device_class != 0x0380)
                continue;

            pci_lookup_name(pacc, vendor_name, sizeof(vendor_name), PCI_LOOKUP_VENDOR, dev->vendor_id);
            pci_lookup_name(pacc, device_name, sizeof(device_name), PCI_LOOKUP_DEVICE, dev->vendor_id, dev->device_id);

            GraphicalUnitConnector connector;

            bool is_igpu = dev->vendor_id == 0x8086 || (dev->vendor_id == 0x1002 && dev->device_id < 0x6800);
            bool is_dgpu = dev->bus >= 1;

            if (is_igpu)
                connector = INTERNAL;
            else if (is_dgpu)
                connector = DEDICATED;
            else
                connector = EXTERNAL;

            GraphicalUnit gpu{
                    .vendor = dev->vendor_id,
                    .product = dev->device_id,
                    .vendorName = std::string(vendor_name),
                    .productName = std::string(device_name),
                    .fullName = std::string(vendor_name) + " " + std::string(device_name),
                    .connector = connector
            };

            gpu_units.push_back(gpu);
        }

        pci_cleanup(pacc);

        return gpu_units;
    }

}