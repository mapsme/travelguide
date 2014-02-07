package com.guidewithme.async;

import android.content.Context;
import android.support.v4.content.AsyncTaskLoader;

import com.guidewithme.cpp.Storage;

public class QueryResultLoader extends AsyncTaskLoader<Storage>
{
  private final Storage mStorage;
  private final String  mQuery;
  private final double  mLat;
  private final double  mLon;
  private final boolean mUseLocation;

  private boolean mIsReady = false;

  public QueryResultLoader(Context context, Storage storage, String query)
  {
    super(context);
    mQuery = query;
    mUseLocation = false;
    mLat = mLon = 0;
    mStorage = storage;
  }

  public QueryResultLoader(Context context, Storage storage, String query, double lat, double lon)
  {
    super(context);
    mQuery = query;
    mUseLocation = true;
    mLat = lat;
    mLon = lon;
    mStorage = storage;
  }

  @Override
  public Storage loadInBackground()
  {
    mStorage.query(mQuery, mUseLocation, mLat, mLon);
    mIsReady = true;
    return mStorage;
  }

  @Override
  protected void onStartLoading()
  {
    if (mIsReady && !takeContentChanged())
      deliverResult(mStorage);
    else
      forceLoad();
  }

}
