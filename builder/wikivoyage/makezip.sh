set -e -u

cat countries_to_generate.txt | while read country; do

    DATA_PATH="Countries/$country/content/data"
    # copy js, css
    cp -r ../../data/* $DATA_PATH

    echo "Generation .zip for : $country"

    # zip content with pushd to avoid adding full path
    pushd Countries/$country/content
      rm ../$country.data.zip || true;
      zip -r \
	      ../$country.data.zip \
	      data
    popd

    echo "Zipped $country"
done
