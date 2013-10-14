package com.guidewithme;

import android.app.Activity;
import android.app.PendingIntent;
import android.content.Intent;
import android.os.Bundle;
import android.support.v4.app.FragmentActivity;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;

import com.guidewithme.article.ArticleInfo;
import com.guidewithme.cpp.Storage;
import com.guidewithme.util.Utils;
import com.mapswithme.maps.api.MWMPoint;
import com.mapswithme.maps.api.MWMResponse;

/**
 * An activity representing a list of ArticleInfos. This activity has different
 * presentations for handset and tablet-size devices. On handsets, the activity
 * presents a list of items, which when touched, lead to a
 * {@link ArticleInfoDetailActivity} representing item details. On tablets, the
 * activity presents the list of items and item details side-by-side using two
 * vertical panes.
 * <p>
 * The activity makes heavy use of fragments. The list of items is a
 * {@link ArticleInfoListFragment} and the item details (if present) is a
 * {@link ArticleInfoDetailFragment}.
 * <p>
 * This activity also implements the required
 * {@link ArticleInfoListFragment.Callbacks} interface to listen for item
 * selections.
 */
public class ArticleInfoListActivity extends FragmentActivity
                                     implements ArticleInfoListFragment.Callbacks,
                                       ArticleInfoListFragment.OnListIconClickedListener,
                                       ArticleInfoListFragment.OnFirstLoadListener
{
  private static final String TAG = ArticleInfoListActivity.class.getSimpleName();

  /**
   * Whether or not the activity is in two-pane mode, i.e. running on a tablet
   * device.
   */
  private boolean mTwoPane;

  private ArticleInfoListFragment mArtInfoListFragment;
  private ArticleInfoDetailFragment mArtInfoDetailFragment;

  private View mShadow;

  @Override
  protected void onCreate(Bundle savedInstanceState)
  {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.activity_articleinfo_list);


    mArtInfoListFragment = (ArticleInfoListFragment) getSupportFragmentManager()
        .findFragmentById(R.id.articleinfo_list);

    if (findViewById(R.id.articleinfo_detail_container) != null)
    {
      mTwoPane = true;

      mShadow = findViewById(R.id.shadow);
      if (mShadow != null)
        mShadow.setOnClickListener(new OnClickListener()
        {
          @Override
          public void onClick(View v)
          {
            toogleListVisible();
          }
        });

      mArtInfoListFragment.setActivateOnItemClick(true);
      mArtInfoListFragment.setOnFirstLoadListener(this);
      mArtInfoListFragment.setHeaderVisible(false);

      mArtInfoDetailFragment = new ArticleInfoDetailFragment();
      mArtInfoDetailFragment.setOnIconClickedListener(this);

      getSupportFragmentManager()
        .beginTransaction()
        .add(R.id.articleinfo_detail_container, mArtInfoDetailFragment)
        .commit();

      if (savedInstanceState != null && savedInstanceState.containsKey(LIST_VISIBLE))
        if (savedInstanceState.getBoolean(LIST_VISIBLE, true))
          showList();
        else
          hideList();

    }

    handleIntent(getIntent());
  }

  private static final String LIST_VISIBLE = ".list_visible";
  @Override
  protected void onSaveInstanceState(Bundle outState)
  {
    super.onSaveInstanceState(outState);
    outState.putBoolean(LIST_VISIBLE, mArtInfoListFragment.isVisible());
  }

  @Override
  protected void onNewIntent(Intent intent)
  {
    super.onNewIntent(intent);
    handleIntent(intent);
  }

  private void handleIntent(Intent intent)
  {
    if ((intent.getFlags() & Intent.FLAG_ACTIVITY_LAUNCHED_FROM_HISTORY) != 0)
      return;

    final MWMResponse mwmResponse = MWMResponse.extractFromIntent(this, intent);
    final MWMPoint point = mwmResponse.getPoint();

    if (point.getId() != null)
    {
      final String id = point.getId();
      Log.d(TAG, id);
      onItemSelected(Storage.get(this).getArticleInfoByUrl(id));
    }
  }

  @Override
  public void onItemSelected(ArticleInfo info)
  {
    if (mTwoPane)
    {
      mArtInfoDetailFragment.setArticleInfo(info);
      if (mShadow != null)
        hideList();
    }
    else
    {
      final Intent detailIntent = new Intent(this, ArticleInfoDetailActivity.class);
      detailIntent.putExtra(ArticleInfoDetailFragment.ARTICLE_INFO, info);
      startActivity(detailIntent);
    }
  }

  @Override
  public void onIconClicked()
  {
    toogleListVisible();
  }

  public void toogleListVisible()
  {
    if (mArtInfoListFragment.isVisible())
      hideList();
    else
      showList();
  }

  private void hideList()
  {
    getSupportFragmentManager()
      .beginTransaction()
      .hide(mArtInfoListFragment)
      .commit();

   if (mShadow != null)
     Utils.fadeOut(this, mShadow);

   // we dont need keyboard when there is no list
   Utils.hideKeyboard(getParent());
  }

  private void showList()
  {
    getSupportFragmentManager()
      .beginTransaction()
      .setCustomAnimations(android.R.anim.slide_in_left, android.R.anim.slide_out_right)
      .show(mArtInfoListFragment)
      .commit();

    if (mShadow != null)
      Utils.fadeIn(this, mShadow);
  }

  @Override
  public void onLoad(ArticleInfo info)
  {
    mArtInfoDetailFragment.setArticleInfo(info);
  }

  @Override
  public void onBackPressed()
  {
    if (mTwoPane && mArtInfoDetailFragment.canGoBack())
      mArtInfoDetailFragment.goBack();
    else
      super.onBackPressed();
  }

  public static PendingIntent createPendingIntent(Activity activity)
  {
    final Intent intent = new Intent(activity, ArticleInfoListActivity.class);
    return PendingIntent.getActivity(activity, 0, intent, 0);
  }
}
