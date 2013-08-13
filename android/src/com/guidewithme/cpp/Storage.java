package com.guidewithme.cpp;

import android.content.Context;
import android.content.res.AssetManager;

import com.guidewithme.article.ArticleInfo;

public class Storage
{
  private static  volatile Storage sInstance;

  public static Storage get(Context context)
  {
    if (sInstance == null)
      synchronized (Storage.class)
      {
        if (sInstance == null)
        {
          sInstance = new Storage(context.getAssets());
          sInstance.init();
        }
      }
    return sInstance;
  }

  // We need to store this reference
  // in order to prevent garbage collection
  // see asset_manager_jni.h
  private final AssetManager mAssetManager;

  private Storage(AssetManager assetManager)
  {
    mAssetManager = assetManager;
  }

  private void init()
  {
    nativeInit(mAssetManager);
  }

  private native void nativeInit(Object jAssetManager);

  public native ArticleInfo getArticleInfoByUrl(String url);
  public native void        query(String query, boolean useLocation, double lat, double lon);
  public native int         getResultSize();
  public native ArticleInfo getArticleInfoByIndex(int index);

  public native static boolean isValidLatLon(double lat, double lon);

  static
  {
    System.loadLibrary("tguide");
  }
}
