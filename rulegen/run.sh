#!/bin/bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )


$SCRIPT_DIR/astgen $SCRIPT_DIR/rule.txt $SCRIPT_DIR/ast.hpp $SCRIPT_DIR/prepend.cc -- $SCRIPT_DIR/append.cc
