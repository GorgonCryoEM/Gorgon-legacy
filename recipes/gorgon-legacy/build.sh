#!/bin/sh

build_dir="${SRC_DIR}/../build_dir"

rm -rf $build_dir
mkdir -p $build_dir
cd $build_dir

cmake $SRC_DIR
make -j Legacy

$PYTHON setup_legacy.py install
