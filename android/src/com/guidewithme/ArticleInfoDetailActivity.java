package com.guidewithme;

import android.content.Intent;
import android.os.Bundle;
import android.support.v4.app.FragmentActivity;
import android.support.v4.app.NavUtils;
import android.view.MenuItem;
import com.susanin.travelguide.R;

/**
 * An activity representing a single ArticleInfo detail screen. This activity is
 * only used on handset devices. On tablet-size devices, item details are
 * presented side-by-side with a list of items in a
 * {@link ArticleInfoListActivity}.
 * <p>
 * This activity is mostly just a 'shell' activity containing nothing more than
 * a {@link ArticleInfoDetailFragment}.
 */
public class ArticleInfoDetailActivity extends FragmentActivity
{

  private ArticleInfoDetailFragment mArticleInfoDetailFragment;

  @Override
  protected void onCreate(Bundle savedInstanceState)
  {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.activity_articleinfo_detail);

    if (savedInstanceState == null)
    {
      // Create the detail fragment and add it to the activity
      // using a fragment transaction.
      final Bundle arguments = new Bundle();
      arguments.putSerializable(ArticleInfoDetailFragment.ARTICLE_INFO,
          getIntent().getSerializableExtra(ArticleInfoDetailFragment.ARTICLE_INFO));

      mArticleInfoDetailFragment = new ArticleInfoDetailFragment();
      mArticleInfoDetailFragment.setArguments(arguments);

      getSupportFragmentManager()
        .beginTransaction()
        .add(R.id.articleinfo_detail_container, mArticleInfoDetailFragment)
        .commit();
    }
  }

  @Override
  public boolean onOptionsItemSelected(MenuItem item)
  {
    switch (item.getItemId())
    {
      case android.R.id.home:
        // This ID represents the Home or Up button. In the case of this
        // activity, the Up button is shown. Use NavUtils to allow users
        // to navigate up one level in the application structure. For
        // more details, see the Navigation pattern on Android Design:
        //
        // http://developer.android.com/design/patterns/navigation.html#up-vs-back
        //
        NavUtils.navigateUpTo(this, new Intent(this, ArticleInfoListActivity.class));
        return true;
    }
    return super.onOptionsItemSelected(item);
  }

  @Override
  public void onBackPressed()
  {
    if (mArticleInfoDetailFragment != null && mArticleInfoDetailFragment.canGoBack())
      mArticleInfoDetailFragment.goBack();
    else
      super.onBackPressed();
  }
}
