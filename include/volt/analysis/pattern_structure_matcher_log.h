#pragma once

#include <volt/analysis/structure_identification_log.h>

namespace Volt{

class PatternCatalog;
class PatternClassifier;

// The execution log of one pattern matching run, on top of the lines every structure identification prints.
class PatternStructureMatcherLog : public StructureIdentificationLog{
public:
    using StructureIdentificationLog::dump;

    void dump(const PatternCatalog& catalog);
    void dump(const PatternClassifier& classifier);
};

}
