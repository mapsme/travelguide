export PATH=~/android-ndk-r9/:$PATH
cat countries_to_generate.txt | while read country; do
	cp -f $country/index.dat ../../android/assets/
	rm  ../../android/build/apk/*
	rm $country/*.apk
	pushd ../../android/
		./gradlew "-PGWMpn=com.guidewithme.`echo $country|tr '[:upper:]' '[:lower:]'`" "-PGWMapk=Guide With Me $country"  "-PGWMappName=Guide With Me $country" assemble
	popd
	cp  ../../android/build/apk/* $country/
done