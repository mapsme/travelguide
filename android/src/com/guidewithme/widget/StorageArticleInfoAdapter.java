package com.guidewithme.widget;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.TextView;

import com.guidewithme.article.ArticleInfo;
import com.guidewithme.cpp.Storage;
import com.guidewithme.thumb.ObbThumbnailProvider;
import com.guidewithme.thumb.ThumbnailsProvider;
import com.guidewithme.R;

public class StorageArticleInfoAdapter extends BaseAdapter
                                       implements ObbThumbnailProvider.MountStateChangedListener
{
  private final Storage mStorage;
  private final Context mContext;
  private final ThumbnailsProvider mThumbnailsProvider;

  public StorageArticleInfoAdapter(Storage storage, Context context)
  {
    mStorage = storage;
    mContext = context;
    mThumbnailsProvider = new ObbThumbnailProvider(context, this);
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
    view.setTag(holder);

    return view;
  }

  class ViewHolder
  {
    final TextView mTitle;
    final TextView mParent;
    final ImageView mThumbnail;

    ArticleInfo mInfo;

    ViewHolder(View view)
    {
      mThumbnail = (ImageView) view.findViewById(R.id.thumbnail);
      mTitle = (TextView) view.findViewById(R.id.title);
      mParent = (TextView) view.findViewById(R.id.parent);
    }

    void fill(ArticleInfo info)
    {
      mInfo = info;
      mTitle.setText(info.getName());
      mParent.setText(info.getParent());
      mThumbnail.setImageDrawable(mThumbnailsProvider.getThumbnailByUrl(info.getIconUrl()));
    }
  }

  @Override
  public void onMountStateChanged(int newState)
  {
    notifyDataSetChanged();
  }

}
