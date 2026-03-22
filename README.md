# Reverse Complement Tool

Small C++ bioinformatics utility that:

- reads DNA sequences from a FASTA file
- computes the GC content for each record
- computes the reverse complement for each record
- exports the results to a CSV file

## Features

- Supports multi-record FASTA files
- Handles lowercase or uppercase input
- Supports common IUPAC DNA symbols: `A C G T R Y S W K M B D H V N`
- Uses modern C++ concurrency with parallel sequence processing
- Uses RAII-based file and application components for safer resource handling
- Produces CSV columns for sequence ID, description, length, original sequence, and reverse complement

## Project Layout

- `include/reverse_complement/`: public headers
- `src/main.cpp`: minimal program entry point
- `src/application.cpp`: CLI flow and orchestration
- `src/fasta_io.cpp`: FASTA parsing and validation
- `src/sequence_processing.cpp`: reverse complement logic and parallel processing
- `src/csv_writer.cpp`: CSV export

## Build

```bash
cmake -S . -B build
cmake --build build
```

## Run

```bash
./build/reverse_complement sample.fasta output.csv
```

## CSV Output

The generated CSV file contains:

- `sequence_id`
- `description`
- `length`
- `original_sequence`
- `reverse_complement`

## Example

Input FASTA:

```fasta
>seq1 Example sequence
ATGCCGTA
```

Output CSV row:

```csv
seq1,Example sequence,8,ATGCCGTA,TACGGCAT
```
