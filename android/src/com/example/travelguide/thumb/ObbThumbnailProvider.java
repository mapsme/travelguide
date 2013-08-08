package com.example.travelguide.thumb;

import android.content.Context;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.os.Environment;
import android.os.storage.OnObbStateChangeListener;
import android.os.storage.StorageManager;
import android.util.Log;

public class ObbThumbnailProvider extends OnObbStateChangeListener implements ThumbnailsProvider
{

  private final static String TAG = "TravelObb";

  public final String RAW_PATH = Environment.getExternalStorageDirectory() + "/data.obb";

  private final Context mContext;
  private final StorageManager mSm;

  public ObbThumbnailProvider(Context context)
  {
    mContext = context;
    mSm = (StorageManager) mContext.getSystemService(Context.STORAGE_SERVICE);
    mSm.mountObb(RAW_PATH, null, this);
  }

  @Override
  public void onObbStateChange(String path, int state)
  {
    super.onObbStateChange(path, state);
    Log.d(TAG, "path: " + path + " state: " + state);
  }

  @Override
  public Drawable getThumbnailByUrl(String url)
  {
    final String pathInObb = "" + mSm.getMountedObbPath(RAW_PATH) + "/data/thumb/" + url;
    final Drawable drawable = BitmapDrawable.createFromPath(pathInObb);
    return drawable;
  }

}
