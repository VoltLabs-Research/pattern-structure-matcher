#pragma once

#include <volt/analysis/pattern_catalog.h>
#include <volt/structures/neighbor_bond_array.h>

#include <cstddef>
#include <vector>

namespace Volt {

namespace PatternMatchingDetail{

// Maps each canonical neighbor slot from `canonicalSlot` on to a runtime slot with the same CNA signature and the
// same bonds to the slots mapped before it.
bool assignGenericCnaPermutationRecursive(
    const CompiledPatternLocalMatcher& matcher,
    const std::vector<PatternCnaSignature>& runtimeSignatures,
    const NeighborBondArray& runtimeNeighborArray,
    std::vector<int>& canonicalToRuntime,
    std::vector<unsigned char>& usedRuntimeSlots,
    int canonicalSlot
);

// Inline: it is checked for every atom a pattern is matched on.
inline bool atomTypesAvailable(const std::vector<int>* atomTypes, std::size_t atomCount){
    return atomTypes != nullptr && atomTypes->size() >= atomCount;
}

}

PatternCnaSignature computePatternCnaSignature(
    const NeighborBondArray& neighborArray,
    int neighborIndex,
    int coordinationNumber
);

bool matchSpeciesWithSymmetry(
    const CompiledPatternLocalMatcher& matcher,
    const std::vector<int>& canonicalNeighborAtomIndices,
    const std::vector<int>* atomTypes,
    int centerAtomIndex,
    bool allowEmptySymmetryFallback
);

bool assignGenericCnaPermutation(
    const CompiledPatternLocalMatcher& matcher,
    const std::vector<PatternCnaSignature>& runtimeSignatures,
    const NeighborBondArray& runtimeNeighborArray,
    std::vector<int>& canonicalToRuntime
);

bool equivalentLocalMatchers(
    const CompiledPatternLocalMatcher& lhs,
    const CompiledPatternLocalMatcher& rhs
);

}
