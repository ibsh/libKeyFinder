#!/bin/bash

################################################################################
# Generate ubuntu packages: libkeyfinder
################################################################################

# Error checking
function check_error {
    if [ $? -gt 0 ]; then
        echo "ERROR ! ABORTING !"
		exit
	fi
}

# Usage
function usage {
    echo ""
    echo "Usage: generate_libkeyfinder_deb.sh [ppa_type]"
    echo ""
    echo "    [ppa_type]  'test' (for the test PPA url) or 'prod'"
    echo ""
    exit
}

# Check parameters
if [ $# -ne 1 ]; then
    usage
fi

# Select PPA
if [[ $1 == test ]] ; then
    PPAPATH=julien-rosener/keyfinder-test
elif [[ $1 == prod ]] ; then
    PPAPATH=julien-rosener/keyfinder
else
    usage
fi

echo "****************************** Install tools ****************************"
sudo apt-get install packaging-dev build-essential dh-make
check_error
echo ""
echo ""

echo "************************* Get version from .pro ************************"
VERSION=$(cat ../LibKeyFinder.pro | grep 'VERSION =' | cut -d'=' -f2 | tr -d ' ')
echo VERSION = $VERSION
check_error
echo ""
echo ""

echo "*************************** Prepare environment *************************"
# Main vars
VERSIONPACKAGE=$VERSION-0ubuntu1
WORKINGPATH=$HOME/libkeyfinder_$VERSION-make_package
DEBPATH=$WORKINGPATH/deb
SOURCEDIR=libkeyfinder_source
TARPACK=libkeyfinder_$VERSION.orig.tar.gz
ORIGDIR=$(pwd)
DISTRIB=$(lsb_release -cs)
export DEBEMAIL=julien.rosener@digital-scratch.org
export DEBFULLNAME="Julien Rosener"
export EDITOR=vim

rm -rf $WORKINGPATH
check_error
mkdir -v $WORKINGPATH
check_error
echo ""
echo ""

echo "**************************** Copy source code ***************************"
git checkout changelog
check_error
cd ..
git archive --format zip --output $WORKINGPATH/archive.zip `git rev-parse --abbrev-ref HEAD`
unzip $WORKINGPATH/archive.zip -d $WORKINGPATH/$SOURCEDIR
check_error
cd debian
echo ""
echo ""

echo "************************* Update changelog ******************************"
ORIGDIR=$(pwd)
cd $WORKINGPATH/$SOURCEDIR
debchange --newversion $VERSIONPACKAGE --distribution $DISTRIB
check_error
cat $WORKINGPATH/$SOURCEDIR/debian/changelog
cp $WORKINGPATH/$SOURCEDIR/debian/changelog $ORIGDIR
check_error
echo ""
echo ""

echo "************************* Compress source directory *********************"
cd $WORKINGPATH
tar cvzf $TARPACK $SOURCEDIR/
echo ""
echo ""

echo "***************************** Create Linux base *************************"
export BUILDUSERID=$USER
cd $WORKINGPATH/$SOURCEDIR
if [ ! -f ~/pbuilder/$DISTRIB-base.tgz ]
then
    pbuilder-dist $DISTRIB create
fi
pbuilder-dist $DISTRIB update
echo ""
echo ""

echo "************Parse debian/ config file and create source.changes *********"
debuild -S -sa
check_error
cd ../
echo ""
echo ""

echo "***************************** Create test DEB files *********************"
mkdir -v $DEBPATH
pbuilder-dist $DISTRIB build --buildresult $DEBPATH *.dsc
check_error
echo ""
echo ""

echo "************ Upload source.changes on Launchpad at ppa:$PPAPATH *************"
dput -f ppa:$PPAPATH *source.changes
check_error
echo ""
echo ""

echo "    Done, testing DEBs are in $DEBPATH"

