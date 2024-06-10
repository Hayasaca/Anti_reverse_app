package com.bao_mat_web.anti_reverse_app;

import android.os.Build;
import android.os.Bundle;
import android.telephony.TelephonyManager;
import android.util.Log;
import android.widget.TextView;

import androidx.activity.EdgeToEdge;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.graphics.Insets;
import androidx.core.view.ViewCompat;
import androidx.core.view.WindowInsetsCompat;

public class Emulator_detection extends AppCompatActivity {

    TextView emulator_status;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        EdgeToEdge.enable(this);
        setContentView(R.layout.activity_emulator_detection);

        emulator_status = findViewById(R.id.emulator_status);
        showEmulatorStatus();
    }

    private void showEmulatorStatus() {
        Boolean isEmulator = checkIfDeviceIsEmulator();
        if(isEmulator==true)
        {
            emulator_status.setText("Emulator!!");
        }
        else
        {
            emulator_status.setText("Not emulator!!");
        }
    }

    private Boolean checkIfDeviceIsEmulator() {
        Log.d("Build fingerprint", Build.FINGERPRINT);
        Log.d("Build model", Build.MODEL);
        Log.d("Build manufacturer", Build.MANUFACTURER);
        Log.d("Build brand", Build.BRAND);
        Log.d("Build device", Build.DEVICE);
        Log.d("Build product", Build.PRODUCT);

        if(Build.FINGERPRINT.startsWith("generic")
                || Build.FINGERPRINT.startsWith("unknown")
                || Build.MODEL.contains("google_sdk")
                || Build.MODEL.contains("Emulator")
                || Build.MODEL.contains("Android SDK built for x86")
                || Build.MANUFACTURER.contains("Genymobile")
                || (Build.BRAND.startsWith("generic") && Build.DEVICE.startsWith("generic"))
                || "google_sdk".equals(Build.PRODUCT))
        {
            return true;
        }
        return false;
    }
}