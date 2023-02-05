
URL=https://github.com/google/leveldb/archive/refs/tags/1.23.zip
FILE_NAME=leveldb-1.23.zip
SHA256=a6fa7eebd11de709c46bf1501600ed98bf95439d6967963606cc964931ce906f
FILE_PATH=${DOWNLOAD_DIR}/${FILE_NAME}
SOURCE_FOLDER=${R_TARGET_DIR}/leveldb-1.23


d_check_rebuild() {
    if [ -f ${DEPS_INSTALL_DIR}/include/leveldb/db.h ]; then
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
          -DLEVELDB_BUILD_TESTS=OFF \
          -DLEVELDB_BUILD_BENCHMARKS=OFF \
          ..
    cd ..

    cd $OLD_DIR
}


