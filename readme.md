# How to build guide for a new country

To add new country to generation list please follow next steps:

1. Add country's name from file *all_countries.txt* to
 file *countries_to_generate.txt*
2. Add default image for this country into the folder *default_images*
this image will be used for articles which don't have image (or we dont have
image for them). It's name must be *&lt;country_name&gt;.jpg*.
3. Remove file *process_html* if exists to update data.

Now you'r ready to build new amazing guides with me!
* run `bash build.sh` to prepare data for guides and Android obb files.

## iOS - add new country

1. Duplicate any existing iOS target and give it a name of new country
2. Some target's proj settings are set to default values and should be fixed/set equal to other targets
3. Set *PRODUCT_NAME* in target project properties equal to a name of new country
4. Set *BUNDLE_ID* in target properties to *com.guidewithme.newcountry* (all lowercase, dots instead of spaces)
5. Set *URL_SCHEME* in target properties to *guidewithme-newcountry* (all lowercase)
6. Add all necessary app icons to the corresponding folder
7. Fix new and duplicated target icons "target membership" checkboxes
8. Drag "../builder/wikivoyage/Countries/<Country Name>/content/data" folder to the Resources and create folder reference
9. Add new target name to iOS/build.sh to use Jenkins autobuild
10. Create new app/key in Flurry and add it to iOS/offlineguides/Statistics.m file

To build new guide locally from XCode, you should copy guide data generated by builder to iOS/Countries/New_Country folder.
In this folder there is content/data folder with *.html files, css, js, images and thumb subfolders


## Android notes

### IMPORTANT
* Don't forget to edit file *android/src/com/guidewithme/expansion/KeyMap.java*
Please add Google Play public license key at the end of the *COUTRY_2_KEY* map AND increment *KEY_COUNT* constant. 
* Add icons for every resolution to *android/icons/CountryName/*

There is one script you need to use to build guides for Android:
 **build-with-android.sh**. It places it's result (obb, apk) into countries
 folder *Countries/&lt;country_name&gt;*.

Android applications have package name made of lowercased country name
and underscores replaced with dotes, added to "com.guidewithme.". For instance:
*United_States -> com.guidewithme.united.states*


### United_Kingdom hack
We have package name "com.guidewithme.uk" for it for the sake compatibility
with first release. obb and apk files are generated accordingly.
