#!/bin/bash
set -e -u -x

LOCAL_DIRNAME="${PWD}/$(dirname "$0")"

TARGET_SDK="iphoneos"
PROJECT_BUILDDIR="${LOCAL_DIRNAME}/Builds"
GUIDES_PROVISIONING_DIR="${LOCAL_DIRNAME}/Profiles"
SYSTEM_PROVISIONING_DIR=~/Library/MobileDevice/Provisioning\ Profiles
KEYCHAIN=~/Library/Keychains/iOS.keychain
DEFAULT_KEYCHAIN=~/Library/Keychains/login.keychain
KEYCHAIN_PASSWORD="ioskeychain"
BUILD_CONFIGURATION="Production"
DATE=$(date +%F)
APP_DSYM_FOLDER="${PROJECT_BUILDDIR}/APP_AND_DSYM-${DATE}"
RESULT_DIR="${PROJECT_BUILDDIR}"

declare arr=(Czech_Republic UK Japan Russia Spain Hawaii California Germany Italy France Turkey Switzerland Greece New_Zealand Thailand Taiwan \
             Estonia Poland Latvia Lithuania Sri_Lanka Hong_Kong Nepal Singapore Malaysia)

# copy all provisioning profiles first
cp "$GUIDES_PROVISIONING_DIR"/* "${SYSTEM_PROVISIONING_DIR}"
# if launched with any parameter, build adhoc versions
if [ $# == 1 ]; then
  RESULT_DIR="${RESULT_DIR}/Adhoc-${DATE}"
  # To correctly sign AdHoc versions from target's XCode project settings, we need to delete all Distribution provisioning
  rm "${SYSTEM_PROVISIONING_DIR}"/*_GuideWithMe.mobileprovision
else
  RESULT_DIR="${RESULT_DIR}/AppStore-${DATE}"
  # To correctly sign Distribution versions from target's XCode project settings, we need to delete AdHoc provisioning
  rm "${SYSTEM_PROVISIONING_DIR}"/MapsWithMe_AdHoc.mobileprovision
fi

if [ -d "$APP_DSYM_FOLDER" ]; then
  rm -rf "${APP_DSYM_FOLDER}"
fi
mkdir -p "$APP_DSYM_FOLDER"

if [ -d "$RESULT_DIR" ]; then
  rm -rf "${RESULT_DIR}"
fi
mkdir -p "$RESULT_DIR"

if [ $# == 1 ]; then
  cp "$LOCAL_DIRNAME"/index.php  "$RESULT_DIR"/index.php
  chmod g+w "$RESULT_DIR"/
  chown :_www "$RESULT_DIR"/
fi

# switch to the keychain with private key
security list-keychains -s "$KEYCHAIN"
# need to unlock keychain to avoid "User interaction is not allowed" error
security unlock -p "$KEYCHAIN_PASSWORD" "$KEYCHAIN"
# keep keychain unlocked for ten hours
security set-keychain-settings -t 36000 -l "$KEYCHAIN"

for i in ${arr[@]}
do
  xcodebuild -target "${i}" -sdk "${TARGET_SDK}" -configuration "${BUILD_CONFIGURATION}" SYMROOT="${PROJECT_BUILDDIR}/${i}"
  xcrun -sdk "${TARGET_SDK}" PackageApplication -v "${PROJECT_BUILDDIR}/${i}/${BUILD_CONFIGURATION}-${TARGET_SDK}/${i}.app" -o "${RESULT_DIR}/${i}.ipa"
  cp -r "${PROJECT_BUILDDIR}/${i}/${BUILD_CONFIGURATION}-${TARGET_SDK}/"* "$APP_DSYM_FOLDER"
  rm -rf "${PROJECT_BUILDDIR}/${i}"
done

zip -r -9 "${RESULT_DIR}/APP_AND_DSYM.zip" "${APP_DSYM_FOLDER}"
rm -rf "${APP_DSYM_FOLDER}"

# switch back to the default keychain
security list-keychains -s "$DEFAULT_KEYCHAIN"
