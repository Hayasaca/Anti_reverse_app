package com.bao_mat_web.anti_reverse_app;

import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.widget.Button;
import android.widget.TextView;

import androidx.activity.EdgeToEdge;
import androidx.appcompat.app.AppCompatActivity;
import android.os.Build;
import androidx.core.graphics.Insets;
import androidx.core.view.ViewCompat;
import androidx.core.view.WindowInsetsCompat;

import java.io.BufferedReader;
import java.io.File;
import java.io.InputStreamReader;

public class MainActivity extends AppCompatActivity {

    Button root_status_btn;
    Button emulator_status_btn;
    Button debug_status_btn;
    Button frida_status_btn;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        EdgeToEdge.enable(this);
        setContentView(R.layout.activity_main);

        root_status_btn = findViewById(R.id.root_status_btn);
        emulator_status_btn = findViewById(R.id.emulator_status_btn);
        debug_status_btn = findViewById(R.id.debug_status_btn);
        frida_status_btn = findViewById(R.id.frida_status_btn);

        root_status_btn.setOnClickListener((v) -> {
            Intent intent = new Intent(MainActivity.this, Root_detection.class);
            startActivity(intent);
        });

        emulator_status_btn.setOnClickListener((v) -> {
            Intent intent = new Intent(MainActivity.this, Emulator_detection.class);
            startActivity(intent);
        });

        debug_status_btn.setOnClickListener((v) -> {
            Intent intent = new Intent(MainActivity.this, Debug_detection.class);
            startActivity(intent);
        });

        frida_status_btn.setOnClickListener((v) -> {
            Intent intent = new Intent(MainActivity.this, Frida_detection.class);
            startActivity(intent);
        });
    }
}