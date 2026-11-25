#pragma once

#include <volt/analysis/pattern_runtime_types.h>

#include <filesystem>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace Volt {

bool loadPatternTemplateSources(
    const std::filesystem::path& latticeDirectory,
    std::vector<PatternTemplateSource>& outSources,
    std::string* errorMessage
);

bool readTemplateStructureFile(
    const std::filesystem::path& filePath,
    TemplateStructureData& outData,
    std::string* errorMessage
);

CompiledPattern compilePattern(
    const PatternTemplateSource& source,
    const TemplateStructureData* templateData
);

namespace PatternSourceLoaderDetail{

// The pattern a lattice YAML defines; throws when the file cannot be read as one.
PatternTemplateSource parsePatternSourceFile(const std::filesystem::path& filePath);

}

class PatternCatalog {
public:
    // A lattice compiled without its template payload: its definition file is missing, or could not be read
    // for the reason `error` gives.
    struct MissingPayload{
        std::string lattice;
        std::string definitionPath;
        bool definitionFound = false;
        std::string error;
    };

    static std::shared_ptr<const PatternCatalog> loadFromDirectory(const std::string& latticeDirectory);

    const std::filesystem::path& latticeDirectory() const{
        return _latticeDirectory;
    }

    const std::vector<CompiledPattern>& patterns() const{
        return _patterns;
    }

    const CompiledPattern& patternById(int patternId) const;
    const CompiledPattern* findPatternByName(std::string_view patternName) const;

    std::vector<int> defaultSelection() const;

    const std::vector<MissingPayload>& missingPayloads() const{
        return _missingPayloads;
    }

private:
    PatternCatalog(
        std::filesystem::path latticeDirectory,
        std::vector<CompiledPattern> patterns,
        std::vector<MissingPayload> missingPayloads
    );

    std::filesystem::path _latticeDirectory;
    std::vector<CompiledPattern> _patterns;
    std::vector<MissingPayload> _missingPayloads;
    std::unordered_map<std::string, int> _nameToId;
};

// The neighbor shells and symmetries compilePattern() builds a pattern's local matchers from.
namespace PatternCompilerDetail{

inline constexpr double kPatternShellDistanceTolerance = 1e-6;

struct PatternNeighborCandidate {
    Vector3 vector = Vector3::Zero();
    int species = 0;
    double distance = 0.0;
};

struct NeighborShellRange {
    int begin = 0;
    int count = 0;
    double averageDistance = 0.0;
};

bool approximatelySameDistance(double lhs, double rhs);
std::vector<NeighborShellRange> buildNeighborShellRanges(
    const std::vector<PatternNeighborCandidate>& neighbors,
    int coordinationNumber
);
NeighborShellRange selectReferenceShell(const std::vector<NeighborShellRange>& shells);
std::vector<PatternNeighborCandidate> gatherPeriodicNeighbors(
    const TemplateStructureData& templateData,
    int centerAtomIndex,
    int imageRadius
);
Vector3 pointToVector(const Point3& point);
int findIdentitySymmetryIndex(const std::vector<PatternSymmetryPermutation>& symmetries);
const Vector3& transformedCanonicalNeighborVector(
    const CompiledPatternLocalMatcher& matcher,
    int symmetryIndex,
    int slot
);
int countMatchingLocalOverlap(
    const CompiledPatternLocalMatcher& matcher,
    int currentSymmetry,
    int neighborSlot,
    int neighborSymmetry,
    int reverseSlot
);
int appendOrResolveLocalMatcherIndex(
    std::vector<CompiledPatternLocalMatcher>& localMatchers,
    CompiledPatternLocalMatcher matcher
);
bool requiresOrientationFallback(const CompiledPatternLocalMatcher& matcher);
void initializeIdentitySymmetry(
    const std::vector<Vector3>& canonicalNeighborVectors,
    std::vector<PatternSymmetryPermutation>& symmetries
);
void retainProperRotations(std::vector<PatternSymmetryPermutation>& symmetries);
void buildGenericSymmetries(
    const std::vector<Vector3>& canonicalNeighborVectors,
    std::vector<PatternSymmetryPermutation>& symmetries
);
bool compileGenericLocalMatcherForAtom(
    const PatternTemplateSource& source,
    const TemplateStructureData& templateData,
    int atomIndex,
    CompiledPatternLocalMatcher& outMatcher
);
void compileGenericLocalMatchers(
    CompiledPattern& pattern,
    const PatternTemplateSource& source,
    const TemplateStructureData& templateData
);

}

}
