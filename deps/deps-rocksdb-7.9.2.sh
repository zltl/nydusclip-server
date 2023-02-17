
URL=https://github.com/facebook/rocksdb/archive/refs/tags/v7.9.2.zip
FILE_NAME=rocksdb-7.9.2.zip
SHA256=ce7a498718df53b3a11f64578b283025c6cdd34bf7379a8e838b84eef212178c
FILE_PATH=${DOWNLOAD_DIR}/${FILE_NAME}
SOURCE_FOLDER=${R_TARGET_DIR}/rocksdb-7.9.2


d_check_rebuild() {
    if [ -f ${DEPS_INSTALL_DIR}/include/rocksdb/db.h ]; then
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
          -DWITH_GFLAGS=OFF \
          -DROCKSDB_BUILD_SHARED=OFF \
          -DWITH_TESTS=OFF \
          ..
    cd ..

    cd $OLD_DIR
}


