package com.example.travelguide;

import static com.example.travelguide.util.Utils.hideIf;
import android.app.Activity;
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
import com.example.travelguide.widget.StorageArticleInfoAdapter;

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
    OnClickListener
{
  private View mRootView;
  private TextView mSearchText;
  private View mCross;


  /**
   * The serialization (saved instance state) Bundle key representing the
   * activated item position. Only used on tablets.
   */
  private static final String STATE_ACTIVATED_POSITION = "activated_position";

  /**
   * The fragment's current callback object, which is notified of list item
   * clicks.
   */
  private Callbacks mCallbacks = sDummyCallbacks;

  /**
   * The current activated item position. Only used on tablets.
   */
  private int mActivatedPosition = ListView.INVALID_POSITION;

  /**
   * A callback interface that all activities containing this fragment must
   * implement. This mechanism allows activities to be notified of item
   * selections.
   */
  public interface Callbacks
  {
    /**
     * Callback for when an item has been selected.
     */
    public void onItemSelected(ArticleInfo info);
  }

  /**
   * A dummy implementation of the {@link Callbacks} interface that does
   * nothing. Used only when this fragment is not attached to an activity.
   */
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

    // Restore the previously serialized activated item position.
    if (savedInstanceState != null && savedInstanceState.containsKey(STATE_ACTIVATED_POSITION))
    {
      setActivatedPosition(savedInstanceState.getInt(STATE_ACTIVATED_POSITION));
    }
  }

  @Override
  public void onAttach(Activity activity)
  {
    super.onAttach(activity);

    // Activities containing this fragment must implement its callbacks.
    if (!(activity instanceof Callbacks))
    {
      throw new IllegalStateException("Activity must implement fragment's callbacks.");
    }

    mCallbacks = (Callbacks) activity;

    // Load initial data
    final Bundle args = new Bundle(1);
    args.putString(KEY_QUERY, "");
    getLoaderManager().initLoader(SEARCH_LOADER, args, this).forceLoad();
  }

  @Override
  public void onDetach()
  {
    super.onDetach();

    // Reset the active callbacks interface to the dummy implementation.
    mCallbacks = sDummyCallbacks;

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

    // setup listeners
    mSearchText.addTextChangedListener(this);
    mCross.setOnClickListener(this);

    return mRootView;
  }

  /**
   *
   * LOADER
   *
   */

  private static int SEARCH_LOADER = 0x1;
  private String KEY_QUERY = "key_query";

  @Override
  public Loader<Storage> onCreateLoader(int id, Bundle args)
  {
    if (id == SEARCH_LOADER)
    {
      final String query = args.getString(KEY_QUERY);
      // TODO: add location check
      // TODO: add progress
      return new QueryResultLoader(getActivity(), query);
    }
    return null;
  }

  @Override
  public void onLoadFinished(Loader<Storage> loader, Storage result)
  {
    setListAdapter(new StorageArticleInfoAdapter(result, getActivity()));
  }

  @Override
  public void onLoaderReset(Loader<Storage> loader)
  {}

  /**
   *
   * TEXT WATCHER
   *
   */

  @Override
  public void onTextChanged(CharSequence s, int start, int before, int count)
  {
    final Bundle args = new Bundle(1);
    args.putString(KEY_QUERY, s.toString());
    getLoaderManager().restartLoader(SEARCH_LOADER, args, this).forceLoad();

    hideIf(TextUtils.isEmpty(s), mCross);
  }

  @Override
  public void afterTextChanged(Editable s)
  {}

  @Override
  public void beforeTextChanged(CharSequence s, int start, int count, int after)
  {}

  /**
   *
   * CLICK
   *
   */

  @Override
  public void onClick(View v)
  {
    if (v.getId() == mCross.getId())
      mSearchText.setText(""); // clean up text field
  }

}
