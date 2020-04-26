package com.example.minigames;

import android.app.ProgressDialog;
import android.content.Context;
import android.os.AsyncTask;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.concurrent.ExecutionException;


public class ConnectionAdapter {
    private Socket socket;
    private Context context;
    private String ip;
    private int port;
    private TextView tvMessages;
    private Button btnDone, btnChoice1, btnChoice2;

    // classes used in ConnectionAdapter
    public class ConnectionStarter extends AsyncTask<Void, Void, Void> {
        private boolean connectSuccess = false;
        private ProgressDialog progressDialog;

        @Override
        protected Void doInBackground(Void... voids) {
            try {
                socket = new Socket(ip, port);
                connectSuccess = true;
                //OutputStream stream = socket.getOutputStream();
                //stream.write("Connection created!".getBytes());
            } catch (UnknownHostException e) {
                Log.i("MyInfo", String.valueOf(e));
                e.printStackTrace();
            } catch (IOException e) {
                Log.i("MyInfo", String.valueOf(e));
                e.printStackTrace();
            } catch (Exception e) {
                Log.i("MyInfo", String.valueOf(e));
                e.printStackTrace();
            }
            return null;
        }

        @Override
        protected void onPreExecute()
        {
            // show a progress dialog
            progressDialog = ProgressDialog.show(context, "Verbinden...", "Verbindung zum Server wird hergestellt.\nBitte warten!");
        }

        @Override
        protected void onPostExecute(Void aVoid) {
            super.onPostExecute(aVoid);

            if (connectSuccess) {
                Toast.makeText(context, "Verbunden", Toast.LENGTH_SHORT).show();
            }
            else {
                Toast.makeText(context, "Verbindung fehlgeschlagen!", Toast.LENGTH_SHORT).show();
            }

            progressDialog.dismiss();
        }
    }

    public class MessageSender extends AsyncTask<Void, Void, Void> {
        private String message;

        public MessageSender(String message) {
            this.message = message;
        }

        @Override
        protected Void doInBackground(Void... voids) {
            OutputStream stream = null;
            try {
                stream = socket.getOutputStream();
                //stream.write("Button geklickt!".getBytes());
                stream.write(message.getBytes());
            } catch (Exception e) {
                try {
                    Toast.makeText(context, "Nachricht nicht gesendet. Internetprobleme?", Toast.LENGTH_SHORT).show();
                } catch (Exception ex) {
                    ex.printStackTrace();
                }
                e.printStackTrace();
            }
            return null;
        }
    }

    public class MessageReceiver extends AsyncTask<Void, Void, Void> {
        String message = "";

        @Override
        protected Void doInBackground(Void... voids) {
            BufferedReader bufferedReader;
            try {
                bufferedReader = new BufferedReader(
                        new InputStreamReader(
                                socket.getInputStream()));

                char[] buffer = new char[255];
                int counter = bufferedReader.read(buffer, 0, 255);
                message = new String(buffer, 0, counter);
            } catch (Exception e) {
                e.printStackTrace();
            }

            //Log.i("MyInfo", message);

            ((GameActivity)context).runOnUiThread(new Runnable() {
                public void run() {
                    // enable the buttons
                    try {
                        btnDone.setEnabled(true);
                        btnChoice1.setEnabled(true);
                        btnChoice2.setEnabled(true);
                    } catch (Exception e) {
                        e.printStackTrace();
                    }

                    try {
                        JSONObject jsonObject = new JSONObject(message);
                        if (jsonObject.getString("Category").equals("1")) {
                            // poll
                            // make the 2 buttons and the one in the middle visible / invisible
                            //Log.i("MyInfo", "Category: Abstimmung");
                            btnDone.setVisibility(View.INVISIBLE);
                            btnChoice1.setVisibility(View.VISIBLE);
                            btnChoice2.setVisibility(View.VISIBLE);

                            btnChoice1.setText(jsonObject.getJSONArray("Options").getString(0));
                            btnChoice2.setText(jsonObject.getJSONArray("Options").getString(1));

                        }
                        else {
                            // normal task
                            //Log.i("MyInfo", "Category: Standard");
                            btnDone.setVisibility(View.VISIBLE);
                            btnChoice1.setVisibility(View.INVISIBLE);
                            btnChoice2.setVisibility(View.INVISIBLE);
                        }

                        String title = jsonObject.getString("String");
                        tvMessages.setText(title);
                    } catch (JSONException e) {
                        e.printStackTrace();
                        Log.i("MyInfo", String.valueOf(e));
                        Toast.makeText(context, "Es ist ein Fehler aufgetreten.", Toast.LENGTH_SHORT).show();
                    }
                }
            });

            return null;
        }
    }


    /***********************************************************************************************************************/
    // Methods

    public ConnectionAdapter(String ip, int port, Context context, TextView tvMessages) {
        this.ip = ip;
        this.port = port;
        this.context = context;
        this.tvMessages = tvMessages;
    }

    public boolean connect() {

        // set time limit!!
        ConnectionStarter connectionStarter = new ConnectionStarter();
        connectionStarter.execute();

        return false;
    }

    public void send(String message) {

        // set time limit!!
        if (isConnected()) {
            MessageSender messageSender = new MessageSender(message);
            messageSender.execute();
        }
        else {
            Toast.makeText(context, "Nachricht konnte nicht gesendet werden!", Toast.LENGTH_SHORT).show();
        }
    }

    public void receive() {

        // set time limit!!
        if (isConnected()) {
            MessageReceiver messageReceiver = new MessageReceiver();
            try {
                messageReceiver.execute().get();
            } catch (ExecutionException e) {
                e.printStackTrace();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        else {
            Toast.makeText(context, "Fehler beim Empfangen einer Nachricht!", Toast.LENGTH_SHORT).show();
        }
    }

    public void setContext(Context context) {
        this.context = context;
        // add the buttons (new context)
        this.btnDone = ((GameActivity)context).findViewById(R.id.btnDone);
        this.btnChoice1 = ((GameActivity)context).findViewById(R.id.btnChoice1);
        this.btnChoice2 = ((GameActivity)context).findViewById(R.id.btnChoice2);
    }

    public void closeConnection() {
        try {
            this.socket.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void setTextView(TextView tvMessages) {
        this.tvMessages = tvMessages;
    }

    public boolean isConnected() {
        return socket.isConnected();
    }
}