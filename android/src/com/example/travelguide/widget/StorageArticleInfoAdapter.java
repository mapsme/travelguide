package com.example.travelguide.widget;

import com.example.travelguide.R;
import com.example.travelguide.article.ArticleInfo;
import com.example.travelguide.cpp.Storage;
import com.example.travelguide.thumb.AssetsThumbnailProvider;
import com.example.travelguide.thumb.ThumbnailsProvider;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.TextView;

public class StorageArticleInfoAdapter extends BaseAdapter
{
  private Storage mStorage;
  private Context mContext;
  private ThumbnailsProvider mThumbnailsProvider;

  public StorageArticleInfoAdapter(Storage storage, Context context)
  {
    mStorage = storage;
    mContext = context;
    mThumbnailsProvider = new AssetsThumbnailProvider(context);
  }

  @Override
  public int getCount()
  {
    return mStorage.getResultSize();
  }

  @Override
  public ArticleInfo getItem(int position)
  {
    return mStorage.getArticleInfoByIndex(position);
  }

  @Override
  public long getItemId(int position)
  {
    return position;
  }

  @Override
  public View getView(int position, View convertView, ViewGroup parent)
  {
    final View view = convertView == null
                      ? LayoutInflater.from(mContext).inflate(R.layout.listitem_article_info, null)
                      : convertView;

    ViewHolder holder;
    if (view.getTag() == null)
      holder = new ViewHolder(view);
    else
      holder = (ViewHolder) view.getTag();

    holder.fill(mStorage.getArticleInfoByIndex(position));

    return view;
  }

  class ViewHolder
  {
    final TextView  mTitle;
    final ImageView mThumbnail;

    ArticleInfo mInfo;

    ViewHolder(View view)
    {
      mTitle = (TextView) view.findViewById(R.id.title);
      mThumbnail = (ImageView) view.findViewById(R.id.thumbnail);
    }

    void fill(ArticleInfo info)
    {
      mInfo = info;
      mTitle.setText(info.getName());
      mThumbnail.setImageDrawable(mThumbnailsProvider.getThumbnailByUrl(info.getIconUrl()));
    }
  }

}
