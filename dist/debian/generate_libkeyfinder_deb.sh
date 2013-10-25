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
    echo "Usage: generate_libkeyfinder_deb.sh [archi]"
    echo ""
    echo "    [archi]  'amd64' or 'i386'"
    echo ""
    exit
}

# Check parameters
if [ $# -ne 1 ]; then
    usage
fi

# Get archi
if [[ $1 == amd64 ]] ; then
    ARCHI=amd64
elif [[ $1 == i386 ]] ; then
    ARCHI=i386
else
    usage
fi

echo "****************************** Install tools ****************************"
sudo apt-get install packaging-dev build-essential dh-make reprepro
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
REPOPATH=$(readlink -f ../../../gh-pages/debian/)
VERSIONPACKAGE=$VERSION-1
WORKINGPATH=$HOME/libkeyfinder_$VERSION-make_package
SOURCEDIR=libkeyfinder_source
TARPACK=libkeyfinder_$VERSION.orig.tar.gz
ORIGDIR=$(pwd)
DISTRIB=stable
DEBBIN=libkeyfinder0_${VERSIONPACKAGE}_${ARCHI}.deb
DEBDEV=libkeyfinder-dev_${VERSIONPACKAGE}_${ARCHI}.deb
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
cd $WORKINGPATH/$SOURCEDIR
debchange --newversion $VERSIONPACKAGE --distribution $DISTRIB
check_error
cat $WORKINGPATH/$SOURCEDIR/debian/changelog
cp $WORKINGPATH/$SOURCEDIR/debian/changelog $ORIGDIR/debian
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
sudo pbuilder --clean
sudo pbuilder --create --architecture $ARCHI --distribution $DISTRIB
echo ""
echo ""

echo "************Parse debian/ config file and create DEBs *********"
pdebuild --architecture $ARCHI
check_error
cd ../
echo ""
echo ""

echo "************ Show content of packages *********"
cp /var/cache/pbuilder/result/${DEBBIN} $WORKINGPATH
cp /var/cache/pbuilder/result/${DEBDEV} $WORKINGPATH
echo "$WORKINGPATH/${DEBBIN}"
dpkg -c $WORKINGPATH/${DEBBIN}
echo ""
echo "$WORKINGPATH/${DEBDEV}"
dpkg -c $WORKINGPATH/${DEBDEV}
check_error
cd ../
echo ""
echo ""

echo "************ Install package to local apt repo $REPOPATH *************"
cd $ORIGDIR
cd $REPOPATH
#reprepro -A $ARCHI remove $DISTRIB libkeyfinder0
#reprepro -A $ARCHI remove $DISTRIB libkeyfinder-dev
reprepro --ask-passphrase -Vb . includedeb stable $WORKINGPATH/${DEBBIN}
reprepro --ask-passphrase -Vb . includedeb stable $WORKINGPATH/${DEBDEV}
check_error
cd $ORIGDIR
echo ""
echo ""

echo "    Done."

