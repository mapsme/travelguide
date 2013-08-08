package com.example.travelguide.webkit;

import android.annotation.SuppressLint;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.util.AttributeSet;
import android.webkit.WebSettings;
import android.webkit.WebView;

import com.example.travelguide.util.Utils;

@SuppressLint({ "SetJavaScriptEnabled", "NewApi" })
public class TgWebView extends WebView
{

  public TgWebView(Context context, AttributeSet attrs, int defStyle)
  {
    super(context, attrs, defStyle);
    final WebSettings settings = getSettings();
    settings.setJavaScriptEnabled(true);
    if (Utils.isApiLevelAbove(10))
      settings.setDisplayZoomControls(false);
  }

  public TgWebView(Context context, AttributeSet attrs)
  {
    this(context, attrs, 0);
  }

  @Override
  public void loadUrl(String url)
  {
    if (!Utils.isExternalUrl(url))
      super.loadUrl(url);
    else
    {
      final Intent i = new Intent(Intent.ACTION_VIEW);
      i.setData(Uri.parse(url));
      if (getContext().getPackageManager().resolveActivity(i, 0) != null)
        getContext().startActivity(i);
    }
  }



}
