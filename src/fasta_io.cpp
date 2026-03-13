#include "reverse_complement/fasta_io.hpp"

#include <cctype>
#include <fstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

namespace {

class InputFile {
public:
    explicit InputFile(const std::string& path) : stream_(path) {
        if (!stream_) {
            throw std::runtime_error("Could not open FASTA file: " + path);
        }
    }

    std::ifstream& stream() noexcept {
        return stream_;
    }

private:
    std::ifstream stream_;
};

std::string trimCarriageReturn(std::string line) {
    if (!line.empty() && line.back() == '\r') {
        line.pop_back();
    }
    return line;
}

std::string normalizeSequenceLine(std::string_view line) {
    std::string normalized;
    normalized.reserve(line.size());

    for (const char ch : line) {
        if (!std::isspace(static_cast<unsigned char>(ch))) {
            normalized.push_back(static_cast<char>(std::toupper(static_cast<unsigned char>(ch))));
        }
    }

    return normalized;
}

std::pair<std::string, std::string> parseHeader(std::string_view header) {
    const std::size_t first_space = header.find_first_of(" \t");
    if (first_space == std::string_view::npos) {
        return {std::string(header), {}};
    }

    return {
        std::string(header.substr(0, first_space)),
        std::string(header.substr(first_space + 1))
    };
}

void finalizeRecord(const FastaRecord& record) {
    if (record.sequence.empty()) {
        throw std::runtime_error("FASTA record '" + record.id + "' has no sequence.");
    }
}

}  // namespace

std::vector<FastaRecord> FastaReader::read(const std::string& input_path) const {
    InputFile input(input_path);
    std::vector<FastaRecord> records;
    FastaRecord current;
    bool in_record = false;
    std::string line;

    while (std::getline(input.stream(), line)) {
        line = trimCarriageReturn(std::move(line));
        if (line.empty()) {
            continue;
        }

        if (line.front() == '>') {
            if (in_record) {
                finalizeRecord(current);
                records.push_back(std::move(current));
                current = {};
            }

            in_record = true;
            std::tie(current.id, current.description) = parseHeader(line.substr(1));
            if (current.id.empty()) {
                throw std::runtime_error("Encountered FASTA header with no identifier.");
            }
        } else {
            if (!in_record) {
                throw std::runtime_error("FASTA content found before the first header line.");
            }

            current.sequence += normalizeSequenceLine(line);
        }
    }

    if (in_record) {
        finalizeRecord(current);
        records.push_back(std::move(current));
    }

    if (records.empty()) {
        throw std::runtime_error("No FASTA records were found in: " + input_path);
    }

    return records;
}
