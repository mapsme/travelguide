package com.guidewithme;

import android.app.Activity;
import android.app.PendingIntent;
import android.content.Intent;
import android.content.pm.PackageManager.NameNotFoundException;
import android.os.Bundle;
import android.os.Messenger;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.ProgressBar;
import android.widget.TextView;

import com.google.android.vending.expansion.downloader.DownloadProgressInfo;
import com.google.android.vending.expansion.downloader.DownloaderClientMarshaller;
import com.google.android.vending.expansion.downloader.DownloaderServiceMarshaller;
import com.google.android.vending.expansion.downloader.Helpers;
import com.google.android.vending.expansion.downloader.IDownloaderClient;
import com.google.android.vending.expansion.downloader.IDownloaderService;
import com.google.android.vending.expansion.downloader.IStub;
import com.guidewithme.expansion.ExpansionService;
import com.guidewithme.util.Expansion;
import com.guidewithme.util.Utils;
import com.guidewithme.uk.R;

public class ExpansionActivity extends Activity
                               implements IDownloaderClient, OnClickListener
{


  private IStub mDownloaderClientStub;
  private IDownloaderService mRemoteService;
  private int mState = -1;

  private ProgressBar mProgressBar;
  private TextView mDownloadState;

  private TextView mSpeed;
  private TextView mTotal;

  private Button mResumePauseRetry;

  @Override
  protected void onCreate(Bundle savedInstanceState)
  {
    super.onCreate(savedInstanceState);

    if (expansionFilesDelivered())
      forwardToApp();
    else
      startDownloading();
  }

  private void startDownloading()
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
      throw new IllegalStateException(e); // This should not happen
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

    mSpeed = (TextView) findViewById(R.id.downloadSpeed);
    mTotal = (TextView) findViewById(R.id.downloadTotal);

    mResumePauseRetry = (Button)findViewById(R.id.resumePauseRetry);
  }

  private void forwardToApp()
  {
    finish();
    final Intent i = new Intent(this, ArticleInfoListActivity.class);
    startActivity(i);
  }

  private boolean expansionFilesDelivered()
  {
    return Expansion.findFirstObbFile(getPackageName()) != null;
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
    mState = newState;
    switch (mState)
    {
      case IDownloaderClient.STATE_COMPLETED:
        forwardToApp();
        return;
    }

    setUpResumePauseRetry(newState);
  }

  @Override
  public void onDownloadProgress(DownloadProgressInfo progress)
  {
    final int currentProgressInPercent =
        (int) (mProgressBar.getMax()*progress.mOverallProgress/(float)progress.mOverallTotal);

    String downloadSpeed = "";
    if (progress.mCurrentSpeed > 0)
      downloadSpeed = Utils.formatDataSize(Math.round(progress.mCurrentSpeed*1024))+ "/s";

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
        mRemoteService.setDownloadFlags(IDownloaderService.FLAGS_DOWNLOAD_OVER_CELLULAR);
      else
        mRemoteService.setDownloadFlags(0);
  }

  private boolean isPaused(int state)
  {
    return state >= STATE_PAUSED_NETWORK_UNAVAILABLE && state <= STATE_PAUSED_SDCARD_UNAVAILABLE;
  }

  private boolean isFailed(int state)
  {
    return state >= STATE_FAILED_UNLICENSED && state <= STATE_FAILED;
  }

  private boolean isActive(int state)
  {
    return state >= STATE_IDLE && state <= STATE_DOWNLOADING;
  }

  private boolean needCellularPermission(int state)
  {
    return state >= STATE_PAUSED_WIFI_DISABLED_NEED_CELLULAR_PERMISSION
        && state <= STATE_PAUSED_NEED_CELLULAR_PERMISSION;
  }

  private void setUpResumePauseRetry(int state)
  {
    if (isPaused(state))
    {
      if (needCellularPermission(state))
        mResumePauseRetry.setText(R.string.use_mobile_net);
      else
        mResumePauseRetry.setText(R.string.resume);
    }
    else if (isFailed(state))
      mResumePauseRetry.setText(R.string.retry);
    else if (isActive(state))
      mResumePauseRetry.setText(R.string.pause);
    else
    {
      Utils.hideView(mResumePauseRetry);
      mResumePauseRetry.setOnClickListener(null);
      return;
    }
    Utils.showView(mResumePauseRetry);
    mResumePauseRetry.setOnClickListener(this);
  }

  @Override
  public void onClick(View v)
  {
    if (R.id.resumePauseRetry == v.getId())
    {
      if (isActive(mState))
        mRemoteService.requestPauseDownload();
      else if (isFailed(mState))
        startDownloading();
      else if (isPaused(mState))
      {
        if (needCellularPermission(mState))
          setMobileNetworkDownloadEnabled(true);
        mRemoteService.requestContinueDownload();
      }
      else
        throw new IllegalStateException("Must not be here.");
    }
  }

}
