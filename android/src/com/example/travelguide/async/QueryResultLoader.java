package com.example.travelguide.async;

import android.content.Context;
import android.support.v4.content.AsyncTaskLoader;

import com.example.travelguide.cpp.Storage;

public class QueryResultLoader extends AsyncTaskLoader<Storage>
{
  private final String  mQuery;
  private final double  mLat;
  private final double  mLon;
  private final boolean mUseLocation;

  public QueryResultLoader(Context context, String query)
  {
    super(context);
    mQuery = query;
    mUseLocation = false;
    mLat = mLon = 0;
  }

  public QueryResultLoader(Context context, String query, double lat, double lon)
  {
    super(context);
    mQuery = query;
    mUseLocation = true;
    mLat = lat;
    mLon = lon;
  }

  @Override
  public Storage loadInBackground()
  {
    Storage.initAssets(getContext());
    final Storage storage = new Storage();
    storage.query(mQuery, mUseLocation, mLat, mLon);
    return storage;
  }

}
