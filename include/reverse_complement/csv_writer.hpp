#pragma once

#include "reverse_complement/models.hpp"

#include <string>
#include <vector>

class CsvWriter {
public:
    void write(const std::string& output_path, const std::vector<SequenceResult>& results) const;
};
