set -e -u

cat countries_to_generate.txt | while read country; do

    # make packages lower case and dot-separated
    PACKAGE="com.guidewithme."$(echo "$country" | tr '[:upper:]' '[:lower:]' | \
	     sed 's/_/\./g')

    # hack for UK
    if [[ "$PACKAGE" == *united.kingdom ]]
    then
       PACKAGE=com.guidewithme.uk
    fi

    # copy js, css
    cp -r ../../data/* Countries/$country/content/data

    echo "Generation .obb for package: $PACKAGE"

    # jobb content
    ../../tools/jobb \
      -d Countries/$country/content \
      -o Countries/$country/main.1.$PACKAGE.obb \
      -pn $PACKAGE -pv 1

 done
