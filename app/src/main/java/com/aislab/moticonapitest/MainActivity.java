package com.aislab.moticonapitest;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

import com.moticon.MoticonAPI;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        TextView tv = (TextView) findViewById(R.id.sample_text);
        tv.setText(stringFromJNI());

        MoticonAPI m = new MoticonAPI(this);
        m.start();
        m.initialize();
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    /**
     * Get the version of the moticon api.
     * @return The version encoded in a long (C: unsigned 32bit int).
     * Use the Version Macros to decode this value
     */
   /* public native long mapiVersion();
    public native int mapiInit();
    public native void mapiFinish();
    public native int mapiReset();
    public native int mapiWaitForEvents(long timeout);

    public native boolean connectInsoles(int leftInsole, int rightInsole);
    public native void disconnectInsoles(int leftInsole, int rightInsole);
    public native void destroyInsoles(int leftInsole, int rightInsole); */

    public native boolean insoleIsConnected(int insole);
    public native boolean insoleIsDisconnected(int insole);
    public native boolean insoleIsSearching(int insole);

    public native int getID(int insole);
    public native int getSize(int insole);
    public native char getSide(int insole);
    //public native void switchMode(int leftInsole, int rightInsole, int mode);
    public native String getMode(int insole);
}
