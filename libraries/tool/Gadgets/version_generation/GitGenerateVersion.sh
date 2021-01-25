#!/bin/bash

# template file
TEMPLATE_FILE="git_version.template"

TEMPLATE_PATH=$2/$TEMPLATE_FILE
VERSION_PATH=$3
echo $TEMPLATE_PATH
echo $VERSION_PATH

# $1 is the prefix of Git Tag, and default is "bb3-v"
if [ ! $1 ]; then
    TAG_HEADER="bb3-v"
else
    TAG_HEADER=$1v
fi
TAG_HEADER_LEN=`echo $TAG_HEADER | wc -c`
#echo $TAG_HEADER_LEN

# get full describe version
FULL_VERSION=`git describe`
# set version to 0.0.0 if no tag name
if [ ! $FULL_VERSION ]; then
    FULL_VERSION=$TAG_HEADER"0.0.0"
fi
#echo $FULL_VERSION

# remove tag header
PROJ_VERSION=`echo $FULL_VERSION | cut -c $TAG_HEADER_LEN-`
#echo $PROJ_VERSION

# split version to five fields
TAG_VERSION=`echo $PROJ_VERSION | awk -F '-' '{print $1}'`
#echo $TAG_VERSION
MAJOR=`echo $TAG_VERSION | awk -F '.' '{print $1}'`
MINOR=`echo $TAG_VERSION | awk -F '.' '{print $2}'`
# App: get revision from SDK tag
#REVISION=`echo $TAG_VERSION | awk -F '.' '{print $3}'`
# Patch: using count number as revision
REVISION=`echo $PROJ_VERSION | awk -F '-' '{print $2}'`
if [ ! $REVISION ]; then
    REVISION=0
fi

#get branch name
CUSTOMER_NAME=$4
if [ ! $CUSTOMER_NAME ]; then
    BRANCH_NAME=`git symbolic-ref --short -q HEAD`
    CUSTOMER_NAME=`echo $BRANCH_NAME | awk -F '-' '{print $2}'`

    if [ ! $CUSTOMER_NAME ]; then
        CUSTOMER_NAME="unknown"
    fi
fi
#echo $CUSTOMER_NAME
CN_1=${CUSTOMER_NAME:0:1}
CN_2=${CUSTOMER_NAME:1:1}
CN_3=${CUSTOMER_NAME:2:1}
if [ ! $CN_3 ]; then
    CN_3=#
fi
CN_4=${CUSTOMER_NAME:3:1}
if [ ! $CN_4 ]; then
    CN_4=#
fi
CN_5=${CUSTOMER_NAME:4:1}
if [ ! $CN_5 ]; then
    CN_5=#
fi
CN_6=${CUSTOMER_NAME:5:1}
if [ ! $CN_6 ]; then
    CN_6=#
fi
CN_7=${CUSTOMER_NAME:6:1}
if [ ! $CN_7 ]; then
    CN_7=#
fi
CN_8=${CUSTOMER_NAME:7:1}
if [ ! $CN_8 ]; then
    CN_8=#
fi

#get commit ID
GIT_CMTID_RAW=`git log --pretty=oneline -1 | cut -c 1-8`
GIT_CMTID=0x$GIT_CMTID_RAW

#echo $MAJOR.$MINOR.$REVISION.$BUILDNUM
#echo $GIT_CMTID

# build time
BUILDING_TIME=`env LANG=en_US.UTF-8 date '+%a %b %e %R:%S %Y'`
#echo $BUILDING_TIME

# substitute version&time in template
`cat $TEMPLATE_PATH | sed -e "s/MAJOR_T/$MAJOR/g" \
                     -e "s/MINOR_T/$MINOR/g" \
                     -e "s/REVISION_T/$REVISION/g" \
                     -e "s/BUILDTIME_T/$BUILDING_TIME/g"    \
                     -e "s/GITCMTID_T/$GIT_CMTID/g" \
                     -e "s/CUSTOMER_NAME_T/$CUSTOMER_NAME/g" \
                     -e "s/CN_1_T/$CN_1/g" \
                     -e "s/CN_2_T/$CN_2/g" \
                     -e "s/CN_3_T/$CN_3/g" \
                     -e "s/CN_4_T/$CN_4/g" \
                     -e "s/CN_5_T/$CN_5/g" \
                     -e "s/CN_6_T/$CN_6/g" \
                     -e "s/CN_7_T/$CN_7/g" \
                     -e "s/CN_8_T/$CN_8/g" \
                    > $VERSION_PATH`

# show result
echo "Generated Version File :"
cat $VERSION_PATH
