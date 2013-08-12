package com.example.travelguide;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;

public class ExpansionActivity extends Activity
{

  @Override
  protected void onCreate(Bundle savedInstanceState)
  {
    super.onCreate(savedInstanceState);

    forwardToApp();
//    setContentView(R.layout.activity_expansion);
  }

  private void forwardToApp()
  {
    finish();
    final Intent i = new Intent(this, ArticleInfoListActivity.class);
    startActivity(i);
  }

}
