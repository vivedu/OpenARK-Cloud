#!/usr/bin/env bash
# @see https://github.com/vivedu/OpenARK-Cloud/issues/5

git clone https://github.com/augcog/OpenARK
cd OpenARK && mkdir build && cd build
cmake ..
make -j4
