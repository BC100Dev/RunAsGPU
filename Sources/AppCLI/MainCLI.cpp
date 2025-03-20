#include <iostream>

#include <RunAsGPU/Shared/Shared.hpp>
#include <RunAsGPU/Shared/Runner.hpp>

namespace fs = std::filesystem;

enum FindBy {
    Name = 0,
    Vendor = 1,
    Product = 2,
    Global = 3
};

int findUnit(const std::vector<GraphicalUnit> &gpuList, const std::string &name, const FindBy &by) {
    for (int i = 0; i < gpuList.size(); i++) {
        switch (by) {
            case Name:
                if (StringContains(gpuList[i].fullName, name))
                    return i;

                break;
            case Vendor:
                if (StringContains(gpuList[i].vendorName, name))
                    return i;

                if (name.find_first_not_of("0123456789ABCDEFabcdef") == std::string::npos) {
                    try {
                        int ven = std::stoi(name, nullptr, 16);
                        if (gpuList[i].vendor == ven)
                            return i;
                    } catch (const std::exception &e) {
                        std::cerr << "Error: Invalid Vendor ID: " << name << std::endl;
                        return -1;
                    }
                }

                break;
            case Product:
                if (StringContains(gpuList[i].productName, name))
                    return i;

                if (name.find_first_not_of("0123456789ABCDEFabcdef") == std::string::npos) {
                    try {
                        int prod = std::stoi(name, nullptr, 16);
                        if (gpuList[i].product == prod)
                            return i;
                    } catch (const std::exception &e) {
                        std::cerr << "Error: Invalid Product ID: " << name << std::endl;
                        return -1;
                    }
                }

                break;
            case Global:
                if (int res = findUnit(gpuList, name, Name); res != -1) return res;
                if (int res = findUnit(gpuList, name, Vendor); res != -1) return res;
                if (int res = findUnit(gpuList, name, Product); res != -1) return res;
                return -1;
        }
    }

    return -1;
}

void list(const std::vector<GraphicalUnit> &gpu_list) {
    for (int i = 0; i < gpu_list.size(); i++) {
        const GraphicalUnit &unit = gpu_list[i];

        std::cout << "GPU - " << unit.vendorName << " (" << std::hex << unit.vendor << ")\n";
        std::cout << "\tName = " << unit.productName << " (" << std::hex << unit.product << ")\n";
        std::cout << "\tID = " << i << "\n";

        if (i != gpu_list.size() - 1)
            std::cout << std::endl;
    }
}

void printHelp(const std::string &exeName) {
    std::cout << "RunAsGPU v1.0" << std::endl;
    std::cout << "Usage: " << exeName << " [action] [options] [exec [..args]]\n" << std::endl;
    std::cout << "Actions:" << std::endl;
    std::cout << "  list                          - List all active GPUs" << std::endl;
    std::cout << "  run <command>                 - Run an application with GPU selection\n" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -ui, --unit-id <ID>           - Select a specific GPU by its given ID" << std::endl;
    std::cout << "  -fu, --find-unit <GPU>        - Find a GPU by identification" << std::endl;
    std::cout << "  -fnu, --find-unit-name <N>    - Find a GPU by name" << std::endl;
    std::cout << "  -fvu, --find-unit-vendor <V>  - Find a GPU by vendor name/ID" << std::endl;
    std::cout << "  -fpu, --find-unit-product <P> - Find a GPU by product name/ID" << std::endl;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printHelp(argv[0]);
        return 1;
    }

    std::string action = argv[1];

    if (action == "--help" || action == "-h") {
        printHelp(argv[0]);
        return 0;
    }

    std::vector<GraphicalUnit> gpu_list = Runner::ListGraphicalUnits();
    int unit = 0;

    if (action == "list") {
        list(gpu_list);
        return 0;
    }

    if (action == "run") {
        std::vector<std::string> args;
        for (int i = 2; i < argc; i++) {
            std::string arg = argv[i];

            if (arg == "-ui" || arg == "--unit-id") {
                if (i + 1 < argc) {
                    std::string unitStr = argv[++i];
                    if (unitStr.find_first_not_of("0123456789") == std::string::npos)
                        unit = std::stoi(unitStr);
                    else {
                        std::cerr << "Invalid GPU ID: " << unitStr << "\n";
                        return 1;
                    }
                } else {
                    std::cerr << "Missing value for " << arg << "\n";
                    return 1;
                }
            } else if (arg == "-fu" || arg == "--find-unit") {
                if (i + 1 < argc) {
                    std::string data = argv[++i];
                    unit = findUnit(gpu_list, data, Global);

                    if (unit == -1) {
                        std::cerr << "Invalid data given: " << data << std::endl;
                        return 1;
                    }
                } else {
                    std::cerr << "Missing value for " << arg << "\n";
                    return 1;
                }
            } else if (arg == "-fnu" || arg == "--find-unit-name") {
                if (i + 1 < argc) {
                    std::string data = argv[++i];
                    unit = findUnit(gpu_list, data, Name);

                    if (unit == -1) {
                        std::cerr << "Invalid name: " << data << std::endl;
                        return 1;
                    }
                } else {
                    std::cerr << "Missing value for " << arg << "\n";
                    return 1;
                }
            } else if (arg == "-fvu" || arg == "--find-unit-vendor") {
                if (i + 1 < argc) {
                    std::string data = argv[++i];
                    unit = findUnit(gpu_list, argv[++i], Vendor);

                    if (unit == -1) {
                        std::cerr << "Invalid vendor name/ID: " << argv[i] << "\n";
                        return 1;
                    }
                } else {
                    std::cerr << "Missing value for " << arg << "\n";
                    return 1;
                }
            } else if (arg == "-fpu" || arg == "--find-unit-product") {
                if (i + 1 < argc) {
                    std::string data = argv[++i];
                    unit = findUnit(gpu_list, data, Product);

                    if (unit == -1) {
                        std::cerr << "Invalid product name/ID: " << data << "\n";
                        return 1;
                    }
                } else {
                    std::cerr << "Missing value for " << arg << "\n";
                    return 1;
                }
            } else
                args.push_back(arg);
        }

        if (args.empty()) {
            std::cerr << "No executable specified.\n";
            return 1;
        }

        std::string execPath = args[0];
        args.erase(args.begin());

        std::string command = "DRI_PRIME=" + std::to_string(unit) + " " + execPath;
        for (const auto& arg : args) {
            command += " " + arg;
        }

        std::cout << "Running: " << command << "\n";
        int result = std::system(command.c_str());

        return result == 0 ? 0 : 1;
    }

    std::cerr << "Unknown command: " << action << "\n";
    printHelp(argv[0]);
    return 1;
}