package com.aislab.moticonapitest;

import android.util.Log;

public class Mapi {

    private final static int INSOLE_38_LEFT_ID = 0x088a0411;
    private final static int INSOLE_38_RIGHT_ID = 0x088b1411;

    private final static int INSOLE_42_LEFT_ID = 0x0c0b0611;
    private final static int INSOLE_42_RIGHT_ID = 0x0c0c1611;

    private final static String INSOLES_LICENCE = "121dd4bd1dba7a00cf060b5f5eaa6ed2";

    private final static int DATA_10_HZ = 1;
    private final static int DATA_20_HZ = 2;
    private final static int DATA_50_HZ = 3;
    private final static int DATA_100_HZ = 4;

    /*private InsoleAPI insole_l;
    private InsoleAPI insole_r;*/
    int[] insole_address;

    private native long mapiVersion();
    private native void mapiFinish();
    private native int mapiReset();
    private native int mapiInit();
    private native boolean mapiWaitForEvents(long timeout);


    private native int[] createInsoles(int leftID, int rightID);
    private native boolean connectInsoles(int leftInsole, int rightInsole);
    private native void disconnectInsoles(int leftInsole, int rightInsole);
    private native void destroyInsoles(int leftInsole, int rightInsole);
    private native void switchMode(int leftInsole, int rightInsole, int mode);
    private native boolean checkEvent(int insole);
    //private native ??? readData(int insole);


    public void start(){
        /*insole_l = new InsoleAPI();
        insole_r = new InsoleAPI();*/

        int mapitInitResult = mapiInit();

        insole_address = createInsoles(INSOLE_42_LEFT_ID, INSOLE_42_RIGHT_ID);
        if(insole_address == null){
            Log.e("Mapi", "Cannot create insoles, null object returned");
            return;
        }
        boolean connected = connectInsoles(insole_address[0], insole_address[1]);
        if(!connected){
            Log.e("Mapi", "Connection false, at least one insoles not connected");
            return;
        }

        switchMode(insole_address[0], insole_address[1], DATA_100_HZ);
        int count = 0;

        while (count < 1000){

            boolean success = mapiWaitForEvents(500);
            if(!success){
                continue;
            }
            boolean successLeft = checkEvent(insole_address[0]);
            if(successLeft){
                // check and print data insoles left
            }
            boolean successRight = checkEvent(insole_address[1]);
            if(successRight){
                // check and print data insoles right
            }

            count++;
        }
    }




    /*public InsoleAPI getInsole_l() {
        return insole_l;
    }

    public void setInsole_l(InsoleAPI insole_l) {
        this.insole_l = insole_l;
    }

    public InsoleAPI getInsole_r() {
        return insole_r;
    }

    public void setInsole_r(InsoleAPI insole_r) {
        this.insole_r = insole_r;
    }*/
}
