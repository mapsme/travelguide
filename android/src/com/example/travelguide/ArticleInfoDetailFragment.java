package com.example.travelguide;

import static com.example.travelguide.util.Utils.hideView;
import static com.example.travelguide.util.Utils.showView;
import android.content.Intent;
import android.graphics.Bitmap;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.TextView;

import com.example.travelguide.ArticleInfoListFragment.OnListIconClickedListener;
import com.example.travelguide.article.ArticleInfo;
import com.example.travelguide.article.ArticlePathFinder;
import com.example.travelguide.article.AssetsArticlePathFinder;

/**
 * A fragment representing a single ArticleInfo detail screen. This fragment is
 * either contained in a {@link ArticleInfoListActivity} in two-pane mode (on
 * tablets) or a {@link ArticleInfoDetailActivity} on handsets.
 */
public class ArticleInfoDetailFragment extends Fragment implements OnClickListener
{

  public static final String ARTICLE_INFO = "article_info";

  private ArticleInfo mItem;

  private View mRootView;
  private WebView mWebView;
  private TextView mTitle;
  private View mShowList;
  private View mProgressContainer;

  private ArticlePathFinder mFinder;

  private OnListIconClickedListener mIconClickedListener;

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

    final Bundle args = getArguments();
    if (args != null && args.containsKey(ARTICLE_INFO))
      mItem = (ArticleInfo) args.getSerializable(ARTICLE_INFO);

    mFinder = new AssetsArticlePathFinder();
  }

  public void setArticleInfo(ArticleInfo info)
  {
    mItem = info;

    final String url = mFinder.getPath(mItem);
    mTitle.setText(mItem.getName());

    if (mWebView.getUrl() != url)
      mWebView.loadUrl(url);
  }

  @Override
  public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState)
  {
    mRootView = inflater.inflate(R.layout.fragment_articleinfo_detail, container, false);
    mWebView = (WebView) mRootView.findViewById(R.id.webView);
    mProgressContainer = mRootView.findViewById(R.id.progressContainer);
    mTitle = (TextView) mRootView.findViewById(R.id.title);
    mShowList = mRootView.findViewById(R.id.showList);

    mWebView.setWebViewClient(new TgWebViewClient());
    mShowList.setOnClickListener(this);

    return mRootView;
  }

  @Override
  public void onViewCreated(View view, Bundle savedInstanceState)
  {
    super.onViewCreated(view, savedInstanceState);

    if (mItem != null)
      setArticleInfo(mItem);
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

  @Override
  public void onClick(View v)
  {
    if (mShowList.getId() == v.getId())
    {
      if (mIconClickedListener != null)
        mIconClickedListener.onIconClicked();
      else
      {
        final Intent i = new Intent(getActivity(), ArticleInfoListActivity.class);
        startActivity(i);
      }
    }
  }

  public void setOnIconClickedListener(OnListIconClickedListener listener)
  {
    mIconClickedListener = listener;
  }

}
