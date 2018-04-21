package com.moticon;

import android.app.IntentService;
import android.app.NotificationManager;
import android.content.Intent;
import android.content.Context;
import android.os.Binder;
import android.os.IBinder;
import android.util.Log;

import com.aislab.moticonapitest.R;

public class MyIntentService extends IntentService {

    private NotificationManager mNM;

    // Unique Identification Number for the Notification.
    // We use it on Notification start, and to cancel it.
    private int NOTIFICATION = 118;
    private final IBinder mBinder = new LocalBinder();

    /**
     * Class for clients to access.  Because we know this service always
     * runs in the same process as its clients, we don't need to deal with
     * IPC.
     */
    public class LocalBinder extends Binder {
        MyIntentService getService() {
            return MyIntentService.this;
        }
    }
    public MyIntentService() {
        super("MyIntentService");
    }


    @Override
    protected void onHandleIntent(Intent intent) {
        Log.e("E che cazzzo", "Sono io");
    }

    @Override
    public IBinder onBind(Intent intent) {
        Log.e("Service", "OnBind Method");
        return mBinder;
    }


}
