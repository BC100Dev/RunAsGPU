#include <iostream>

#include <RunAsGPU/Shared/Shared.hpp>
#include <RunAsGPU/Shared/HelpPage.hpp>
#include <RunAsGPU/Shared/Runner.hpp>

namespace fs = std::filesystem;

void list() {
    std::vector<GraphicalUnit> gpu_list = Runner::ListGraphicalUnits();
    for (int i = 0; i < gpu_list.size(); i++) {
        const GraphicalUnit& unit = gpu_list[i];

        std::cout << "GPU - " << unit.vendorName << " (" << std::hex << unit.vendor << ")\n";
        std::cout << "\tName = " << unit.productName << " (" << std::hex << unit.product << ")\n";
        std::cout << "\tID = " << i << "\n";
        std::cout << "\tConnector = ";

        switch (unit.connector) {
            case EXTERNAL:
                std::cout << "External Graphics Card" << std::endl;
                break;
            case INTERNAL:
                std::cout << "Internal GPU" << std::endl;
                break;
            case DEDICATED:
                std::cout << "Dedicated GPU" << std::endl;
                break;
        }

        if (i != gpu_list.size() - 1)
            std::cout << std::endl;
    }
}

int main(int argc, char **argv) {
    if (argc == 1 || (argc == 2 && (std::string(argv[1]) == "--help" || std::string(argv[1]) == "-h"))) {
        std::string fn = GetExecutablePath().filename().string();

        std::cout << "RunAsGPU v1.0" << std::endl;
        std::cout << "usage: ./" << fn << " [action] [options] [exec [..args]]" << std::endl << std::endl;
        std::cout << "actions:" << std::endl;

        HelpPage actions;
        actions.setStartSpaceWidth(4);
        actions.setDescSeparator(" ");
        actions.addArg("run", "", "Run an executable");
        actions.addArg("list", "", "List all active GPUs");
        actions.display(std::cout);

        std::cout << std::endl;
        std::cout << "options:" << std::endl;

        HelpPage options;
        options.setStartSpaceWidth(4);
        options.setSpaceWidth(3);
        options.setDescSeparator("=");
        options.addArg("-ui  | --unit-id", "ID", "Select a specific GPU by its given ID");
        options.addArg("-fu  | --find-unit", "[GPU]", "Find a GPU by its identification (Vendor ID, Device / Product ID, Name)");
        options.addArg("-fnu | --find-unit-name", "[Name]", "Find a GPU by the name");
        options.addArg("-fvu | --find-unit-vendor", "[Vendor]", "Find a GPU by the name or ID of the Vendor (e.g. NVIDIA)");
        options.addArg("-fpu | --find-unit-product", "[Product]", "Find a GPU by the name or ID of the Product (e.g. GeForce 6800 XT)");
        options.addArg("-ni  | --non-interactive", "", "Will not prompt for GPU selection (e.g. running as a script)");
        options.display(std::cout);

        return 0;
    }

    std::vector<std::string> args;
    std::string action = argv[1];
    if (action == "list") {
        list();
        return 0;
    }

    return 0;
}