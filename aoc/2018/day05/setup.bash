#!/bin/bash

for c in {a..z}; do
    mkdir -p inputs/${c}
    cp input inputs/${c}/input_001
done
cp input inputs/input_001
