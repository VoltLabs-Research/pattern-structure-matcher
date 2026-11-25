#pragma once

#include <volt/core/lammps_parser.h>

#include <nlohmann/json.hpp>

#include <memory>
#include <string>
#include <vector>

namespace Volt {

class AnalysisContext;
class PatternCatalog;
class StructureAnalysis;
struct OrientationClusterAtomState;
struct PatternDxaAtomState;

using json = nlohmann::json;

class PatternStructureMatchingService {
public:
    PatternStructureMatchingService();

    void setLatticeDirectory(std::string latticeDirectory);
    void setReferenceLatticeDirectory(std::string referenceLatticeDirectory);
    void setSelectedPatterns(std::string selectedPatternsCsv);
    void setDissolveSmallClusters(bool dissolveSmallClusters);

    json compute(
        const LammpsParser::Frame& frame,
        const std::string& outputBase,
        const std::string& inputDumpPath
    );

private:
    static bool resolveSelectedPatternIds(
        const PatternCatalog& catalog,
        const std::string& csv,
        std::vector<int>& outPatternIds,
        std::string& outError
    );
    static std::shared_ptr<const std::vector<PatternDxaAtomState>> smoothOrientationFallbackStates(
        const PatternCatalog& catalog,
        const std::shared_ptr<const std::vector<PatternDxaAtomState>>& atomStates
    );
    static std::shared_ptr<std::vector<OrientationClusterAtomState>> buildOrientationClusterStates(
        const std::shared_ptr<const std::vector<PatternDxaAtomState>>& atomStates
    );
    static void recomputeClusterOrientations(StructureAnalysis& analysis, AnalysisContext& context);

    std::string _latticeDirectory;
    std::string _referenceLatticeDirectory;
    std::string _selectedPatternsCsv;
    bool _dissolveSmallClusters = false;
};

}
