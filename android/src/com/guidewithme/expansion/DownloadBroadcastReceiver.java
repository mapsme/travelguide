package com.guidewithme.expansion;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager.NameNotFoundException;

import com.google.android.vending.expansion.downloader.DownloaderClientMarshaller;

public class DownloadBroadcastReceiver extends BroadcastReceiver
{

  @Override
  public void onReceive(Context context, Intent intent)
  {
    try
    {
      DownloaderClientMarshaller
        .startDownloadServiceIfRequired(context, intent, DownloadBroadcastReceiver.class);
    }
    catch (final NameNotFoundException e)
    {
      e.printStackTrace();
    }
  }

}
