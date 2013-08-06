package com.example.travelguide;

import android.content.Intent;
import android.os.Bundle;
import android.support.v4.app.FragmentActivity;

import com.example.travelguide.article.ArticleInfo;

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
public class ArticleInfoListActivity extends FragmentActivity implements ArticleInfoListFragment.Callbacks
{

  /**
   * Whether or not the activity is in two-pane mode, i.e. running on a tablet
   * device.
   */
  private boolean mTwoPane;

  @Override
  protected void onCreate(Bundle savedInstanceState)
  {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.activity_articleinfo_list);

    if (findViewById(R.id.articleinfo_detail_container) != null)
    {
      mTwoPane = true;
      ((ArticleInfoListFragment) getSupportFragmentManager().findFragmentById(R.id.articleinfo_list))
          .setActivateOnItemClick(true);
    }

    // TODO: If exposing deep links into your app, handle intents here.
  }

  /**
   * Callback method from {@link ArticleInfoListFragment.Callbacks} indicating
   * that the item with the given ID was selected.
   */
  @Override
  public void onItemSelected(ArticleInfo info)
  {
    if (mTwoPane)
    {
      Bundle arguments = new Bundle();
      arguments.putSerializable(ArticleInfoDetailFragment.ARTICLE_INFO, info);
      ArticleInfoDetailFragment fragment = new ArticleInfoDetailFragment();

      fragment.setArguments(arguments);
      getSupportFragmentManager().beginTransaction().replace(R.id.articleinfo_detail_container, fragment).commit();
    }
    else
    {
      Intent detailIntent = new Intent(this, ArticleInfoDetailActivity.class);
      detailIntent.putExtra(ArticleInfoDetailFragment.ARTICLE_INFO, info);
      startActivity(detailIntent);
    }
  }
}
