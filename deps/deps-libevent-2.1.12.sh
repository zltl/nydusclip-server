
URL=https://github.com/libevent/libevent/releases/download/release-2.1.12-stable/libevent-2.1.12-stable.tar.gz
FILE_NAME=libevent-2.1.12-stable.tar.gz
SHA256=92e6de1be9ec176428fd2367677e61ceffc2ee1cb119035037a27d346b0403bb
FILE_PATH=${DOWNLOAD_DIR}/${FILE_NAME}
SOURCE_FOLDER=${R_TARGET_DIR}/libevent-2.1.12-stable


d_check_rebuild() {
    if [ -f ${DEPS_INSTALL_DIR}/include/event2/event.h ]; then
        echo -n "already build"
    else
        echo -n ""
    fi
}

d_configure() {
    OLD_DIR=$PWD
    cd ${SOURCE_FOLDER}

    DBG=Release
#    if [ -z ${RELEASE} ]; then
#        DBG=Debug
#    fi

    # try cmake
    mkdir -p build
    cd build
    cmake -DCMAKE_INSTALL_PREFIX=${DEPS_INSTALL_DIR} \
          -DCMAKE_BUILD_TYPE=${DBG} \
          ..
    cd ..

    cd $OLD_DIR
}


