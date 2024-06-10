package com.bao_mat_web.anti_reverse_app;

import android.app.ActivityManager;
import android.os.Bundle;
import android.widget.TextView;

import androidx.activity.EdgeToEdge;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.graphics.Insets;
import androidx.core.view.ViewCompat;
import androidx.core.view.WindowInsetsCompat;

import java.util.List;

public class Frida_detection extends AppCompatActivity {

    TextView frida_status;

    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        EdgeToEdge.enable(this);
        setContentView(R.layout.activity_frida_detection);

        frida_status = findViewById(R.id.frida_status);
        showFridaStatus();
    }

    void showFridaStatus() {

        frida_status.setText("Not using Frida");
    }


}