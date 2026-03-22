#include "reverse_complement/csv_writer.hpp"

#include <fstream>
#include <stdexcept>
#include <string>
#include <string_view>

namespace {

std::string escapeCsv(std::string_view value) {
    bool needs_quotes = false;
    std::string escaped;
    escaped.reserve(value.size());

    for (const char ch : value) {
        if (ch == '"' || ch == ',' || ch == '\n' || ch == '\r') {
            needs_quotes = true;
        }

        if (ch == '"') {
            escaped += "\"\"";
        } else {
            escaped += ch;
        }
    }

    if (needs_quotes) {
        return "\"" + escaped + "\"";
    }

    return escaped;
}

}  // namespace

void CsvWriter::write(const std::string& output_path, const std::vector<SequenceResult>& results) const {
    std::ofstream stream(output_path);
    if(!stream) {
        throw std::runtime_error("Failed to open output file: " + output_path);
    }    
    stream << "sequence_id,description,length,GC content,ambigous base count, original_sequence,reverse_complement\n";

    for (const auto& result : results) {
        stream << escapeCsv(result.record.id) << ','
               << escapeCsv(result.record.description) << ','
               << result.record.sequence.size() << ','
               << result.gc_content << ','
               << result.ambigous_base_count << ','
               << escapeCsv(result.record.sequence) << ','
               << escapeCsv(result.reverse_complement) << '\n';
    }
}
