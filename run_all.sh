#!/usr/bin/env bash
set -e
echo "1) Generating data…"
./build/data_generator

echo && echo "2) Ground‑truth quantiles…"
./build/ground_truth data/normal_data.txt

echo && echo "3) Streaming t‑Digest…"
./build/tdigest_main data/normal_data.txt 500 1000 5000 4

echo && echo "4) Single‑thread benchmark…"
./build/benchmark
