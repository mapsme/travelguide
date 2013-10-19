package com.guidewithme.expansion;

import com.google.android.vending.expansion.downloader.impl.DownloaderService;

public class ExpansionService extends DownloaderService
{
  private final static byte[] SALT = "salt".getBytes();

  @Override
  public String getPublicKey()
  {
    final String coutryName = getPackageName().replace("com.guidewithme.", "");

    if (!KeyMap.COUTRY_2_KEY.containsKey(coutryName))
      throw new IllegalArgumentException("There is no key for " + coutryName);

    return KeyMap.COUTRY_2_KEY.get(coutryName);
  }

  @Override
  public byte[] getSALT()
  {
    return SALT;
  }

  @Override
  public String getAlarmReceiverClassName()
  {
    return DownloadBroadcastReceiver.class.getName();
  }

}
