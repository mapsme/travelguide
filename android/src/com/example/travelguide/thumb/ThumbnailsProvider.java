package com.example.travelguide.thumb;

import android.graphics.drawable.Drawable;

public interface ThumbnailsProvider
{
  public Drawable getThumbnailByUrl(String url);
}
