package com.example.travelguide.thumb;

import static com.example.travelguide.util.Utils.notNull;
import android.content.Context;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.os.storage.OnObbStateChangeListener;
import android.os.storage.StorageManager;
import android.util.Log;

import com.example.travelguide.util.Expansion;

public class ObbThumbnailProvider extends OnObbStateChangeListener implements ThumbnailsProvider
{

  // Mount events
  public interface MountStateChangedListener
  {
    public void onMountStateChanged(int newState);
  }

  private MountStateChangedListener mStateChangedListener;
  public void setOnMountChangedListener(MountStateChangedListener listener)
  {
    mStateChangedListener = listener;
  }
  // !Mount event

  private final static String TAG = "TravelObb";

  private final Context mContext;
  private final StorageManager mSm;

  public ObbThumbnailProvider(Context context)
  {
    mContext = context;
    mSm = (StorageManager) mContext.getSystemService(Context.STORAGE_SERVICE);
    mSm.mountObb(Expansion.getPath(), null, this);
  }

  public ObbThumbnailProvider(Context context, MountStateChangedListener listener)
  {
    this(context);
    mStateChangedListener = listener;
  }

  @Override
  public void onObbStateChange(String path, int state)
  {
    super.onObbStateChange(path, state);
    Log.d(TAG, "path: " + path + " state: " + state);

    if (notNull(mStateChangedListener))
      mStateChangedListener.onMountStateChanged(state);
  }

  @Override
  public Drawable getThumbnailByUrl(String url)
  {
    final String pathInObb = mSm.getMountedObbPath(Expansion.getPath()) + "/data/thumb/" + url;
    return BitmapDrawable.createFromPath(pathInObb);
  }

}
