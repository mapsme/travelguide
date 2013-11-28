package com.guidewithme.async;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.util.Enumeration;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;

import android.content.Context;
import android.os.Environment;
import android.util.Log;

public class ZippedGuidesHelper
{
  private final ZipFile mZFile;
  private final Context mContext;

  public ZippedGuidesHelper(Context context, String filename)
  {
    mContext = context;
    try
    {
      final String fullPath = Environment.getExternalStorageDirectory().getAbsoluteFile()
          + File.separator + filename;

      Log.d("ZIPZIP", fullPath);
      mZFile = new ZipFile(fullPath);
    }
    catch (final IOException e)
    {
      throw new RuntimeException(e);
    }
  }

  public InputStream getData(String url)
  {
    try
    {
      final ZipEntry ze = mZFile.getEntry(url);
      Log.d("ZIPZIP", url);
      if (ze == null)
      {
        final Enumeration<? extends ZipEntry> entries = mZFile.entries();
        while (entries.hasMoreElements())
          Log.d("ZIPZIP", entries.nextElement().getName());
      }

      return mZFile.getInputStream(ze);
    }
    catch (final IOException e)
    {
      throw new RuntimeException(e);
    }
  }

}
