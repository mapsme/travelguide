package com.example.travelguide.cpp;

import android.content.Context;
import android.content.res.AssetManager;

import com.example.travelguide.article.ArticleInfo;

public class Storage
{

  static
  {
    System.loadLibrary("tguide");
  }

  private static boolean sIsAssetsInited = false;
  public static void initAssets(Context context)
  {
    if (sIsAssetsInited)
      return;

    final AssetManager am = context.getAssets();
    nativeInitIndex(am);
    sIsAssetsInited = true;
  }

  public native static ArticleInfo getArticleInfoByUrl(String url);

  public native static void query(String query, boolean useLocation, double lat, double lon);

  public native static  int getResultSize();

  public native static ArticleInfo getArticleInfoByIndex(int index);

  native private static void nativeInitIndex(AssetManager am);

}
