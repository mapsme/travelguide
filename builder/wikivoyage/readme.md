# How to add country

To add new country to genenarion list follow next 3 steps:

1. Add country's name from file *all_countries.txt* to
 file *countries_to_generate.txt*
2. Add default image  for this country into the folder *default_images*
this image will be used for articles which don't have image (or we dont have
image for them). It's name must be *<country_name>.jpg*.
3. Remove file *process_html* if exists to update data.

Now you'r ready to build new amazing guides with me!
* run `bash build.sh` to prepare data for guides
* or run `bash build-with-android.sh` to prepare data and android files.


## More Android notes

There is one script you need to use to build guides for Android:
 **build-with-android.sh**. It places it's result (obb, apk) into countries
 folder *Counties<country_name>*.

Android applications have package name made of lowercased country name
and underscores replaced with dotes, added to "com.guidewithme.". For instance:
*Unated_States -> com.guidewithme.unated.states*


### United_Kingdom hack
We have package name "com.guidewithme.uk" for it for the sake compatibility
with first release. obb and apk files are generated accordingly.
