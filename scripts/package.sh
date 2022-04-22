#!/usr/bin/bash

# get LIBAA_PACKAGE_NAME from ae_version.h
#eval $(sed -n 's/^#define  *\([^ ]*\)  *\(.*\) *$/export \1=\2/p' build-android-armv7/install/include/aa_version.h)

NAME=libaa
MERGE0_LIB_NAME=rubberband
MERGE1_LIB_NAME=samplerate

##### package android lib
ANDROIDPKGNAME=${NAME}-android-lib
rm -rf $ANDROIDPKGNAME
mkdir -p $ANDROIDPKGNAME
mkdir -p $ANDROIDPKGNAME/armeabi-v7a
mkdir -p $ANDROIDPKGNAME/arm64-v8a
mkdir -p $ANDROIDPKGNAME/include

cp build-android-armv7/install/lib/lib*.a $ANDROIDPKGNAME/armeabi-v7a/
cp build-android-aarch64/install/lib/lib*.a $ANDROIDPKGNAME/arm64-v8a/
cp -r build-android-aarch64/install/include/* $ANDROIDPKGNAME/include/
rm -f $ANDROIDPKGNAME.zip
zip -9 -r $ANDROIDPKGNAME.zip $ANDROIDPKGNAME

##### package ios framework
IOSPKGNAME=${NAME}-ios-lib
rm -rf $IOSPKGNAME
mkdir -p $IOSPKGNAME
mkdir -p $IOSPKGNAME/lib
mkdir -p $IOSPKGNAME/include
cp -r build-ios/install/include/* $IOSPKGNAME/include
cp build-ios/install/lib/* $IOSPKGNAME/lib
rm -f $IOSPKGNAME.zip
zip -9 -y -r $IOSPKGNAME.zip $IOSPKGNAME