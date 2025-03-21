#!/bin/bash

set -e

if [[ -z "$OUT_BINDIR" || -z "$OUT_PACKAGE" || -z "$RESDIR" || -z "$VERSION_METAINFO" ]]; then
    echo "Environment variables not set! Run the script like this:"
    echo "   export OUT_BINDIR=Builds/Linux-x86_64"
    echo "   export OUT_PACKAGE=Builds/DebBuild"
    echo "   export RESDIR=Resources"
    echo "   export VERSION_METAINFO=1.0-2"
    echo "   ./Scripts/deb_build/make_debpkg.sh"
    exit 1
fi

rm -rf "$OUT_PACKAGE"
mkdir -p "$OUT_PACKAGE/DEBIAN"
mkdir -p "$OUT_PACKAGE/usr/bin"
mkdir -p "$OUT_PACKAGE/usr/share/applications"
mkdir -p "$OUT_PACKAGE/usr/share/icons/hicolor/256x256/apps"

cp Scripts/deb_build/DEBIAN/control "$OUT_PACKAGE/DEBIAN/control"

cp "$OUT_BINDIR/RunAsGPU" "$OUT_PACKAGE/usr/bin/"
cp "$OUT_BINDIR/RunAsGPU-CLI" "$OUT_PACKAGE/usr/bin/"

cp "$RESDIR/RunAsGPU.desktop" "$OUT_PACKAGE/usr/share/applications/"

cp "$RESDIR/RunAsGPU-Icon.png" "$OUT_PACKAGE/usr/share/icons/hicolor/256x256/apps/RunAsGPU.png"

chmod 755 "$OUT_PACKAGE/usr/bin/RunAsGPU"
chmod 755 "$OUT_PACKAGE/usr/bin/RunAsGPU-CLI"
chmod 644 "$OUT_PACKAGE/usr/share/applications/RunAsGPU.desktop"
chmod 644 "$OUT_PACKAGE/usr/share/icons/hicolor/256x256/apps/RunAsGPU.png"

dpkg-deb --build "$OUT_PACKAGE" "$OUT_PACKAGE/RunAsGPU-amd64-${VERSION_METAINFO}.deb"

echo "Debian package built: RunAsGPU-amd64-${VERSION_METAINFO}.deb"