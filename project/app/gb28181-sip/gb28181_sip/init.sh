#!/bin/bash

WORKSPACE=`pwd`
cd $WORKSPACE/3rd_party

VERSION=4.1.0
CRESS_COMPILE=arm-rockchip830-linux-uclibcgnueabihf

# libosip2
tar xvf libosip2-$VERSION.tar.gz
cd libosip2-$VERSION

./configure --host=$CRESS_COMPILE --prefix=$WORKSPACE
make -j48 install

cd $WORKSPACE/3rd_party
rm -rf libosip2-$VERSION
# --------------------------------------------------

# libexosip2
tar xvf libeXosip2-$VERSION.tar.gz
cd libeXosip2-$VERSION

./configure --host=$CRESS_COMPILE --disable-openssl --prefix=$WORKSPACE LDFLAGS="-L$WORKSPACE/lib" CFLAGS="-L$WORKSPACE/include"
make -j48 install

cd $WORKSPACE/3rd_party
rm -rf libeXosip2-$VERSION
# ----------------------------------------------------

# mxml
tar xvf mxml-4.0.3.tar.gz
cd mxml-4.0.3

./configure --host=$CRESS_COMPILE  --prefix=$WORKSPACE
make -j48 install

cd $WORKSPACE/3rd_party
rm -rf mxml-4.0.3


# rm unused files
cd $WORKSPACE

rm -rf share
rm -rf lib/pkgconfig
# rm lib/*.dylib || true
# rm lib/*.so* || true
# rm lib/*.la || true

export LD_LIBRARY_PATH=$WORKSPACE/lib:$LD_LIBRARY_PATH
