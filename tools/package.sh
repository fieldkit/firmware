#!/bin/bash

set -xe

BUILD=$1
FK_VERSION=$2
FK_VERSION_PATH=${FK_VERSION//\//_}
PACKAGE=fk-firmware-${FK_VERSION_PATH}
PROJECT=${BUILD}/..

mkdir -p ${BUILD}/${PACKAGE}
cp ${PROJECT}/tools/flash-* ${BUILD}/${PACKAGE}
cp ${PROJECT}/tools/jlink-* ${BUILD}/${PACKAGE}
cp ${BUILD}/samd51/bootloader/fkbl.elf ${BUILD}/${PACKAGE}
cp ${BUILD}/samd51/bootloader/fkbl-fkb.bin ${BUILD}/${PACKAGE}
cp ${BUILD}/samd51/fk/fk-bundled-fkb.elf ${BUILD}/${PACKAGE}
cp ${BUILD}/samd51/fk/fk-bundled-fkb.bin ${BUILD}/${PACKAGE}
cp ${BUILD}/samd09/modules/weather/sidecar/fk-weather-sidecar*.elf ${BUILD}/${PACKAGE}
cp ${BUILD}/samd09/modules/weather/sidecar/fk-weather-sidecar*.bin ${BUILD}/${PACKAGE}
# rsync -a --exclude .ccls-cache --exclude "*.lib" --exclude "*.tar*" --exclude ".git" --exclude "build" --exclude "gcc-arm-none-*" ${PROJECT}/third-party ${BUILD}/${PACKAGE}
# rsync -a --exclude .ccls-cache --exclude "*.lib" --exclude "*.tar*" --exclude ".git" --exclude "build" ${PROJECT}/fk ${BUILD}/${PACKAGE}
# rsync -a --exclude .ccls-cache --exclude "*.lib" --exclude "*.tar*" --exclude ".git" --exclude "build" ${PROJECT}/boards ${BUILD}/${PACKAGE}
# rsync -a --exclude .ccls-cache --exclude "*.lib" --exclude "*.tar*" --exclude ".git" --exclude "build" ${PROJECT}/modules ${BUILD}/${PACKAGE}
chmod 755 ${BUILD}/${PACKAGE}/flash-*
chmod 755 ${BUILD}/${PACKAGE}/jlink-*
touch ${BUILD}/${PACKAGE}/fk.cfg-disabled
cd ${BUILD} && zip -r ${PACKAGE}.zip ${PACKAGE}
cp ${BUILD}/${PACKAGE}.zip ${BUILD}/fk-firmware.zip
