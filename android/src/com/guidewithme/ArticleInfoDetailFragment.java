package com.guidewithme;

import static com.guidewithme.util.Utils.notNull;

import java.io.InputStream;

import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.net.Uri;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.webkit.MimeTypeMap;
import android.webkit.URLUtil;
import android.webkit.WebResourceResponse;
import android.webkit.WebSettings;
import android.webkit.WebSettings.LayoutAlgorithm;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.TextView;

import com.guidewithme.ArticleInfoListFragment.OnListIconClickedListener;
import com.guidewithme.article.ArticleInfo;
import com.guidewithme.async.ZippedGuidesStorage;
import com.guidewithme.cpp.Storage;
import com.guidewithme.util.Expansion;
import com.guidewithme.util.Utils;
import com.mapswithme.maps.api.MapsWithMeApi;

/**
 * A fragment representing a single ArticleInfo detail screen. This fragment is
 * either contained in a {@link ArticleInfoListActivity} in two-pane mode (on
 * tablets) or a {@link ArticleInfoDetailActivity} on handsets.
 */
public class ArticleInfoDetailFragment extends Fragment
                                       implements OnClickListener
{
  private static final String TAG = "ArticleInfoDetailFragment";
  public static final String ARTICLE_INFO = "article_info";

  private static ArticleInfo mItem;

  private View mRootView;
  private WebView mWebView;
  private TextView mTitle;
  private View mShowList;
  private View mProgressContainer;

  private ZippedGuidesStorage mZippedGuidesStorage;

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
    else if (savedInstanceState != null && savedInstanceState.containsKey(ARTICLE_INFO))
      mItem = (ArticleInfo) savedInstanceState.getSerializable(ARTICLE_INFO);

    mZippedGuidesStorage = new ZippedGuidesStorage(Expansion.findPackageObbFile(getActivity().getPackageName()));
  }

  @Override
  public void onSaveInstanceState(Bundle outState)
  {
    super.onSaveInstanceState(outState);
    outState.putSerializable(ARTICLE_INFO, mItem);
  }

  public void setArticleInfo(ArticleInfo info)
  {
    mItem = info;
    // we need to set correct scheme for the web view
    final String url = "file:///" + mItem.getArticleId();
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

      // Fix java.lang.NullPointerException at:
      // android.view.animation.AnimationUtils.loadAnimation(AnimationUtils.java:71)
      final Context context = getActivity();
      if (shouldAnimate(url, context))
      {
        Utils.fadeOut(context, mProgressContainer);
        Utils.fadeIn(context, mWebView);
      }

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

      // Fix java.lang.NullPointerException at:
      // android.view.animation.AnimationUtils.loadAnimation(AnimationUtils.java:71)
      final Context context = getActivity();
      if (shouldAnimate(url, context))
      {
        Utils.fadeOut(context, mWebView);
        Utils.fadeIn(context, mProgressContainer);
      }

      if (URLUtil.isFileUrl(url) && url.endsWith(".html"))
      {
        final String strippedUrl = url.substring(url.lastIndexOf('/') + 1, url.lastIndexOf('.'));
        mTitle.setText(getStorage().getArticleInfoByUrl(strippedUrl).getName());
      }
    }

    private boolean shouldAnimate(String url, final Context context)
    {
      return notNull(context) && !Utils.isAnchorUrl(url);
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
        final PendingIntent pi = ArticleInfoListActivity.createPendingIntent(getActivity());

        // TODO: Decided to use 11 as default scale, but MapsWithMe has bug with scales,
        // so do pass 13 as a compromise.
        MapsWithMeApi.showMapsWithMeUrl(getActivity(), pi, 13, url);

        return true;
      }

      return super.shouldOverrideUrlLoading(view, url);
    }

    @Override
    public WebResourceResponse shouldInterceptRequest(WebView view, String url)
    {
      if (url.startsWith("file:///"))
      {
        final InputStream is = mZippedGuidesStorage.getData(url.replace("file:///", "data/"));
        if (is != null)
        {
          String type = getActivity().getContentResolver().getType(Uri.parse(url));
          // On KitKat it can return null
          if (type == null)
            type = MimeTypeMap.getSingleton().getMimeTypeFromExtension(MimeTypeMap.getFileExtensionFromUrl(url));
          if (type == null)
          {
            Log.d(TAG, "ERROR: Unknown mime-type for " + url);
            type = "application/octet-stream";
          }
          return new WebResourceResponse(type, "UTF-8", is);
        }
      }
      return super.shouldInterceptRequest(view, url);
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
