
URL=https://ftp.postgresql.org/pub/source/v15.2/postgresql-15.2.tar.gz
FILE_NAME=postgresql-15.2.tar.gz
SHA256=eccd208f3e7412ad7bc4c648ecc87e0aa514e02c24a48f71bf9e46910bf284ca
FILE_PATH=${DOWNLOAD_DIR}/${FILE_NAME}
SOURCE_FOLDER=${R_TARGET_DIR}/postgresql-15.2


d_check_rebuild() {
    if [ -f ${DEPS_INSTALL_DIR}/include/libpq/libpq-fs.h ]; then
        echo -n "already build"
    else
        echo -n ""
    fi
}

d_configure() {
    OLD_DIR=$PWD
    cd ${SOURCE_FOLDER}

    ./configure --prefix=${DEPS_INSTALL_DIR} >/dev/null

    cd $OLD_DIR
}

d_build() {
    OLD_DIR=$PWD
    cd ${SOURCE_FOLDER}
    env

    make

    cd $OLD_DIR
}


d_install() {
    OLD_DIR=$PWD
    cd ${SOURCE_FOLDER}

    make install

    cd $OLD_DIR
}


