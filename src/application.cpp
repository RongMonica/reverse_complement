#include "reverse_complement/application.hpp"

#include "reverse_complement/csv_writer.hpp"
#include "reverse_complement/fasta_io.hpp"
#include "reverse_complement/sequence_processing.hpp"

#include <iostream>
#include <string>

namespace {

void printUsage(const char* program_name) {
    std::cerr << "Usage: " << program_name << " <input.fasta> <output.csv>\n";
    std::cerr << "Example: " << program_name << " sample.fasta results.csv\n";
}

}  // namespace

int Application::run(int argc, char* argv[]) const {
    if (argc != 3) {
        printUsage(argv[0]);
        return 1;
    }

    const std::string input_path = argv[1];
    const std::string output_path = argv[2];

    const FastaReader fasta_reader;
    const ParallelSequenceProcessor sequence_processor;
    const CsvWriter csv_writer;

    const auto records = fasta_reader.read(input_path);
    const auto results = sequence_processor.process(records);
    csv_writer.write(output_path, results);

    std::cout << "Processed " << results.size()
              << " FASTA record(s) into " << output_path << '\n';
    return 0;
}
