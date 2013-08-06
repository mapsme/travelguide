package com.example.travelguide;

import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.webkit.WebView;
import android.widget.TextView;

import com.example.travelguide.article.ArticleInfo;
import com.example.travelguide.article.ArticlePathFinder;
import com.example.travelguide.article.AssetsArticlePathFinder;

/**
 * A fragment representing a single ArticleInfo detail screen. This fragment is
 * either contained in a {@link ArticleInfoListActivity} in two-pane mode (on
 * tablets) or a {@link ArticleInfoDetailActivity} on handsets.
 */
public class ArticleInfoDetailFragment extends Fragment
{
  public static final String ARTICLE_INFO = "article_info";

  private ArticleInfo mItem;

  private View mRootView;
  private WebView mWebView;

  private ArticlePathFinder mFinder;

  /**
   * Mandatory empty constructor for the fragment manager to instantiate the
   * fragment (e.g. upon screen orientation changes).
   */
  public ArticleInfoDetailFragment()
  {}

  @Override
  public void onCreate(Bundle savedInstanceState)
  {
    super.onCreate(savedInstanceState);

    if (getArguments().containsKey(ARTICLE_INFO))
      mItem = (ArticleInfo) getArguments().getSerializable(ARTICLE_INFO);

    if (mItem == null)
      throw new RuntimeException("ArticleInfo must be specified.");

    mFinder = new AssetsArticlePathFinder();
  }

  @Override
  public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState)
  {
    mRootView = inflater.inflate(R.layout.fragment_articleinfo_detail, container, false);
    mWebView = (WebView) mRootView.findViewById(R.id.webView);
    ((TextView) mRootView.findViewById(R.id.articleinfo_detail)).setText(mItem.getName());

    mWebView.loadUrl(mFinder.getPath(mItem));

    return mRootView;
  }

}
