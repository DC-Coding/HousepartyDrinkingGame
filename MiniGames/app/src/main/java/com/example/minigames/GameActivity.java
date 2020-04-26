package com.example.minigames;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.app.AlertDialog;
import android.content.Intent;
import android.os.Build;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

public class GameActivity extends AppCompatActivity implements View.OnClickListener {

    private Button btnDone, btnChoice1, btnChoice2;
    private TextView tvChallenges;
    private ConnectionAdapter connectionAdapter;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_game);

        btnDone = findViewById(R.id.btnDone);
        btnChoice1 = findViewById(R.id.btnChoice1);
        btnChoice2 = findViewById(R.id.btnChoice2);
        btnDone.setOnClickListener(this);
        btnChoice1.setOnClickListener(this);
        btnChoice2.setOnClickListener(this);
        tvChallenges = findViewById(R.id.tvChallenges);

        connectionAdapter = LoginActivity.getConnectionAdapter();
        connectionAdapter.setContext(this);
        connectionAdapter.setTextView(tvChallenges);
        connectionAdapter.receive();

    }

    @Override
    public void onBackPressed() {
        // give the user a hint, that the app will be closed
        Toast.makeText(this, "Die App wird geschlossen!", Toast.LENGTH_SHORT).show();

        // close the socket and the app
        connectionAdapter.closeConnection();
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
            finishAndRemoveTask();
        }
        else {
            finishAffinity();
        }
        super.onBackPressed();
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
            case R.id.btnDone:
                btnDone.setEnabled(false);
                connectionAdapter.send("Done");
                connectionAdapter.receive();
                break;
            case R.id.btnChoice1:
                btnChoice1.setEnabled(false);
                btnChoice2.setEnabled(false);
                connectionAdapter.send("0");
                connectionAdapter.receive();
                break;
            case R.id.btnChoice2:
                btnChoice1.setEnabled(false);
                btnChoice2.setEnabled(false);
                connectionAdapter.send("1");
                connectionAdapter.receive();
                break;
        }
    }
}
