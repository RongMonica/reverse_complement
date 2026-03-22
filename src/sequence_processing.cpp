#include "reverse_complement/sequence_processing.hpp"

#include <algorithm>
#include <array>
#include <cctype>
#include <future>
#include <stdexcept>
#include <thread>
#include <utility>

namespace {

constexpr std::array<char, 26> buildComplementTable() {
    std::array<char, 26> table{};
    table['A' - 'A'] = 'T';
    table['B' - 'A'] = 'V';
    table['C' - 'A'] = 'G';
    table['D' - 'A'] = 'H';
    table['G' - 'A'] = 'C';
    table['H' - 'A'] = 'D';
    table['K' - 'A'] = 'M';
    table['M' - 'A'] = 'K';
    table['N' - 'A'] = 'N';
    table['R' - 'A'] = 'Y';
    table['S' - 'A'] = 'S';
    table['T' - 'A'] = 'A';
    table['V' - 'A'] = 'B';
    table['W' - 'A'] = 'W';
    table['Y' - 'A'] = 'R';
    return table;
}

char complementBase(char base) {
    static constexpr auto complements = buildComplementTable();
    const char upper = static_cast<char>(std::toupper(static_cast<unsigned char>(base)));

    if (upper < 'A' || upper > 'Z') {
        throw std::runtime_error(std::string("Unsupported nucleotide found: '") + base + "'");
    }

    const char complement = complements[static_cast<std::size_t>(upper - 'A')];
    if (complement == '\0') {
        throw std::runtime_error(std::string("Unsupported nucleotide found: '") + base + "'");
    }

    return complement;
}

std::string percentGC(std::string_view sequence){
    if(sequence.empty()) return 0;
    int cnt = 0;
    for(char c : sequence){
        char upper = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
        if(upper == 'G' || upper == 'C') cnt++;
    }
    return std::to_string((cnt * 100) / sequence.size()) + "%";
}

std::size_t chooseWorkerCount(std::size_t task_count) {
    const std::size_t available = std::max<std::size_t>(1, std::thread::hardware_concurrency());
    return std::min(task_count, available);
}

std::vector<SequenceResult> processChunk(
    const std::vector<FastaRecord>& records,
    std::size_t begin,
    std::size_t end,
    const ReverseComplementer& reverse_complementer
) {
    std::vector<SequenceResult> chunk;
    chunk.reserve(end - begin);

    for (std::size_t index = begin; index < end; ++index) {
        chunk.push_back(SequenceResult{
            records[index],
            reverse_complementer(records[index].sequence),
            percentGC(records[index].sequence)
        });
    }

    return chunk;
}

}  // namespace

std::string ReverseComplementer::operator()(std::string_view sequence) const {
    std::string result(sequence.size(), '\0');
    std::transform(sequence.rbegin(), sequence.rend(), result.begin(), complementBase);
    return result;
}

std::vector<SequenceResult> ParallelSequenceProcessor::process(const std::vector<FastaRecord>& records) const {
    if (records.empty()) {
        return {};
    }

    const std::size_t worker_count = chooseWorkerCount(records.size());
    const std::size_t chunk_size = (records.size() + worker_count - 1) / worker_count;
    const ReverseComplementer reverse_complementer;

    std::vector<std::future<std::vector<SequenceResult>>> tasks;
    tasks.reserve(worker_count);

    for (std::size_t begin = 0; begin < records.size(); begin += chunk_size) {
        const std::size_t end = std::min(begin + chunk_size, records.size());
        tasks.push_back(std::async(std::launch::async, [begin, end, &records, &reverse_complementer] {
            return processChunk(records, begin, end, reverse_complementer);     
        }));
    }

    std::vector<SequenceResult> results;
    results.reserve(records.size());

    for (auto& task : tasks) {
        auto chunk = task.get();
        std::move(chunk.begin(), chunk.end(), std::back_inserter(results));
    }

    return results;
}
