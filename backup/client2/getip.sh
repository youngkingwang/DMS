#!/bin/bash
LC_ALL=C ifconfig  | grep 'inet addr:'| grep -v '127.0.0.1' |
cut -d: -f2 | awk '{ print $1}'
