package com.example.travelguide.util;

import java.io.File;

import android.view.View;

public class Utils
{
  public static View hideView(View view)
  {
    view.setVisibility(View.GONE);
    return view;
  }

  public static View showView(View view)
  {
    view.setVisibility(View.VISIBLE);
    return view;
  }

  public static View hideIf(boolean condition, View view)
  {
    if (condition)
      hideView(view);
    else
      showView(view);
    return view;
  }

  public static boolean fileExists(String path)
  {
    final File file = new File(path);
    return file.exists();
  }

  public static boolean isExternalUrl(String path)
  {
    return path.startsWith("http") || path.startsWith("www.");
  }
}
