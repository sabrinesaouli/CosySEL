#!/bin/bash

MODE="$1"
shift

CNF="$1"
shift

SOLVER="$(dirname $0)/glucose_static"

# SOLVER="gdb --args $(dirname $0)/glucose"
# SOLVER="valgrind --leak-check=full $(dirname $0)/glucose"
# SOLVER="$(dirname $0)/glucose_release"

if [ "$MODE" = "saucy" ]; then
    cat "$CNF" | $(dirname $0)/BreakID -no-row -no-bin -no-small -no-relaxed -s -1 -store-sym "$CNF.sym" > /dev/null
    $SOLVER -model -breakid "$CNF" $@
elif [ "$MODE" = "bliss" ]; then
    $(dirname $0)/CNFBlissSymmetries "$CNF" > "$CNF.bliss"
    $SOLVER -model -bliss "$CNF" $@
else
    echo "ERROR";
fi
