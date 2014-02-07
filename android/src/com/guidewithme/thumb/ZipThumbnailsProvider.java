package com.guidewithme.thumb;

import java.io.IOException;
import java.io.InputStream;
import java.util.Enumeration;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;

import android.content.Context;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.util.Log;

import com.guidewithme.util.CollectionUtils;
import com.guidewithme.util.CollectionUtils.Predicate;

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

  @SuppressWarnings("unchecked")
  @Override
  public Drawable getThumbnailByUrl(String url)
  {
    try
    {
      final String thumbsPrefix = "data/thumb/";
      ZipEntry ze = mZFile.getEntry(thumbsPrefix + url);
      if (ze == null)
      {
        Log.e("GWM " + mContext.getPackageName(), "null entry:" + url);

        // Provide random icon in that case
        final Predicate<ZipEntry> isThumbEntry = new Predicate<ZipEntry>()
        {
          @Override
          public boolean apply(ZipEntry arg)
          {
            return arg.getName().startsWith(thumbsPrefix);
          }
        };
        ze = CollectionUtils.any(CollectionUtils.filter((Enumeration<ZipEntry>)mZFile.entries(), isThumbEntry));
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
