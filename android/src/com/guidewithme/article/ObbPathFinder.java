package com.guidewithme.article;

import android.content.Context;
import android.os.storage.OnObbStateChangeListener;
import android.os.storage.StorageManager;
import android.util.Log;

import com.guidewithme.util.Expansion;

public class ObbPathFinder extends OnObbStateChangeListener
                           implements ArticlePathFinder
{
  private final static String TAG = "TravelObb";

  private final Context mContext;
  private final StorageManager mSm;

  public ObbPathFinder(Context context)
  {
    mContext = context;
    mSm = (StorageManager) mContext.getSystemService(Context.STORAGE_SERVICE);
    mSm.mountObb(Expansion.getPath(mContext.getPackageName()), null, this);
  }

  @Override
  public String getPath(ArticleInfo info)
  {
    return getRootDir() + info.getArticleId();
  }

  public String getRootDir()
  {
    return "file://" + mSm.getMountedObbPath(Expansion.getPath(mContext.getPackageName())) + "/data/";
  }

  @Override
  public void onObbStateChange(String path, int state)
  {
    super.onObbStateChange(path, state);
    Log.d(TAG, "path: " + path + " state: " + state);
  }

}
