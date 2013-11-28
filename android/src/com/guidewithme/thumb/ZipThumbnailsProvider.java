package com.guidewithme.thumb;

import java.io.IOException;
import java.io.InputStream;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;

import android.content.Context;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;

public class ZipThumbnailsProvider implements ThumbnailsProvider
{
  private final ZipFile mZFile;
  private final Context mContext;

  public ZipThumbnailsProvider(Context context, String filename)
  {
    mContext = context;
    try
    {
      mZFile = new ZipFile(filename);
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
      final InputStream is = mZFile.getInputStream(ze);
      return new BitmapDrawable(mContext.getResources(), is);
    }
    catch (final IOException e)
    {
      throw new RuntimeException(e);
    }
  }

}
