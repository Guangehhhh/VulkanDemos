package com.monkey.sample;

import android.app.AlertDialog;
import android.app.NativeActivity;
import android.content.DialogInterface;
import android.content.pm.ApplicationInfo;
import android.os.Bundle;
import java.util.concurrent.Semaphore;

public class VulkanActivity extends NativeActivity {

    static {
        System.loadLibrary("monkeyandroid-lib");
    }
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    }
    
    private final Semaphore semaphore = new Semaphore(0, true);

    public void showAlert(final String message) {

        final VulkanActivity activity = this;

        ApplicationInfo applicationInfo = activity.getApplicationInfo();
        final String applicationName = applicationInfo.nonLocalizedLabel.toString();

        this.runOnUiThread(new Runnable() {
           public void run() {
               AlertDialog.Builder builder = new AlertDialog.Builder(activity, android.R.style.Theme_Material_Dialog_Alert);
               builder.setTitle(applicationName);
               builder.setMessage(message);
               builder.setPositiveButton("Close", new DialogInterface.OnClickListener() {
                   public void onClick(DialogInterface dialog, int id) {
                       semaphore.release();
                   }
               });
               builder.setCancelable(false);
               AlertDialog dialog = builder.create();
               dialog.show();
           }
        });

        try {
            semaphore.acquire();
        }
        catch (InterruptedException e) { 

        }
    }

}
