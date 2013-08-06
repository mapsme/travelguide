package com.example.travelguide.thumb;

import java.io.IOException;
import java.io.InputStream;

import android.annotation.SuppressLint;
import android.content.Context;
import android.content.res.AssetManager;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;

public class AssetsThumbnailProvider implements ThumbnailsProvider
{
  private static String DIR = "thumbnails/";
  private Context       mContext;
  private AssetManager  mAssetManager;

  public AssetsThumbnailProvider(Context context)
  {
    mContext = context;
    mAssetManager = mContext.getAssets();
  }

  @SuppressLint("DefaultLocale")
  @Override
  public Drawable getThumbnailByUrl(String url)
  {
    final String fullPath = DIR + url.toLowerCase();
    try
    {
      final InputStream is = mAssetManager.open(fullPath);
      return BitmapDrawable.createFromStream(is, fullPath);
    }
    catch (IOException e)
    {
      throw new RuntimeException("Cant find asset: " + fullPath, e);
    }
  }

}
