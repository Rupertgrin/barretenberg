#!/usr/bin/env bash

# Default values
DEFAULT_PLONK_FLAVOUR="ultra"
DEFAULT_CIRCUIT_FLAVOUR="blake"
DEFAULT_INPUTS="1,2,3,4"

# Read input arguments with defaults
PLONK_FLAVOUR="${1:-$DEFAULT_PLONK_FLAVOUR}"
CIRCUIT_FLAVOUR="${2:-$DEFAULT_CIRCUIT_FLAVOUR}"
INPUTS="${3:-$DEFAULT_INPUTS}"

# Path to the binaries and SRS
BIN="../cpp/build/bin/solidity_proof_gen"
SRS_PATH="../cpp/srs_db/ignition"

# Normalize input by removing newlines
INPUTS=$(sed 's/\\n//g' <<< "$INPUTS")

# Check if the plonk flavour is 'honk' and update the binary path
if [ "$PLONK_FLAVOUR" == "honk" ]; then
    BIN="../cpp/build/bin/honk_solidity_proof_gen"
fi

# Ensure the binary file exists
if [ ! -x "$BIN" ]; then
    echo "Error: Binary file $BIN not found or not executable."
    exit 1
fi

# Ensure the SRS path exists
if [ ! -d "$SRS_PATH" ]; then
    echo "Error: SRS path $SRS_PATH does not exist."
    exit 1
fi

# Run the binary with the provided arguments
"$BIN" "$PLONK_FLAVOUR" "$CIRCUIT_FLAVOUR" "$SRS_PATH" "$INPUTS"
