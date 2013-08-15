package com.guidewithme.expansion;

import com.google.android.vending.expansion.downloader.impl.DownloaderService;

public class ExpansionService extends DownloaderService
{

  private final static String RSA_KEY_STRING = "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAu1XiE4QTmYZvKH5VPu5RwSeW0B5SzGyVGFc9fBona0J7gNHGBb7zW6MJzxrnen725/JvWCsQQPjEbfGQGn8SZcxCrKMcN4rBQLtw66fmllEJgtFORZMep0WlH+8Wk9Ss4CvYwnvwUsHtTRAQbFKYBbaGVHZwZBKDyVNzTAuM+jQo8RPocbkbhv8bcBy3z4oolAMhBZghAIOBafi608zv+AVW3L1SJZ9gWrVC/+F5YlxXV4+7npImHjThwB9ZDjWkSJZjlQSiDkNf6H0PnyMv+yymyOumXMoiXW4uLTGAWXhpUxrLuIpAd8M2lDU3Ofwxz2tMkd7QuB/ZSgEmlyCs0QIDAQAB";
  private final static byte[] SALT = "salt".getBytes();

  @Override
  public String getPublicKey()
  {
    return RSA_KEY_STRING;
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
