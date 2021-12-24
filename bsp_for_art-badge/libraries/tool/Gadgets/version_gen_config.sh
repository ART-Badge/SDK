#!/bin/bash

# Paths
VERSION_TOOL_PATH="../../../tool/Gadgets/version_generation/GitGenerateVersion.sh"
TAG_HEADER="bb3-sdk-"
TEMPLATE_FILE_PATH="../../../tool/Gadgets/version_generation"
TARGET_VERSION_FILE_PATH="../../../inc/platform/version.h"

# Customer Name
CUSTOMER_NAME="sdk-v1"

# call version generation tool to generate the version file
$VERSION_TOOL_PATH $TAG_HEADER $TEMPLATE_FILE_PATH $TARGET_VERSION_FILE_PATH $CUSTOMER_NAME
