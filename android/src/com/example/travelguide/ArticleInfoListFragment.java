package com.example.travelguide;

import static com.example.travelguide.util.Utils.hideIf;
import static com.example.travelguide.util.Utils.hideView;
import static com.example.travelguide.util.Utils.showView;
import java.util.ArrayList;
import android.app.Activity;
import android.content.Context;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.os.Bundle;
import android.support.v4.app.ListFragment;
import android.support.v4.app.LoaderManager.LoaderCallbacks;
import android.support.v4.content.Loader;
import android.text.Editable;
import android.text.TextUtils;
import android.text.TextWatcher;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.ListView;
import android.widget.TextView;
import com.example.travelguide.article.ArticleInfo;
import com.example.travelguide.async.QueryResultLoader;
import com.example.travelguide.cpp.Storage;
import com.example.travelguide.util.Utils;
import com.example.travelguide.widget.StorageArticleInfoAdapter;
import com.mapswithme.maps.api.MWMPoint;
import com.mapswithme.maps.api.MapsWithMeApi;
import com.susanin.travelguide.R;

/**
 * A list fragment representing a list of ArticleInfos. This fragment also
 * supports tablet devices by allowing list items to be given an 'activated'
 * state upon selection. This helps indicate which item is currently being
 * viewed in a {@link ArticleInfoDetailFragment}.
 * <p>
 * Activities containing this fragment MUST implement the {@link Callbacks}
 * interface.
 */
