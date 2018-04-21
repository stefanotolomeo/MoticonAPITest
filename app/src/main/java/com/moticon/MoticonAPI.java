package com.moticon;

import android.content.Intent;
import android.content.IntentFilter;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.util.Log;

import com.dsi.ant.AntInterface;
import com.dsi.ant.AntInterfaceIntent;
import com.dsi.ant.exception.*;


public class MoticonAPI {
    private static final String TAG = "mapi-java";

    private final static int INSOLE_38_LEFT_ID = 0x088a0411;
    private final static int INSOLE_38_RIGHT_ID = 0x088b1411;

    private final static int INSOLE_42_LEFT_ID = 0x0c0b0611;
    private final static int INSOLE_42_RIGHT_ID = 0x0c0c1611;

    private AntInterface mAntReceiver;
    private Context mContext;
    private IntentFilter statusIntentFilter;
    private boolean is_enabled = false;

    public MoticonAPI(Context context) {
        mContext = context;
        mAntReceiver = new AntInterface();
    }

    // functions in C-moticonapi android.c
    // they are properly call at runtime even if they are highlighted in RED
    public native void setup_callback();
    public native void send_data_to_api(byte[] message);
    /* NATIVE METHODS that call functions in mapi-lib.cpp*/
    private native int mapiInit();
    private native int[] createInsoles(int leftID, int rightID);

    public void initialize(){
        //int mapitInitResult = mapiInit();
        //Log.e("MAPI_INIT", "Result is "+mapitInitResult);

        int[] insole_address = createInsoles(INSOLE_42_LEFT_ID, INSOLE_42_RIGHT_ID);
    }

    private void loadLibraries(){
        System.loadLibrary("moticonapi");
        System.loadLibrary("mapi-lib");
        System.loadLibrary("insoles-lib");
    }
    public int start() {
        Log.e(TAG, "Method START");
        statusIntentFilter = new IntentFilter();
        statusIntentFilter.addAction(AntInterfaceIntent.ANT_ENABLED_ACTION);
        statusIntentFilter.addAction(AntInterfaceIntent.ANT_ENABLING_ACTION);
        statusIntentFilter.addAction(AntInterfaceIntent.ANT_DISABLED_ACTION);
        statusIntentFilter.addAction(AntInterfaceIntent.ANT_DISABLING_ACTION);
        statusIntentFilter.addAction(AntInterfaceIntent.ANT_RESET_ACTION);
        statusIntentFilter.addAction(AntInterfaceIntent.ANT_INTERFACE_CLAIMED_ACTION);
        statusIntentFilter.addAction(Intent.ACTION_AIRPLANE_MODE_CHANGED);
        mContext.registerReceiver(mAntStatusReceiver, statusIntentFilter);

        loadLibraries();

        setup_callback(); // native function

        if (! AntInterface.hasAntSupport(mContext))
            return 1;

        return 0;
    }

    private void enable() {
        Log.e(TAG, "Method ENABLE");
        if (is_enabled)
            return;

        if (!mAntReceiver.initService(mContext, mAntServiceListener)) {
            Log.e(TAG, "Error init");
            return;
        }

        Log.i(TAG, "Service initialized");

        if (mAntReceiver.isServiceConnected()) {
            Log.i(TAG, "Service is connected");
            try {
                if (! mAntReceiver.hasClaimedInterface())
                    Log.i(TAG, "Interface not yet claimed -> do it now");
                    mAntReceiver.claimInterface();

                Log.i(TAG, "Enable interface");
                mAntReceiver.enable();

                Log.i(TAG, "Disable event buffering");
                mAntReceiver.ANTDisableEventBuffering();
            }
            catch (AntInterfaceException e)  {
                Log.e(TAG, "enable() Error: " + e);
            }
        }else{
            Log.e(TAG, "Service is not yet connected");
        }

        receiveAntRxMessages(true);
        is_enabled = true;
    }

    public void disable() {
        Log.e(TAG, "Metodo DISABLE");
        if (! is_enabled)
            return;

        is_enabled = false;
        receiveAntRxMessages(false);
        try {
            if (mAntReceiver.hasClaimedInterface())
                mAntReceiver.releaseInterface();

            mAntReceiver.stopRequestForceClaimInterface();
            mAntReceiver.releaseService();
        }
        catch (AntInterfaceException e)  {
            Log.e(TAG, "disable() Error " + e);
        }
    }

