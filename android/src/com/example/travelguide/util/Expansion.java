package com.example.travelguide.util;

import java.io.File;

import android.os.Environment;

public class Expansion
{

  public static String getMainObbFileName(String version, String packageName)
  {
    final String path = String.format("main.%s.%s.obb", version, packageName);
    return path;
  }

  public static String getObbLocation(String packageName)
  {
    final String location = Environment.getExternalStorageDirectory() + File.separator
                            + "Android" + File.separator
                            + "obb" + File.separator
                            + packageName;
    return location;
  }

  public static String getMainObbPath(String version, String packageName)
  {
    final String mainObbPath = getObbLocation(packageName) + File.separator
                               + getMainObbFileName(version, packageName);
    return mainObbPath;
  }

  public static boolean isObbFilePresent(String version, String packageName)
  {
    final String obbPath = getMainObbPath(version, packageName);
    final boolean exists = Utils.fileExists(obbPath);
    return exists;
  }

  private Expansion() {}
}
