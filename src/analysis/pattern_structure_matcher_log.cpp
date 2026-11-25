#include <volt/analysis/pattern_structure_matcher_log.h>

#include <volt/analysis/pattern_catalog.h>
#include <volt/analysis/pattern_classifier.h>
#include <volt/core/type_counts.h>

#include <spdlog/spdlog.h>

#include <algorithm>
#include <numeric>
#include <string>
#include <vector>

namespace Volt{

void PatternStructureMatcherLog::dump(const PatternCatalog& catalog){
    for(const PatternCatalog::MissingPayload& payload : catalog.missingPayloads()){
        if(!payload.definitionFound){
            spdlog::warn("Lattice '{}' is missing its definition file '{}'", payload.lattice, payload.definitionPath);
        }else{
            spdlog::warn("Skipping template payload for lattice '{}': {}", payload.lattice, payload.error);
        }
    }

    std::string names;
    std::string unmatchableNames;
    for(const CompiledPattern& pattern : catalog.patterns()){
        names += (names.empty() ? "" : ", ") + pattern.name;
        if(!pattern.supportedForLocalMatching){
            unmatchableNames += (unmatchableNames.empty() ? "" : ", ") + pattern.name;
        }
    }
    spdlog::info("Loaded {} lattice patterns in {}: {}", catalog.patterns().size(), phaseElapsed(), names);
    if(!unmatchableNames.empty()){
        spdlog::warn("Skipping lattice patterns without a usable local matcher: {}", unmatchableNames);
    }
}

void PatternStructureMatcherLog::dump(const PatternClassifier& classifier){
    const PatternCatalog& catalog = *classifier.catalog();
    const std::vector<int>& atomPatternIds = classifier.atomPatternIds();
    std::vector<std::size_t> patternCounts(catalog.patterns().size(), 0);
    for(const int patternId : atomPatternIds){
        if(patternId >= 0 && patternId < static_cast<int>(patternCounts.size())) ++patternCounts[patternId];
    }
    const std::size_t atomCount = atomPatternIds.size();
    const std::size_t matchedAtoms = std::accumulate(patternCounts.begin(), patternCounts.end(), std::size_t{0});
    spdlog::info(
        "Matched {} of {} atoms ({:.1f}%) in {}",
        matchedAtoms, atomCount, percentOf(matchedAtoms, atomCount), phaseElapsed()
    );

    // Most common first, then the unmatched atoms as Other.
    std::vector<int> matchedPatternIds;
    for(const CompiledPattern& pattern : catalog.patterns()){
        if(patternCounts[static_cast<std::size_t>(pattern.id)] > 0){
            matchedPatternIds.push_back(pattern.id);
        }
    }
    std::stable_sort(matchedPatternIds.begin(), matchedPatternIds.end(), [&](int lhs, int rhs){
        return patternCounts[static_cast<std::size_t>(lhs)] > patternCounts[static_cast<std::size_t>(rhs)];
    });
    TypeCounts counts{"Structure types"};
    counts.atoms = atomCount;
    for(const int patternId : matchedPatternIds){
        counts.counts.emplace_back(catalog.patternById(patternId).name, patternCounts[static_cast<std::size_t>(patternId)]);
    }
    counts.counts.emplace_back("Other", atomCount - matchedAtoms);
    dump(counts);

    if(matchedAtoms == 0){
        spdlog::warn("No atom matched any of the selected patterns");
    }
}

}
