#pragma once

#include "reverse_complement/models.hpp"

#include <string>
#include <vector>

class FastaReader {
public:
    [[nodiscard]] std::vector<FastaRecord> read(const std::string& input_path) const;
};
