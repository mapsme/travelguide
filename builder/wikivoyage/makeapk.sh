export PATH=~/android-ndk-r9/:$PATH

set -e -u

cat countries_to_generate.txt | while read country; do

    rm ../../android/assets/index.dat || echo "No previous index found."
    cp -f Countries/$country/index.dat ../../android/assets/

    rm  ../../android/build/apk/* || true
    rm Countries/$country/*.apk || true

    pushd ../../android/
    ./gradlew "-PGWMpn=com.guidewithme.`echo $country|tr '[:upper:]' '[:lower:]'`" "-PGWMapk=Guide With Me $country"  "-PGWMappName=Guide With Me $country" clean assembleRelease
    popd

    cp  ../../android/build/apk/* Countries/$country/
done
