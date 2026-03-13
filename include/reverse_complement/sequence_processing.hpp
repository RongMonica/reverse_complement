#pragma once

#include "reverse_complement/models.hpp"

#include <string>
#include <string_view>
#include <vector>

class ReverseComplementer {
public:
    [[nodiscard]] std::string operator()(std::string_view sequence) const;
};

class ParallelSequenceProcessor {
public:
    [[nodiscard]] std::vector<SequenceResult> process(const std::vector<FastaRecord>& records) const;
};
