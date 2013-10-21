<html><body><h2><center>Guide With Me AD HOCs</center><h2><br /><hr />

<?php
function curPageURL() {
 $pageURL = 'http';
 if ($_SERVER["HTTPS"] == "on") {$pageURL .= "s";}
 $pageURL .= "://";
 if ($_SERVER["SERVER_PORT"] != "80") {
  $pageURL .= $_SERVER["SERVER_NAME"].":".$_SERVER["SERVER_PORT"].$_SERVER["REQUEST_URI"];
 } else {
  $pageURL .= $_SERVER["SERVER_NAME"].$_SERVER["REQUEST_URI"];
 }
 return $pageURL;
}

$sTestUrlStart = "<a href=\"itms-services://?action=download-manifest&url=";
$currentUrl = curPageURL();
$sTestUrlEnd = ".plist\">";
$sUrlEndText = "</a>";

//plist content
$sPlistStart = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>
<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">
<plist version=\"1.0\">
<dict>
    <key>items</key>
    <array>
        <dict>
            <key>assets</key>
            <array>
                <dict>
                    <key>kind</key>
                    <string>software-package</string>
                    <key>url</key>
                    <string>";
$sPlist2 = ".ipa</string>
                </dict>
            </array>
            <key>metadata</key>
            <dict>
                <key>bundle-identifier</key>
                <string>com.guidewithme.";

$sPlist3  =   "</string>
                <key>bundle-version</key>
                <string>1.0</string>
                <key>kind</key>
                <string>software</string>
                <key>title</key>
                <string>GuideWithMe ";
$sPlistEnd =  "</string>
            </dict>
        </dict>
    </array>
</dict>
</plist>";


foreach (glob("*.ipa") as $filename) {
      	$fileNameWithoutExt = pathinfo($filename, PATHINFO_FILENAME);
        echo $sTestUrlStart;
      	echo $currentUrl;
      	echo $fileNameWithoutExt;
      	echo $sTestUrlEnd;
      	echo $fileNameWithoutExt;
      	echo "<br>";
        //create plist
      	$plistContent = $sPlistStart . $currentUrl . $fileNameWithoutExt . $sPlist2 . strtolower($fileNameWithoutExt) . $sPlist3 . $fileNameWithoutExt . $sPlistEnd;
      	$plistName = $fileNameWithoutExt .'.plist';
        file_put_contents($plistName, $plistContent);
      }
?>
</body></html>
