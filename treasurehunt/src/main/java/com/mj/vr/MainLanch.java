package com.mj.vr;

import android.content.Intent;
import android.os.Bundle;
import android.app.Activity;
import android.view.Menu;
import android.view.View;
import android.widget.Button;
import android.widget.*;
import com.google.vr.sdk.base.CardboardViewNativeImpl;
import com.google.vr.cardboard.MutableEglConfigChooser;
import com.google.vr.cardboard.VrParamsProviderJni;

public class MainLanch extends Activity {

    Button button;
    TextView textView;
    int count = 0;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        button = (Button)findViewById(R.id.button1);
//        int i = TreasureActivityJNI.cout;
        textView = (TextView)findViewById(R.id.textView1);
    }

    static {
        System.loadLibrary("gvr");
        System.loadLibrary("gvr_audio");
        System.loadLibrary("gvr_t");
    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        return true;
    }

    public void Btn1_Click(View view)
    {
        String str = "---->" + count;
//        str = stringFromJNI();
        textView.setText(str);
        startActivity(new Intent(MainLanch.this, TreasureHuntActivity.class));
        count++;
    }

}
