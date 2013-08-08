package com.example.travelguide.util;

import java.io.File;

import android.content.Context;
import android.view.View;
import android.view.animation.Animation;
import android.view.animation.Animation.AnimationListener;
import android.view.animation.AnimationUtils;

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

  public static void fadeIn(Context context, final View target)
  {
    final Animation anim = AnimationUtils.loadAnimation(context, android.R.anim.fade_in);
    final AnimationListener listener = new AnimationListener()
    {

      @Override
      public void onAnimationStart(Animation animation)
      {
        showView(target);
      }

      @Override
      public void onAnimationRepeat(Animation animation)
      {}

      @Override
      public void onAnimationEnd(Animation animation)
      {}
    };
    anim.setAnimationListener(listener);
    target.startAnimation(anim);
  }

  public static void fadeOut(Context context, final View target)
  {
    final Animation anim = AnimationUtils.loadAnimation(context, android.R.anim.fade_out);
    final AnimationListener listener = new AnimationListener()
    {

      @Override
      public void onAnimationStart(Animation animation)
      {}

      @Override
      public void onAnimationRepeat(Animation animation)
      {}

      @Override
      public void onAnimationEnd(Animation animation)
      {
        hideView(target);
      }
    };
    anim.setAnimationListener(listener);
    target.startAnimation(anim);
  }

}
