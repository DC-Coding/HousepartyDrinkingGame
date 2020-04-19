package com.example.minigames;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.app.AlertDialog;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.widget.CheckBox;
import android.widget.CompoundButton;

public class SettingsActivity extends AppCompatActivity implements CompoundButton.OnCheckedChangeListener {

    CheckBox cbSaveUsername;
    SharedPreferences sharedPreferences;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_settings);

        // use sharedPreferences to store data
        sharedPreferences = getSharedPreferences("com.example.minigames", Context.MODE_PRIVATE);

        cbSaveUsername = findViewById(R.id.cbSaveUsername);
        cbSaveUsername.setOnCheckedChangeListener(this);

        if (sharedPreferences.getBoolean("saveUsername", true)) {
            cbSaveUsername.setChecked(true);
        }
        else {
            cbSaveUsername.setChecked(false);
        }
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        MenuInflater inflater = getMenuInflater();
        inflater.inflate(R.menu.menu_top_right, menu);
        // disable the option "settings" while settings activity is open
        menu.findItem(R.id.menuSettings).setEnabled(false);
        return super.onCreateOptionsMenu(menu);
    }

    @Override
    public boolean onOptionsItemSelected(@NonNull MenuItem item) {
        super.onOptionsItemSelected(item);

        switch (item.getItemId()) {
            case R.id.menuSettings:
                Intent intent = new Intent(this, SettingsActivity.class);
                startActivity(intent);
                return true;
            case R.id.menuAbout:
                // show a small AlertDialog
                new AlertDialog.Builder(this)
                        .setIcon(android.R.drawable.ic_dialog_info)
                        .setTitle("Über uns")
                        .setMessage("Entwickelt von:\nDaniel\nJohanna\nLucas\nMichael")
                        .setNeutralButton("Ok", null)
                        .show();
                return true;
            default:
                return false;
        }
    }

    @Override
    public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
        // save the changed data
        if (isChecked) {
            sharedPreferences.edit().putBoolean("saveUsername", true).apply();
        }
        else {
            sharedPreferences.edit().putBoolean("saveUsername", false).apply();
            sharedPreferences.edit().remove("username").apply();
        }
    }
}
