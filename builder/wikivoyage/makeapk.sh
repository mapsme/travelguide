set -e -u

cat countries_to_generate.txt | while read country; do

    # copy index
    rm ../../android/assets/index.dat || echo "No previous index found."
    cp -f Countries/$country/content/data/index.dat ../../android/assets/

    # copy resources
    toCopy=(drawable-ldpi drawable-mdpi drawable-hdpi \
        drawable-xhdpi drawable-xxhdpi)

    COUNTRY_RES_DIR="../../android/icons/$country"
    if [ ! -d "$COUNTRY_RES_DIR" ]; then
        COUNTRY_RES_DIR="../../android/icons/default"
        echo "ERROR: Icons are missing for $country - default icons are used"
    fi
    for resDir in ${toCopy[*]}
    do
        cp -f $COUNTRY_RES_DIR/$resDir/* ../../android/res/$resDir
        echo "Copied $resDir for $country"
    done

    rm  ../../android/build/apk/* || true
    rm Countries/$country/*.apk || true

    # make packages lower case and dot-separated
    PACKAGE="com.guidewithme."$(echo "$country" | tr '[:upper:]' '[:lower:]' | \
        sed 's/_/\./g')

    # hack for UK
    if [[ "$PACKAGE" == *united.kingdom ]]
    then
       PACKAGE=com.guidewithme.uk
    fi

    # remove underscores from title
    TITLE=$(echo "$country" | sed 's/_/ /g')

    pushd ../../android/
    ./gradlew "-PGWMpn=$PACKAGE" "-PGWMapk=GuideWithMe $country" \
        "-PGWMappName=GuideWithMe $TITLE" clean assembleRelease
    popd

    cp  ../../android/build/outputs/apk/*release.apk Countries/$country/
done
