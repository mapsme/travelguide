package com.guidewithme.util;

import java.io.File;

import android.os.Environment;

public class Expansion
{
  public static final String KEY = null;

  public static String getPath(String packageName)
  {
    return findPackageObbFile(packageName);
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

  public static String findPackageObbFile(String packageName)
  {
    final String[] pathsToLook = { getObbLocation(packageName),
                                   // For development purpose - obbs can be in MWM folder, if MWM is installed too
                                   Environment.getExternalStorageDirectory() + File.separator + "MapsWithMe",
                                   // For user-testing, to directly download obb to the device by http link
                                   Environment.getExternalStorageDirectory() + File.separator + Environment.DIRECTORY_DOWNLOADS };
    for (final String pathToLook : pathsToLook)
    {
      final File obbDir = new File(pathToLook);
      if (!obbDir.exists())
        continue;
      final String[] filesInDir = obbDir.list();
      for (final String fileName : filesInDir)
        if (fileName.endsWith(packageName + ".obb"))
          return obbDir.getAbsolutePath() + File.separator + fileName;
    }
    // obb was not found :(
    return null;
  }

  private Expansion() {}
}
