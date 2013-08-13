package com.example.travelguide;

import android.app.PendingIntent;
import android.content.Intent;
import android.graphics.Bitmap;
import android.net.Uri;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.webkit.URLUtil;
import android.webkit.WebSettings;
import android.webkit.WebSettings.LayoutAlgorithm;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.TextView;

import com.example.travelguide.ArticleInfoListFragment.OnListIconClickedListener;
import com.example.travelguide.article.ArticleInfo;
import com.example.travelguide.article.ArticlePathFinder;
import com.example.travelguide.article.ObbPathFinder;
import com.example.travelguide.cpp.Storage;
import com.example.travelguide.util.Utils;
import com.mapswithme.maps.api.MWMPoint;
import com.mapswithme.maps.api.MapsWithMeApi;
import com.susanin.travelguide.R;

/**
 * A fragment representing a single ArticleInfo detail screen. This fragment is
 * either contained in a {@link ArticleInfoListActivity} in two-pane mode (on
 * tablets) or a {@link ArticleInfoDetailActivity} on handsets.
 */
public class ArticleInfoDetailFragment extends Fragment
                                       implements OnClickListener
{

  public static final String ARTICLE_INFO = "article_info";

  private ArticleInfo mItem;

  private View mRootView;
  private WebView mWebView;
  private TextView mTitle;
  private View mShowList;
  private View mProgressContainer;

  private ArticlePathFinder mFinder;
//  private Storage mStorage;

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

    mFinder = new ObbPathFinder(getActivity().getApplicationContext());
  }

  public void setArticleInfo(ArticleInfo info)
  {
    mItem = info;

    final String url = mFinder.getPath(mItem);
    mTitle.setText(mItem.getName());

    if (!url.equalsIgnoreCase(mWebView.getUrl()))
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

  public class TgWebViewClient extends WebViewClient
  {

    @Override
    public void onPageFinished(WebView view, String url)
    {
      super.onPageFinished(view, url);
      Utils.fadeOut(getActivity(), mProgressContainer);
      Utils.fadeIn(getActivity(), mWebView);

      // If picture enable zoom, else disable
      final WebSettings ws = mWebView.getSettings();

      final boolean isPicture = Utils.isPictUrl(url);
      ws.setBuiltInZoomControls(isPicture);
      ws.setSupportZoom(isPicture);
      ws.setLoadWithOverviewMode(isPicture);
      ws.setLayoutAlgorithm(isPicture ? LayoutAlgorithm.SINGLE_COLUMN : LayoutAlgorithm.NARROW_COLUMNS);
      ws.setUseWideViewPort(isPicture);
    }

    @Override
    public void onPageStarted(WebView view, String url, Bitmap favicon)
    {
      super.onPageStarted(view, url, favicon);
      Utils.fadeOut(getActivity(), mWebView);
      Utils.fadeIn(getActivity() ,mProgressContainer);

      if (URLUtil.isFileUrl(url) && url.endsWith(".html"))
      {
        final String strippedUrl = url.substring(url.lastIndexOf('/') + 1, url.lastIndexOf('.'));
        mTitle.setText(getStorage().getArticleInfoByUrl(strippedUrl).getName());
      }
    }

    @Override
    public boolean shouldOverrideUrlLoading(WebView view, String url)
    {
      if (URLUtil.isNetworkUrl(url))
      {
        final Intent i = new Intent(Intent.ACTION_VIEW);
        i.setData(Uri.parse(url));
        if (getActivity().getPackageManager().resolveActivity(i, 0) != null)
          startActivity(i);

        return true;
      }
      else if (url.startsWith("mapswithme://"))
      {
        final Uri uri = Uri.parse(url);
        final String latlon = uri.getQueryParameter("ll");
        final double lat = Double.parseDouble(latlon.split(",")[0]);
        final double lon = Double.parseDouble(latlon.split(",")[1]);
        final String name = uri.getQueryParameter("n");
        final String id = uri.getQueryParameter("id");

        final PendingIntent pi = ArticleInfoListActivity.createPendingIntent(getActivity());
        final MWMPoint point = new MWMPoint(lat, lon, name, id);
        MapsWithMeApi.showPointsOnMap(getActivity(), name, pi, point);

        return true;
      }

      return super.shouldOverrideUrlLoading(view, url);
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
        startActivity(i.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP));
      }
    }
  }

  public void setOnIconClickedListener(OnListIconClickedListener listener)
  {
    mIconClickedListener = listener;
  }

  public boolean canGoBack()
  {
    return mWebView.canGoBack();
  }

  public void goBack()
  {
    mWebView.goBack();
  }

  public Storage getStorage()
  {
    return Storage.get(getActivity());
  }

}
