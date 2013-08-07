package com.example.travelguide;

import android.graphics.Bitmap;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.TextView;

import static com.example.travelguide.util.Utils.*;

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

  private View mProgressContainer;

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
    mProgressContainer = mRootView.findViewById(R.id.progressContainer);

    ((TextView) mRootView.findViewById(R.id.articleinfo_detail)).setText(mItem.getName());
    mWebView.setWebViewClient(new TgWebViewClient());
    mWebView.loadUrl(mFinder.getPath(mItem));

    return mRootView;
  }

  class TgWebViewClient extends WebViewClient
  {
    @Override
    public void onPageFinished(WebView view, String url)
    {
      super.onPageFinished(view, url);
      hideView(mProgressContainer);
      showView(mWebView);
    }

    @Override
    public void onPageStarted(WebView view, String url, Bitmap favicon)
    {
      super.onPageStarted(view, url, favicon);
      hideView(mWebView);
      showView(mProgressContainer);
    }
  }

}
