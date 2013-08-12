package com.example.travelguide;

import static com.example.travelguide.util.Expansion.DATA_FILE;
import android.app.Activity;
import android.app.PendingIntent;
import android.content.Intent;
import android.content.pm.PackageManager.NameNotFoundException;
import android.os.Bundle;
import android.os.Messenger;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.CompoundButton.OnCheckedChangeListener;
import android.widget.ProgressBar;
import android.widget.TextView;

import com.example.travelguide.expansion.ExpansionService;
import com.example.travelguide.util.Utils;
import com.google.android.vending.expansion.downloader.DownloadProgressInfo;
import com.google.android.vending.expansion.downloader.DownloaderClientMarshaller;
import com.google.android.vending.expansion.downloader.DownloaderServiceMarshaller;
import com.google.android.vending.expansion.downloader.Helpers;
import com.google.android.vending.expansion.downloader.IDownloaderClient;
import com.google.android.vending.expansion.downloader.IDownloaderService;
import com.google.android.vending.expansion.downloader.IStub;
import com.susanin.travelguide.R;

public class ExpansionActivity extends Activity
                               implements IDownloaderClient, OnCheckedChangeListener
{


  private IStub mDownloaderClientStub;
  private IDownloaderService mRemoteService;

  private ProgressBar mProgressBar;
  private TextView mDownloadState;
  private CheckBox mMobNetworkDownload;

  private TextView mSpeed;
  private TextView mTotal;


  @Override
  protected void onCreate(Bundle savedInstanceState)
  {
    super.onCreate(savedInstanceState);

    if (expansionFilesDelivered())
      forwardToApp(); // START!
    else
    {
       try
      {
        final int startResult = DownloaderClientMarshaller
           .startDownloadServiceIfRequired(getApplicationContext(),
                                           createNotificationIntent(),
                                           ExpansionService.class);

        if (startResult == DownloaderClientMarshaller.NO_DOWNLOAD_REQUIRED)
          forwardToApp(); // START!
        else
          setUpForDownloading();
      }
      catch (final NameNotFoundException e)
      {
        throw new RuntimeException(e); // TODO This should not happen
      }

    }
  }

  @Override
  protected void onResume()
  {
    super.onResume();
    connect();
  }

  @Override
  protected void onStop()
  {
    super.onStop();
    disconnect();
  }

  private void setUpForDownloading()
  {
    mDownloaderClientStub = DownloaderClientMarshaller.CreateStub(this, ExpansionService.class);
    setContentView(R.layout.activity_expansion);

    mProgressBar = (ProgressBar) findViewById(R.id.downloadProgress);
    mDownloadState = (TextView) findViewById(R.id.downloadState);

    mMobNetworkDownload = (CheckBox) findViewById(R.id.mobNetDownload);
    mMobNetworkDownload.setOnCheckedChangeListener(this);

    mSpeed = (TextView) findViewById(R.id.downloadSpeed);
    mTotal = (TextView) findViewById(R.id.downloadTotal);
  }

  private void forwardToApp()
  {
    finish();
    final Intent i = new Intent(this, ArticleInfoListActivity.class);
    startActivity(i);
  }

  private boolean expansionFilesDelivered()
  {
    final String fileName = Helpers.getExpansionAPKFileName(this, DATA_FILE.isMain, DATA_FILE.versionCode);
    if (!Helpers.doesFileExist(this, fileName, DATA_FILE.fileSize, false))
      return false;
    return true;
  }

  private PendingIntent createNotificationIntent()
  {
    final Intent i = new Intent(this, this.getClass());
    i.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP | Intent.FLAG_ACTIVITY_NEW_TASK);

    final PendingIntent pi = PendingIntent.getActivity(this, 0, i, PendingIntent.FLAG_UPDATE_CURRENT);
    return pi;
  }

  private void connect()
  {
    if (mDownloaderClientStub != null)
      mDownloaderClientStub.connect(getApplicationContext());
  }

  private void disconnect()
  {
    if (mDownloaderClientStub != null)
      mDownloaderClientStub.disconnect(getApplicationContext());
  }

  @Override
  public void onServiceConnected(Messenger m)
  {
    mRemoteService = DownloaderServiceMarshaller.CreateProxy(m);
    mRemoteService.onClientUpdated(mDownloaderClientStub.getMessenger());
    mRemoteService.requestDownloadStatus();
  }

  @Override
  public void onDownloadStateChanged(int newState)
  {
    mDownloadState.setText(Helpers.getDownloaderStringResourceIDFromState(newState));

    switch (newState)
    {
      case IDownloaderClient.STATE_COMPLETED:
        forwardToApp();
        return;
    }
  }

  @Override
  public void onDownloadProgress(DownloadProgressInfo progress)
  {
    final int currentProgressInPercent =
        (int) (mProgressBar.getMax()*progress.mOverallProgress/(float)progress.mOverallTotal);

    String downloadSpeed = "";
    if (progress.mCurrentSpeed > 0)
      downloadSpeed = Utils.formatDataSize(Math.round(progress.mCurrentSpeed*1024))+ "/S";

    final String progressStr = Utils.formatDataSize(progress.mOverallProgress) + "/"
                               + Utils.formatDataSize(progress.mOverallTotal);

    mSpeed.setText(downloadSpeed);
    mTotal.setText(progressStr);
    mProgressBar.setProgress(currentProgressInPercent);
  }

  private void setMobileNetworkDownloadEnabled(boolean enabled)
  {
    if (mRemoteService != null)
      if (enabled)
      {
      mRemoteService.setDownloadFlags(IDownloaderService.FLAGS_DOWNLOAD_OVER_CELLULAR);
      mRemoteService.requestContinueDownload();
      }
      else
        mRemoteService.setDownloadFlags(0);
  }

  @Override
  public void onCheckedChanged(CompoundButton buttonView, boolean isChecked)
  {
    if (buttonView == mMobNetworkDownload)
      setMobileNetworkDownloadEnabled(isChecked);
  }
}
