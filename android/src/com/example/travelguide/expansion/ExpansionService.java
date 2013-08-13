package com.example.travelguide.expansion;

import com.google.android.vending.expansion.downloader.impl.DownloaderService;

public class ExpansionService extends DownloaderService
{

  private final static String RSA_KEY_STRING = "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAl6szmyCTg60x/H9W3su2Hkay6LfYHwDYgOJOckfKYlcliRvswXmsy8iw/jVb0NuBRKvyEMOpf/NkvRe9yhRJhdgYErnn4psZbfkyvNoF683GgelG5wzzz7RKKFr9Q1bq/dXc7CHHWeDBkYy0r/6JU7PM7VQgSGLu12qLcGvZNt7YYC5oKdZfT42Ox343mB7d/02QbmNtcnhfrx2rlRlbP2Ap7NlL/Is5Ga2bmESoyqaAue/Y2uAhr5O4tMysYUevF+0EVUlEJE+NdCmK3I9WcFM9gXy1IjRgWQRZWD/znnGSsMHetVExbO+b57JU2l8wbPfuBbLwdIfqVRiK7mEmowIDAQAB";
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
