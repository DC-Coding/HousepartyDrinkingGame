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
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;


public class LoginActivity extends AppCompatActivity implements View.OnClickListener {

    private Button btnLogin;
    private EditText etUsername;
    private SharedPreferences sharedPreferences;
    public static ConnectionAdapter connectionAdapter;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        btnLogin = findViewById(R.id.btnLogin);
        btnLogin.setOnClickListener(this);
        etUsername = findViewById(R.id.etUsername);

        // use sharedPreferences to get the stored data
        sharedPreferences = this.getSharedPreferences("com.example.minigames", Context.MODE_PRIVATE);
        if (sharedPreferences.getBoolean("saveUsername", true)) {
            etUsername.setText(sharedPreferences.getString("username", ""));
        }

        connectionAdapter = new ConnectionAdapter("coffeecrew.ddns.net", 12345, this,
                (TextView) findViewById(R.id.tvChallenges));
        connectionAdapter.connect();
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        MenuInflater inflater = getMenuInflater();
        inflater.inflate(R.menu.menu_top_right, menu);
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
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.btnLogin:
                String username;
                username = etUsername.getText().toString();

                // check if the username is valid (wether none, nor already taken)
                if (username.length() < 3) {
                    Toast.makeText(this, "Benutzername ist zu kurz!\n(min. 3 Buchstaben)", Toast.LENGTH_SHORT).show();
                    break;
                }

                else if (false){
                    // check if the username is already taken
                }

                else {
                    Toast.makeText(this, "Dein Benutzername: " + username, Toast.LENGTH_SHORT).show();

                    // store the username if the user wants to save his name
                    if (sharedPreferences.getBoolean("saveUsername", true)) {
                        sharedPreferences.edit().putString("username", etUsername.getText().toString()).apply();
                    }

                    connectionAdapter.send(username);

                    Intent intent = new Intent(this, GameActivity.class);
                    startActivity(intent);
                    finish();
                    break;
                }
        }
    }

    public static ConnectionAdapter getConnectionAdapter() {
        return connectionAdapter;
    }
}
