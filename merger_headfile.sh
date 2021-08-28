#!/bin/env bash
cat ./include/cyaron.hpp > cyaron.hpp

headers="
include/core/base.hpp
include/core/traits.hpp
include/core/exec.hpp
include/random.hpp
include/random_string.hpp
include/utils.hpp
include/io.hpp
include/compare.hpp
include/graph.hpp
"

for header in $headers
do
    cat $header >> cyaron.hpp
done
