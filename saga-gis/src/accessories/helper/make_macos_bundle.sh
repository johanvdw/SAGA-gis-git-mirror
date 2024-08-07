#!/bin/bash -e

# brew install dylibbundler

SRCDIR=./saga-code
APPDIR=./SAGA.app

rm -rf "${APPDIR}"
mkdir "${APPDIR}"
mkdir "${APPDIR}/Contents"
mkdir "${APPDIR}/Contents/Frameworks"
mkdir "${APPDIR}/Contents/MacOS"
mkdir "${APPDIR}/Contents/MacOS/proj-data"
mkdir "${APPDIR}/Contents/MacOS/gdal-data"
mkdir "${APPDIR}/Contents/PlugIns"
mkdir "${APPDIR}/Contents/Resources"
mkdir "${APPDIR}/Contents/Tools"

cp -Rf /usr/local/bin/saga_gui "${APPDIR}/Contents/MacOS"
cp -Rf /usr/local/bin/saga_cmd "${APPDIR}/Contents/MacOS"
cp -Rf /usr/local/share/saga/saga_prj.dic "${APPDIR}/Contents/MacOS"
cp -Rf /usr/local/share/saga/saga_prj.srs "${APPDIR}/Contents/MacOS"
cp -Rf /usr/local/share/saga/saga_tip.txt "${APPDIR}/Contents/MacOS/saga_gui.tip"
cp -Rf /usr/local/lib/saga/*.dylib "${APPDIR}/Contents/Tools"
cp -Rf /usr/local/share/saga/toolchains/*.xml "${APPDIR}/Contents/Tools"
cp -Rf /usr/local/share/proj/* "${APPDIR}/Contents/MacOS/proj-data"
cp -Rf /usr/local/share/gdal/* "${APPDIR}/Contents/MacOS/gdal-data"
cp -Rf "${SRCDIR}/saga-gis/src/saga_core/saga_gui/res/saga.plist" "${APPDIR}/Contents/Info.plist"
cp -Rf "${SRCDIR}/saga-gis/src/saga_core/saga_gui/res/saga.icns" "${APPDIR}/Contents/Resources"

rm "${APPDIR}/Contents/MacOS/proj-data/"*.tif

dylibbundler -cd -b -s /usr/local/lib/ -x "${APPDIR}/Contents/MacOS/saga_cmd" -d "${APPDIR}/Contents/libs/" -p @executable_path/../libs/

for file in "${APPDIR}/Contents/Tools"/*.dylib; do
    dylibbundler -of -b -s /usr/local/lib/ -x "${file}" -d "${APPDIR}/Contents/libs/" -p @executable_path/../libs/
done

dylibbundler -of -b -s /usr/local/lib/ -x "${APPDIR}/Contents/MacOS/saga_gui" -d "${APPDIR}/Contents/libs/" -p @executable_path/../libs/
