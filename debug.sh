#!/bin/bash
docker build . -t build-djgpp
docker run -it --rm -v "${PWD}":/tmp/game build-djgpp
make run
