#ifndef RUNASGPU_RUNNER_HPP
#define RUNASGPU_RUNNER_HPP

#include <RunAsGPU/Shared/GraphicalUnit.hpp>

#include <vector>
#include <map>

namespace Runner {

    std::vector<GraphicalUnit> ListGraphicalUnits();

    void RunApplication(const GraphicalUnit& unit, const std::string& bin, const std::vector<std::string>& args, const std::map<std::string, std::string>& env_map);

}

#endif //RUNASGPU_RUNNER_HPP
