#!/bin/bash

cat <<- EOH
# Automatically generated ReTunesFree release

This release is automatically generated every time I update ReTunesFree.
As such it is the latest version of the code and may be
unstable, unusable, unsuitable for human consumption, and so on.
I plan on stable releases being available in the Rack Plugin Manager.

These assets were built against
https://vcvrack.com/downloads/Rack-SDK-1.1.6.zip

The build date and most recent commits are:

EOH
date
echo ""
echo "Most recent commits:" 
echo ""
git log --pretty=oneline | head -5
