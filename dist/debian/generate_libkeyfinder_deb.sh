#!/bin/bash

################################################################################
# Generate Debian packages: libkeyfinder
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
    echo "Usage: generate_libkeyfinder_deb.sh [repo_type]"
    echo ""
    echo "    [repo_type]  'test' (for the test repository url) or 'prod'"
    echo ""
    exit
}

# Check parameters
if [ $# -ne 1 ]; then
    usage
fi

# Select PPA
if [[ $1 == test ]] ; then
    REPOPATH=julien-rosener/digitalscratch-test
elif [[ $1 == prod ]] ; then
    REPOPATH=julien-rosener/digitalscratch
else
    usage
fi

echo "****************************** Install tools ****************************"
sudo apt-get install packaging-dev build-essential dh-make
check_error
echo ""
echo ""

echo "************************* Get version from .pro ************************"
VERSION=$(cat ../../LibKeyFinder.pro | grep 'VERSION =' | cut -d'=' -f2 | tr -d ' ')
echo VERSION = $VERSION
check_error
echo ""
echo ""

echo "*************************** Prepare environment *************************"
# Main vars
VERSIONPACKAGE=$VERSION-1
WORKINGPATH=$HOME/libkeyfinder_$VERSION-make_package
DEBPATH=$WORKINGPATH/deb
SOURCEDIR=libkeyfinder_source
TARPACK=libkeyfinder_$VERSION.orig.tar.gz
ORIGDIR=$(pwd)
DISTRIB=stable
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
git checkout debian/changelog
check_error
cd ../../
git archive --format zip --output $WORKINGPATH/archive.zip `git rev-parse --abbrev-ref HEAD`
unzip $WORKINGPATH/archive.zip -d $WORKINGPATH/$SOURCEDIR
check_error
echo ""
echo ""

echo "**************************** Install debian/ folder ***************************"
cp -r $WORKINGPATH/$SOURCEDIR/dist/debian/debian $WORKINGPATH/$SOURCEDIR/
check_error
echo ""
echo ""

echo "************************* Update changelog ******************************"
cd dist/debian/debian
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
    sudo pbuilder --create --distribution $DISTRIB
fi
sudo pbuilder --update --distribution $DISTRIB
echo ""
echo ""

echo "************Parse debian/ config file and create source.changes *********"
debuild -b -sa
check_error
cd ../
echo ""
echo ""

echo "***************************** Create test DEB files *********************"
mkdir -v $DEBPATH
pbuilder build --distribution $DISTRIB --buildresult $DEBPATH *.dsc
check_error
echo ""
echo ""

echo "************ Upload source.changes on Launchpad at ppa:$REPOPATH *************"
#dput -f ppa:$REPOPATH *source.changes
check_error
echo ""
echo ""

echo "    Done, testing DEBs are in $DEBPATH"

