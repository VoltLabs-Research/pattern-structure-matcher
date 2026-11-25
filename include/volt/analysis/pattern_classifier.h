#pragma once

#include <volt/analysis/nearest_neighbor_finder.h>
#include <volt/analysis/pattern_catalog.h>
#include <volt/analysis/pattern_runtime_types.h>
#include <volt/analysis/structure_analysis.h>
#include <volt/structures/neighbor_bond_array.h>

#include <memory>
#include <string>
#include <vector>

namespace Volt {

class PatternClassifier {
public:
    explicit PatternClassifier(std::shared_ptr<const PatternCatalog> catalog);

    void setAtomTypes(const std::vector<int>* atomTypes);
    void setSelectedPatternIds(std::vector<int> selectedPatternIds);
    void classify(StructureAnalysis& analysis);
    void configureDxaClustering(StructureAnalysis& analysis) const;

    const std::shared_ptr<const PatternCatalog>& catalog() const{
        return _catalog;
    }

    const std::vector<int>& selectedPatternIds() const{
        return _selectedPatternIds;
    }

    const std::vector<int>& atomPatternIds() const{
        return _atomPatternIds;
    }

    std::shared_ptr<const std::vector<PatternDxaAtomState>> atomDxaStates() const{
        return _atomDxaStates;
    }

private:
    static bool lexicographicallyGreaterMatrix(const Matrix3& lhs, const Matrix3& rhs);
    static void permuteCanonicalToRuntimeBySymmetry(
        const std::vector<int>& canonicalToRuntime,
        const PatternSymmetryPermutation& symmetry,
        int coordinationNumber,
        std::vector<int>& outCanonicalToRuntime
    );
    static double orientationAssignmentError(
        const CompiledPatternLocalMatcher& matcher,
        const NearestNeighborFinder::Query<MAX_NEIGHBORS>& query,
        const std::vector<int>& canonicalToRuntime,
        const Matrix3& orientation
    );
    static bool computeMatchOrientation(
        const CompiledPatternLocalMatcher& matcher,
        const NearestNeighborFinder::Query<MAX_NEIGHBORS>& query,
        const std::vector<int>& canonicalToRuntime,
        Matrix3& outOrientation
    );
    static bool reduceOrientationToFundamentalZone(
        const CompiledPatternLocalMatcher& matcher,
        const NearestNeighborFinder::Query<MAX_NEIGHBORS>& query,
        std::vector<int>& inOutCanonicalToRuntime,
        Matrix3& inOutOrientation
    );
    static bool assignBestOrientationFallbackPermutation(
        const CompiledPatternLocalMatcher& matcher,
        const std::vector<PatternCnaSignature>& runtimeSignatures,
        const NeighborBondArray& runtimeNeighborArray,
        const NearestNeighborFinder::Query<MAX_NEIGHBORS>& query,
        std::vector<int>& outCanonicalToRuntime,
        Matrix3& outOrientation
    );
    static bool matchGenericLocalMatcherImpl(
        const CompiledPattern& pattern,
        const CompiledPatternLocalMatcher& matcher,
        int localMatcherIndex,
        const NearestNeighborFinder& neighFinder,
        int atomIndex,
        const std::vector<int>* atomTypes,
        bool allowCollapsedNeighborShell,
        bool* outRejectedByNeighborGap,
        PatternAtomMatch& outMatch
    );
    static bool matchGenericLocalMatcher(
        const CompiledPattern& pattern,
        const CompiledPatternLocalMatcher& matcher,
        int localMatcherIndex,
        const NearestNeighborFinder& neighFinder,
        int atomIndex,
        const std::vector<int>* atomTypes,
        PatternAtomMatch& outMatch
    );
    static bool matchPatternAtom(
        const CompiledPattern& pattern,
        const NearestNeighborFinder& neighFinder,
        int atomIndex,
        const std::vector<int>* atomTypes,
        PatternAtomMatch& outMatch
    );
    static int patternSelectionPriority(const CompiledPattern& pattern);

    std::shared_ptr<const PatternCatalog> _catalog;
    std::vector<int> _selectedPatternIds;
    std::vector<int> _configuredSelectedPatternIds;
    std::vector<int> _atomPatternIds;
    std::shared_ptr<std::vector<PatternDxaAtomState>> _atomDxaStates;
    const std::vector<int>* _atomTypes = nullptr;
};

}
