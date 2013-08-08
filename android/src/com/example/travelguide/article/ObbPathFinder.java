package com.example.travelguide.article;

import android.content.Context;
import android.os.Environment;
import android.os.storage.OnObbStateChangeListener;
import android.os.storage.StorageManager;
import android.util.Log;

public class ObbPathFinder extends OnObbStateChangeListener
                           implements ArticlePathFinder
{
  private final static String TAG = "TravelObb";

  // TODO: dynamically detect path
  public final String RAW_PATH = Environment.getExternalStorageDirectory() + "/data.obb";

  private final Context mContext;
  private final StorageManager mSm;

  public ObbPathFinder(Context context)
  {
    mContext = context;
    mSm = (StorageManager) mContext.getSystemService(Context.STORAGE_SERVICE);
    mSm.mountObb(RAW_PATH, null, this);
  }

  @Override
  public String getPath(ArticleInfo info)
  {
    return getRootDir() + info.getArticleId();
  }

  public String getRootDir()
  {
    return "file://" + mSm.getMountedObbPath(RAW_PATH) + "/data/";
  }

  @Override
  public void onObbStateChange(String path, int state)
  {
    super.onObbStateChange(path, state);
    Log.d(TAG, "path: " + path + " state: " + state);
  }

}
