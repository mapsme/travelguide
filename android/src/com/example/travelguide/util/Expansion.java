package com.example.travelguide.util;

import java.io.File;

import android.os.Environment;

public class Expansion
{
  public static final String PACKAGE = "com.susanin.travelguide";

  public static final XAPKFile DATA_FILE = new XAPKFile(true, 1, 251904055);

  public static class XAPKFile
  {
    public final boolean isMain;
    public final int versionCode;
    public final long fileSize;

    public XAPKFile(boolean isMain, int versionCode, long fileSizeInBytes)
    {
      this.isMain = isMain;
      this.versionCode = versionCode;
      this.fileSize = fileSizeInBytes;
    }
  }

  public static String getPath()
  {
    return Expansion.getMainObbPath(String.valueOf(DATA_FILE.versionCode), PACKAGE);
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

  private Expansion() {}
}
