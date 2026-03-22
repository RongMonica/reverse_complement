#pragma once

#include <string>

struct FastaRecord {
    std::string id;
    std::string description;
    std::string sequence;
};

struct SequenceResult {
    FastaRecord record;
    std::string reverse_complement;
    std::string gc_content;
};
