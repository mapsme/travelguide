package com.example.travelguide.thumb;

import android.content.Context;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.os.storage.OnObbStateChangeListener;
import android.os.storage.StorageManager;
import android.util.Log;

import com.example.travelguide.util.Expansion;

public class ObbThumbnailProvider extends OnObbStateChangeListener implements ThumbnailsProvider
{

  private final static String TAG = "TravelObb";

  private final Context mContext;
  private final StorageManager mSm;

  public ObbThumbnailProvider(Context context)
  {
    mContext = context;
    mSm = (StorageManager) mContext.getSystemService(Context.STORAGE_SERVICE);
    mSm.mountObb(Expansion.getPath(mContext.getPackageName()), null, this);
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
    final String pathInObb = mSm.getMountedObbPath(Expansion.getPath(mContext.getPackageName())) + "/data/thumb/" + url;
    return BitmapDrawable.createFromPath(pathInObb);
  }

}
