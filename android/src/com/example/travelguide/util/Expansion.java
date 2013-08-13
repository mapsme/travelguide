package com.example.travelguide.util;

import java.io.File;

import android.os.Environment;

public class Expansion
{
  public static String getPath(String packageName)
  {
    return findFirstObbFile(packageName);
  }

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

  public static String findFirstObbFile(String packageName)
  {
    final File obbDir = new File(getObbLocation(packageName));
    if (obbDir.list() == null)
      return null;

    for (final String filePath : obbDir.list())
    {
      if (filePath.endsWith(".obb"))
        return obbDir.getAbsolutePath() + File.separator + filePath;
    }

    return null;
  }

  private Expansion() {}
}
