#include "reverse_complement/csv_writer.hpp"

#include <fstream>
#include <stdexcept>
#include <string>
#include <string_view>

namespace {

class OutputFile {
public:
    explicit OutputFile(const std::string& path) : stream_(path) {
        if (!stream_) {
            throw std::runtime_error("Could not create CSV file: " + path);
        }
    }

    std::ofstream& stream() noexcept {
        return stream_;
    }

private:
    std::ofstream stream_;
};

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
    OutputFile output(output_path);
    auto& stream = output.stream();

    stream << "sequence_id,description,length,original_sequence,reverse_complement\n";

    for (const auto& result : results) {
        stream << escapeCsv(result.record.id) << ','
               << escapeCsv(result.record.description) << ','
               << result.record.sequence.size() << ','
               << escapeCsv(result.record.sequence) << ','
               << escapeCsv(result.reverse_complement) << '\n';
    }
}
