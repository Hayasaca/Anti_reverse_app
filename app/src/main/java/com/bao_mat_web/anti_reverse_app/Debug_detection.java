package com.bao_mat_web.anti_reverse_app;

import android.os.Bundle;
import android.os.Debug;
import android.widget.TextView;

import androidx.activity.EdgeToEdge;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.graphics.Insets;
import androidx.core.view.ViewCompat;
import androidx.core.view.WindowInsetsCompat;

public class Debug_detection extends AppCompatActivity {

    TextView debug_status;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        EdgeToEdge.enable(this);
        setContentView(R.layout.activity_debug_detection);

        debug_status = findViewById(R.id.debug_status);
        show_debug_status();
    }

    void show_debug_status() {
        boolean isInDebug = timing_check();
        if(isInDebug == true) {
            debug_status.setText("Get out of debug mode bro");
        }
        else {
            debug_status.setText("Not debug bro");
        }
    }

    private boolean timing_check() {
        long start = Debug.threadCpuTimeNanos();
        for(int i=0; i<1000000; ++i)
            continue;
        long stop = Debug.threadCpuTimeNanos();
        if(stop - start < 10000000) {
            return false;
        }
        else {
            return true;
        }
    }
}