public class ArticleInfoListFragment extends ListFragment implements LoaderCallbacks<Storage>, TextWatcher,
    OnClickListener, LocationListener
{
  private static final String TAG = ArticleInfoListFragment.class.getSimpleName();

  public interface OnListIconClickedListener
  {
    public void onIconClicked();
  }

  // First load
  public interface OnFirstLoadListener
  {
    void onLoad(ArticleInfo info);
  }

  private OnFirstLoadListener mOnFirstLoad;
  private boolean mFirstLoad = true;

  public void setOnFirstLoadListener(OnFirstLoadListener listener)
  {
    mOnFirstLoad = listener;
  }

  // !First load

  private View mRootView;
  private TextView mSearchText;
  private View mCross;

  private View mListContainer;
  private View mProgressContainer;
  private View mHeader;
  private View mAbout;
  private View mShowMapForAll;

  // Location
  private LocationManager mLocationManager;
  private static long sLastLocationRequestTime = 0;
  private final static long LOCATION_UPDATE_INTERVAL = 30 * 60 * 1000;
  // !Location

  private static final String STATE_ACTIVATED_POSITION = "activated_position";

  private Callbacks mCallbacks = sDummyCallbacks;

  private int mActivatedPosition = ListView.INVALID_POSITION;

  public interface Callbacks
  {
    public void onItemSelected(ArticleInfo info);
  }

  private static Callbacks sDummyCallbacks = new Callbacks()
  {
    @Override
    public void onItemSelected(ArticleInfo info)
    {}
  };

  /**
   * Mandatory empty constructor for the fragment manager to instantiate the
   * fragment (e.g. upon screen orientation changes).
   */
  public ArticleInfoListFragment()
  {}

  @Override
  public void onViewCreated(View view, Bundle savedInstanceState)
  {
    super.onViewCreated(view, savedInstanceState);

    mLocationManager = (LocationManager) getActivity().getSystemService(Context.LOCATION_SERVICE);

    // Restore the previously serialized activated item position.
    if (savedInstanceState != null && savedInstanceState.containsKey(STATE_ACTIVATED_POSITION))
      setActivatedPosition(savedInstanceState.getInt(STATE_ACTIVATED_POSITION));
  }

  @Override
  public void onResume()
  {
    super.onResume();

    Utils.hideKeyboard(getActivity());
    checkLocation();
    search(mSearchText.getText().toString());
  }

  private void checkLocation()
  {
    if (System.currentTimeMillis() - sLastLocationRequestTime > LOCATION_UPDATE_INTERVAL)
    {
      // reqestSingleUpdate() listen for single update
      // and when get it will unsubscribe from LocationManager
      if (mLocationManager.isProviderEnabled(LocationManager.GPS_PROVIDER))
        mLocationManager.requestSingleUpdate(LocationManager.GPS_PROVIDER, this, null);
      if (mLocationManager.isProviderEnabled(LocationManager.NETWORK_PROVIDER))
        mLocationManager.requestSingleUpdate(LocationManager.NETWORK_PROVIDER, this, null);
    }
  }

  @Override
  public void onPause()
  {
    super.onPause();
    mLocationManager.removeUpdates(this);
  }

  @Override
  public void onAttach(Activity activity)
  {
    super.onAttach(activity);

    // Activities containing this fragment must implement its callbacks.
    if (!(activity instanceof Callbacks))
      throw new IllegalStateException("Activity must implement fragment's callbacks.");

    mCallbacks = (Callbacks) activity;
  }

  @Override
  public void onDetach()
  {
    super.onDetach();
    // Reset the active callbacks interface to the dummy implementation.

    mCallbacks = sDummyCallbacks;
    if (getLoaderManager().hasRunningLoaders())
      getLoaderManager().destroyLoader(SEARCH_LOADER);
  }

  @Override
  public void onListItemClick(ListView listView, View view, int position, long id)
  {
    super.onListItemClick(listView, view, position, id);
    mCallbacks.onItemSelected((ArticleInfo) getListAdapter().getItem(position));
  }

  @Override
  public void onSaveInstanceState(Bundle outState)
  {
    super.onSaveInstanceState(outState);
    if (mActivatedPosition != ListView.INVALID_POSITION)
      outState.putInt(STATE_ACTIVATED_POSITION, mActivatedPosition);
  }

  /**
   * Turns on activate-on-click mode. When this mode is on, list items will be
   * given the 'activated' state when touched.
   */
  public void setActivateOnItemClick(boolean activateOnItemClick)
  {
    // When setting CHOICE_MODE_SINGLE, ListView will automatically
    // give items the 'activated' state when touched.
    getListView().setChoiceMode(activateOnItemClick ? ListView.CHOICE_MODE_SINGLE : ListView.CHOICE_MODE_NONE);
  }

  private void setActivatedPosition(int position)
  {
    if (position == ListView.INVALID_POSITION)
    {
      getListView().setItemChecked(mActivatedPosition, false);
    }
    else
    {
      getListView().setItemChecked(position, true);
    }

    mActivatedPosition = position;
  }

  @Override
  public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState)
  {
    mRootView = inflater.inflate(R.layout.fragment_articleinfo_list, null, false);
    mSearchText = (TextView) mRootView.findViewById(R.id.searchText);
    mCross = mRootView.findViewById(R.id.clearSearch);

    mListContainer = mRootView.findViewById(R.id.listContainer);
    mProgressContainer = mRootView.findViewById(R.id.progressContainer);
    mHeader = mRootView.findViewById(R.id.header);
    mAbout = mRootView.findViewById(R.id.about);
    mShowMapForAll = mRootView.findViewById(R.id.showMapForAll);

    // setup listeners
    mSearchText.addTextChangedListener(this);
    mCross.setOnClickListener(this);
    mAbout.setOnClickListener(this);
    mShowMapForAll.setOnClickListener(this);

    return mRootView;
  }

  public void setHeaderVisible(boolean visible)
  {
    hideIf(!visible, mHeader);
  }

  private static int SEARCH_LOADER = 0x1;
  private final String KEY_QUERY = "key_query";

  @Override
  public Loader<Storage> onCreateLoader(int id, Bundle args)
  {
    if (id == SEARCH_LOADER)
    {
      final String query = args.getString(KEY_QUERY);
      showView(mProgressContainer);
      hideView(mListContainer);

      final Location loc = getLocation();
      if (loc != null)
        return new QueryResultLoader(getActivity(), query, loc.getLatitude(), loc.getLongitude());
      else
        return new QueryResultLoader(getActivity(), query);
    }
    return null;
  }

  @SuppressWarnings("static-access")
  @Override
  public void onLoadFinished(Loader<Storage> loader, Storage result)
  {
    setListAdapter(new StorageArticleInfoAdapter(result, getActivity()));
    showView(mListContainer);
    hideView(mProgressContainer);

    if (mFirstLoad && mOnFirstLoad != null && result.getResultSize() > 0)
    {
      mFirstLoad = false;
      mOnFirstLoad.onLoad(result.getArticleInfoByIndex(0));
    }
  }

  @Override
  public void onLoaderReset(Loader<Storage> loader)
  {
    hideView(mProgressContainer);
    showView(mListContainer);
  }

  private void search(CharSequence s)
  {
    final Bundle args = new Bundle(1);
    args.putString(KEY_QUERY, s.toString());
    getLoaderManager().restartLoader(SEARCH_LOADER, args, this).forceLoad();

    hideIf(TextUtils.isEmpty(s), mCross);
  }

  @Override
  public void onTextChanged(CharSequence s, int start, int before, int count)
  {
    search(s);
  }

  @Override
  public void afterTextChanged(Editable s)
  {}

  @Override
  public void beforeTextChanged(CharSequence s, int start, int count, int after)
  {}

  @Override
  public void onClick(View v)
  {
    if (v.getId() == mCross.getId())
      mSearchText.setText(""); // clean up text field
    else if (v.getId() == mAbout.getId())
    {
      // TODO: show about dialog
    }
    else if (v.getId() == mShowMapForAll.getId())
      openInMwm();
  }

  private void openInMwm()
  {
    final int count = Storage.getResultSize();
    if (count < 1)
        return;

    final ArrayList<MWMPoint> points = new ArrayList<MWMPoint>();
    for (int i = 0; i < count; ++i)
    {
      final ArticleInfo info = Storage.getArticleInfoByIndex(i);
      if (info.isValidLatLon())
        points.add(Utils.articleInfo2MwmPoint(info));
    }
    if (points.size() < 1)
      return;

    MapsWithMeApi.showPointsOnMap
      (getActivity(), "Hello, my articles!",
       ArticleInfoListActivity.createPendingIntent(getActivity()),
       points.toArray(new MWMPoint[points.size()]));
  }

  private Location getLocation()
  {
    Location loc = null;

    if ((loc = mLocationManager.getLastKnownLocation(LocationManager.GPS_PROVIDER)) == null)
      loc = mLocationManager.getLastKnownLocation(LocationManager.NETWORK_PROVIDER);

    return loc;
  }

  @Override
  public void onLocationChanged(Location location)
  {
    sLastLocationRequestTime = System.currentTimeMillis();
  }

  @Override
  public void onProviderDisabled(String provider)
  {}

  @Override
  public void onProviderEnabled(String provider)
  {}

  @Override
  public void onStatusChanged(String provider, int status, Bundle extras)
  {}

}
