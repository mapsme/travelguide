package com.guidewithme.thumb;

import android.graphics.drawable.Drawable;

public interface ThumbnailsProvider
{
  public Drawable getThumbnailByUrl(String url);
}
