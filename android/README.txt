To compile guides apk you need to create local.properties file with two values:
sdk.dir=/Full/path/to/android/sdk
ndk.dir=/Full/path/to/android/ndk

and you need to clone mapsme android api repo (don't forget to update it if already cloned):
cd 3rdparty; git clone git@github.com:mapsme/api-android.git

Use
./gradlew installDebug (or iD) to build and install debug version or
./gradlew assembleRelease (or aR) to create release apk (installRelease to test it on a device)