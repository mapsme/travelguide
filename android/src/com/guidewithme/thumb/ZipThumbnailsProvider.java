package com.guidewithme.thumb;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.util.Enumeration;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;

import android.content.Context;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.os.Environment;
import android.util.Log;

public class ZipThumbnailsProvider implements ThumbnailsProvider
{
  private final ZipFile mZFile;
  private final Context mContext;

  public ZipThumbnailsProvider(Context context, String filename)
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

  @Override
  public Drawable getThumbnailByUrl(String url)
  {
    try
    {
      final ZipEntry ze = mZFile.getEntry("data/thumb/" + url);
      Log.d("ZIPZIP", url);
      if (ze == null)
      {
        final Enumeration<? extends ZipEntry> entries = mZFile.entries();
        while (entries.hasMoreElements())
          Log.d("ZIPZIP", entries.nextElement().getName());
      }

      final InputStream is = mZFile.getInputStream(ze);
      return new BitmapDrawable(mContext.getResources(), is);
    }
    catch (final IOException e)
    {
      throw new RuntimeException(e);
    }
  }

}
