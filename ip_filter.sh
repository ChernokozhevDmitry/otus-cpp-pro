#!/bin/sh
cmake --build build
./build/debug/ip_filter.exe ./ip_filter.tsv > ip_filter.txt
md5sum ./ip_filter.txt