    public void destroy(){
        Log.e(TAG, "Metodo DESTROY");
        Log.i(TAG, "unregister Ant Receiver");
        mContext.unregisterReceiver(mAntStatusReceiver);
    }

    public String bytArrayToHex(byte[] a) {
        Log.e(TAG, "Metodo BYT_ARRY_TO_HEX");
           StringBuilder sb = new StringBuilder();
           for(byte b: a)
              sb.append(String.format("%02x ", b&0xff));
           return sb.toString();
    }

    // receive data from c-moticonapi and forward to antlib.jar
    public int send_data_to_ant(byte[] message) {
        Log.e(TAG, "Method SEND_DATA_TO_ANT");
        //Log.i(TAG, "send to ant: (" + message.length + ") " + bytArrayToHex(message));
        if (message.length == 1) {
            switch(message[0]) {
            case 0:
                enable();
                break;
            case 1:
                disable();
                break;
            }
            return 0;
        }

        try {
            mAntReceiver.ANTTxMessage(message);
        }
        catch (AntInterfaceException e)  {
            Log.e(TAG, "Error sending data to ant: " + e + " message to send: " + message);
            return 1;
        }
        return 0;
    }

    private AntInterface.ServiceListener mAntServiceListener = new AntInterface.ServiceListener()  {
            public void onServiceConnected() {
                Log.e(TAG, "mAntServiceListener onServiceConnected()");
                if (!is_enabled){
                    Log.i(TAG, "is not enabled");
                    return;
                }

                try {
                    if (!mAntReceiver.hasClaimedInterface()){
                        Log.i(TAG, "interface not claimed, do it now");
                        if (mAntReceiver.claimInterface()){
                            Log.i(TAG, "interface succesfully claimed");
                        }else{
                            Log.e(TAG, "Error while claiming interface");

                            // this is needed for ANT-api 4.0 to work
                            mAntReceiver.requestForceClaimInterface("Moticon-APP");
                        }
                    }else{
                        Log.i(TAG, "mAntReceiver.hasClaimedInterface() already done");
                    }
                }
                catch (AntInterfaceException e)  {
                        Log.e(TAG, "Error:" + e);
                }
            }

            public void onServiceDisconnected()  {
                Log.e(TAG, "mAntServiceListener onServiceDisconnected()");
            }
        };


    // enable or disable receiving ANT messages (setup callback to mAntMessageReceiver)
    private void receiveAntRxMessages(boolean register) {
        Log.e(TAG, "Metodo RECEIVE_ANT_RX_MESSAGES");
        if(register)  {
            Log.i(TAG, "setup callback to mAntMessageReceiver -> receiveAntRxMessages: START");
            mContext.registerReceiver
                (mAntMessageReceiver,
                 new IntentFilter(AntInterfaceIntent.ANT_RX_MESSAGE_ACTION));
        }
        else {
            try {
                Log.i(TAG, "remove callback to mAntMessageReceiver -> receiveAntRxMessages: STOP");
                mContext.unregisterReceiver(mAntMessageReceiver);
            }
            catch(IllegalArgumentException e) {
                Log.i(TAG, "Error unregisterReceiver failed: " + e);
            }
        }
    }

    // receive data messages from ANT and forward to c-moticonapi
    private final BroadcastReceiver mAntMessageReceiver = new BroadcastReceiver() {
            public void onReceive(Context context, Intent intent) {
                Log.e(TAG, "Metodo ONRECEIVE di mAntMessageReceiver");
                byte[] message = intent.getByteArrayExtra(AntInterfaceIntent.ANT_MESSAGE);
                Log.e(TAG, "receive from ANT: "+intent.getAction() + " -> send to moticonapi: (" + message.length + ") " + bytArrayToHex(message));
                send_data_to_api(message);
            }
        };

    // receive Status messages from ANT
    private final BroadcastReceiver mAntStatusReceiver = new BroadcastReceiver() {
       public void onReceive(Context context, Intent intent)
       {
           Log.e(TAG, "Metodo ONRECEIVE di mAntStatusReceiver");
          String ANTAction = intent.getAction();
          Log.i(TAG, "got ANT status: " + ANTAction );
          try {
              if (!mAntReceiver.isEnabled()){
                  Log.i(TAG, "receiver is not yet enabled, do it now: ");
                  mAntReceiver.enable();
              }else{
                  Log.i(TAG, "receiver seems already enabled");
              }
          }
          catch (AntInterfaceException e)  {
              Log.e(TAG, "mAntReceiver.enable() Error: " + e);
          }
       }
    };
}
