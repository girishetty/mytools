/*
 * Copyright (C) 2009,2010 Markus Bode Internet sungen (bolutions.com)
 * 
 * Licensed under the GNU General Public License v3
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @author Markus Bode
 * @version $Id: StartActivity.java 727 2011-01-02 13:04:32Z markus $
 */

package com.android.webserver;

import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.File;
import java.util.Date;
import java.util.List;
import java.util.Vector;
import java.util.Collections;
import java.net.InetAddress;
import java.net.NetworkInterface;
import org.apache.http.conn.util.InetAddressUtils;

import com.android.webserver.R;

import android.app.Activity;
import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;

import android.content.Intent;
import android.content.IntentFilter;
import android.content.BroadcastReceiver;
import android.content.Context;

import android.net.wifi.WifiManager;
import android.net.wifi.WifiConfiguration;
import android.net.wifi.WifiConfiguration.AuthAlgorithm;
import android.net.wifi.WifiConfiguration.KeyMgmt;
import android.net.wifi.ScanResult;

import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.View;
import android.widget.ScrollView;
import android.widget.TextView;

public class StartActivity extends Activity {
    private Server mServer;
    private final int mPort = 8080;
    private static TextView mLog;
    private static ScrollView mScroll;

    private NotificationManager mNotificationManager;
    private WifiManager mWifiManager;
    private WifiConfiguration mWifiConf;
    private IntentFilter mIntentFilter;
    private List<String> mWifiList;
    private boolean mScanIssued = false;
    private boolean mScanCompleted = false;
    private boolean mNotStarted = true;
    private boolean mStartedSoftAP = false;

    private final int NO_STEP = 0;
    private final int START_SOFT_AP = 1;
    private final int STOP_SOFT_AP = 2;
    private final int ENABLE_WIFI = 3;
    private final int DISABLE_WIFI = 4;
    private final int SCAN_WIFI = 5;
    private final int ADD_NETWORK = 6;
    private int mNextStep = NO_STEP;

    private final String TYPE_ETHERNET = "eth0";
    private final String TYPE_WIFI = "wlan0";
    private final String TYPE_P2P = "p2p0";
    private final String TYPE_UAP = "uap0";
    public static final String HTML_PATH = "/sdcard/device_config/";

    private static final String TAG = "WebServer";
    
    final Handler mHandler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            Bundle b = msg.getData();
            log(b.getString("msg"));
        }
    };

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        mNotificationManager = (NotificationManager) getSystemService(NOTIFICATION_SERVICE);
        
        mLog = (TextView) findViewById(R.id.log);
        mScroll = (ScrollView) findViewById(R.id.ScrollView01);

        mWifiList = new Vector<String>();
        mWifiManager = (WifiManager) getSystemService(WIFI_SERVICE);

        //Add the Broadcast Intent for Soft AP creation and Wifi Scan Result
        mIntentFilter = new IntentFilter(WifiManager.WIFI_AP_STATE_CHANGED_ACTION);
        mIntentFilter.addAction(WifiManager.SCAN_RESULTS_AVAILABLE_ACTION);
        mIntentFilter.addAction(WifiManager.WIFI_STATE_CHANGED_ACTION);
        registerReceiver (new WiFiScanReceiver(), mIntentFilter);
        log("");

        //Enable WiFi to begin with, and initiate enable SoftAP
        /*
         * When the service comes up, Enable the WiFi first
         * By default we are using "uap" or the SoftAP as the interface to run the webserver
         * But, we can also use ethernet as the host for webserver, in which case use "SCAN_WIFI" to set next step as below
         *
         * mNextStep = SCAN_WIFI;
         */
        mNextStep = START_SOFT_AP;
        enableWifi();

        //Create the default web-pages
        createServerWebPages();
    }

    private void startServer(String intfType, int port) {
        try {
            mNotStarted = false;
            String ipAddress = getIpAddress(intfType);

            log("StartActivity: Starting mServer on "+ipAddress + ":" + port);
            mServer = new Server(ipAddress, port, mHandler, this);
            mServer.start();
		    
            Intent i = new Intent(this, StartActivity.class);
            PendingIntent contentIntent = PendingIntent.getActivity(this, 0, i, 0);

            Notification notif = new Notification(R.drawable.icon, "Webserver is running", System.currentTimeMillis());
            notif.setLatestEventInfo(this, "Webserver", "Webserver is running", contentIntent);
            notif.flags = Notification.FLAG_NO_CLEAR;
            mNotificationManager.notify(1234, notif);
    	} catch (Exception e) {
            log("StartActivity:" + e.getMessage());
            mNotStarted = true;
        }
    }

    private void stopServer() {
        if( mServer != null ) {
            mServer.stopServer();
            mServer.interrupt();
            log("StartActivity: Server was killed.");
            mNotificationManager.cancelAll();
            mNotStarted = true;
        }
        else {
            log("StartActivity: Cannot kill mServer!? Please restart your phone.");
        }
    }

    private void createServerWebPages() {

        ServerWebPage webPage = new ServerWebPage403 ();
        webPage.createWebPage();
        webPage = null;
        webPage = new ServerWebPage404 ();
        webPage.createWebPage();
        webPage = null;
        webPage = new ServerWebPageIndex ();
        webPage.createWebPage();
        webPage = null;
        webPage = new ServerWebPageStatus ();
        webPage.createWebPage();
    }
    
    public static void log( String s ) {
        mLog.append(s + "\n");
        mScroll.fullScroll(ScrollView.FOCUS_DOWN);
        Log.i(TAG, s);
    }
    
    /**
     * Get IP address of given interface type
     * @param String interface type (such as "eth0", "wlan0", "uap0" or "p2p0"
     * @return  address or empty string
     */
    public static String getIpAddress(String intfType) {
        try {
            List<NetworkInterface> interfaces = Collections.list(NetworkInterface.getNetworkInterfaces());
            for (NetworkInterface intf : interfaces) {
                String dispName = intf.getDisplayName();
                if (dispName.equals(intfType)) {
                    List<InetAddress> addrs = Collections.list(intf.getInetAddresses());
                    for (InetAddress addr : addrs) {
                        if (!addr.isLoopbackAddress()) {
                            String sAddr = addr.getHostAddress().toUpperCase();
                            boolean isIPv4 = InetAddressUtils.isIPv4Address(sAddr); 
                            if (isIPv4) {
                                return sAddr;
                            }
                        }
                    }
                }
            }
        } catch (Exception ex) { /* ignoring the exceptions */ }
        return "";
    }

    void disableWifi() {

        if (mWifiManager.isWifiEnabled() == true) {
            mWifiManager.setWifiEnabled(false);
        } else if (mNextStep == ENABLE_WIFI) {
            //Already disabled, Enable now, without starting SoftAP
            mNextStep = ADD_NETWORK;
            enableWifi();
        } else if (mNextStep == STOP_SOFT_AP) {
            //Already disabled, Enable now, without starting SoftAP
            mNextStep = ENABLE_WIFI;
            mWifiManager.setWifiApEnabled(null, false);
        }
    }

    void enableWifi() {

        if (mWifiManager.isWifiEnabled() == false) {
            mWifiManager.setWifiEnabled(true);
        }
        else if (mNextStep == START_SOFT_AP) {
            //WiFi is Already enabled, so can start SoftAP
            if (mStartedSoftAP == false) {
                setupWifiSoftAp();
            }   
        }
        else if (mNextStep == SCAN_WIFI) {
            //WiFi is Already enabled, and we are not using SoftAP
            //So we can launch the web-Server on eth0 interface and also start a WiFi Scan
            if(mScanIssued == false) {
                mScanIssued = scanWifi();
            }
            if(mNotStarted) {
                startServer(TYPE_ETHERNET, mPort);
            }
            mNextStep = NO_STEP;
        }
    }

    void setupWifiSoftAp() {
        /*
         *  Create Wifi Soft AP with some SSID and Password
         *  Soft AP saves the config data in "/data/misc/wifi/softap.conf"
         *  A Random name and OTP is preferred to for the security reason
         */
        String softAPSSID = "BG2CDAP";
        String softAPPass = "12345678";
        WifiConfiguration config = new WifiConfiguration();
        config.SSID = softAPSSID;
        config.preSharedKey = softAPPass;
        config.allowedKeyManagement.set(KeyMgmt.WPA2_PSK);
        config.allowedAuthAlgorithms.set(AuthAlgorithm.OPEN);

        if (mWifiManager.setWifiApEnabled(config, true)) {
            log("StartActivity: Enabling Soft AP for uap0");
        } else {
            log("StartActivity: Failed to Enable  Soft AP for uap0!!");
            //ToDo: We have to terminate the Process here
        }
    }

    //Enable the WiFi and issue a scan in the very beginign to save the time later
    boolean scanWifi() {
        //registerReceiver (new WiFiScanReceiver(), new IntentFilter(WifiManager.SCAN_RESULTS_AVAILABLE_ACTION));  
        return mWifiManager.startScan();
    }

    boolean connectToWiFiAP(String wifiAP, String wifiPASS) {

        boolean success = true;
        mWifiConf = null;
        mWifiConf = new WifiConfiguration();
        mWifiConf.SSID = "\"" + wifiAP + "\"";
        mWifiConf.preSharedKey  = "\"" + wifiPASS + "\"";
        mWifiConf.hiddenSSID = true;
        mWifiConf.status = WifiConfiguration.Status.ENABLED;

        mWifiConf.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.TKIP);
        mWifiConf.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.CCMP);
        mWifiConf.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.WPA_PSK);
        mWifiConf.allowedPairwiseCiphers.set(WifiConfiguration.PairwiseCipher.TKIP);
        mWifiConf.allowedPairwiseCiphers.set(WifiConfiguration.PairwiseCipher.CCMP);
        mWifiConf.allowedProtocols.set(WifiConfiguration.Protocol.RSN);

        //Start the chain: disable soft ap --> disableWifi -> EnableWifi -> AddNetwork and EnableNetwork
        mNextStep = DISABLE_WIFI;
        mWifiManager.setWifiApEnabled(null, false);

        return success;
    }

    boolean addAndEnableNetwork() {

        int netId = mWifiManager.addNetwork(mWifiConf);
        boolean success = mWifiManager.enableNetwork(netId, true);
        if (success) {
            //Save the configuration so that for every bootup, the device will connect to this wireless network
            mWifiManager.saveConfiguration();
        }
        return success;
    }
    
    List<String> getWiFiAPList() {

        if (!mScanIssued || mScanCompleted == false) {
            mScanCompleted = false;
            //Try scanning for WiFi APs again
            mScanIssued = scanWifi();
        }
        return mWifiList;
    }

    void setWiFiAPList() {

        List<ScanResult> wifiAPs = mWifiManager.getScanResults();
        int count = wifiAPs.size();
        if (count > 0 ) {
            //Clear the List before adding anything
            mWifiList.clear();

            String wifiAP = "";
            log("StartActivity:setWiFiAPList: Found " + count + " WiFi APs");

            for (int index = 0; index < count; index++) {
                wifiAP = wifiAPs.get(index).SSID;
                mWifiList.add(wifiAP);
            }
        }
    }

    class WiFiScanReceiver extends BroadcastReceiver {

        @Override
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            if (WifiManager.WIFI_STATE_CHANGED_ACTION.equals(action)) {
                int wifiState = intent.getIntExtra(WifiManager.EXTRA_WIFI_STATE, WifiManager.WIFI_STATE_UNKNOWN);
                if (wifiState == WifiManager.WIFI_STATE_ENABLED) {
                    //WiFi is enabled now, so can start SoftAP if thats what we are expected to do
                    if (mStartedSoftAP == false && mNextStep == START_SOFT_AP) {
                        log("StartActivity: WiFiScanReceiver::OnReceive: WiFi is ON, can start SoftAP Now");
                        setupWifiSoftAp();
                        mNextStep = NO_STEP;
                    } else if (mNextStep == SCAN_WIFI) {
                        log("StartActivity: WiFiScanReceiver::OnReceive: WiFi is ON, can scan wif and start server");
                        //Now we can launch the web-Server on eth0 interface and also start a WiFi Scan
                        if(mScanIssued == false) {
                            log("StartActivity: WiFiScanReceiver::OnReceive: Issuing WiFiScan");
                            mScanIssued = scanWifi();
                        }
                        if(mNotStarted) {
                            startServer(TYPE_ETHERNET, mPort);
                        }
                        mNextStep = NO_STEP;
                    } else if (mNextStep == ADD_NETWORK) {
                        //Now its time to Add and Connect to the wifi network
                        log("StartActivity: WiFiScanReceiver::OnReceive: WiFi is ON, can add network now");
                        addAndEnableNetwork();
                        mNextStep = NO_STEP;
                    }
                } else if (wifiState == WifiManager.WIFI_STATE_DISABLED) {
                    //Wifi is Diabled, and we can enale the WiFi again
                    log("StartActivity: WiFiScanReceiver::OnReceive: WiFi is OFF now");
                    if (mNextStep == ENABLE_WIFI) {
                        log("StartActivity: WiFiScanReceiver::OnReceive: Set WiFi to ON");
                        mNextStep = ADD_NETWORK;
                        enableWifi();
                    } else if (mNextStep == STOP_SOFT_AP) {
                        log("StartActivity: WiFiScanReceiver::OnReceive: Disable Soft AP");
                        mNextStep = ENABLE_WIFI;
                        mWifiManager.setWifiApEnabled(null, false);
                    }
                } else if (wifiState == WifiManager.WIFI_STATE_UNKNOWN && mNextStep == ENABLE_WIFI) {
                    log("StartActivity: WiFiScanReceiver::OnReceive: WiFi is in Unknown State, but lets try to set WiFi to ON");
                    mNextStep = ADD_NETWORK;
                    enableWifi();
                } else {
                    log("StartActivity: WiFiScanReceiver::OnReceive:1: Unknown Event: " + action + " wifiState: " + wifiState);
                }
            } else if (WifiManager.WIFI_AP_STATE_CHANGED_ACTION.equals(action)) {
                int wifiApState = intent.getIntExtra(WifiManager.EXTRA_WIFI_AP_STATE, WifiManager.WIFI_AP_STATE_FAILED);
                if (wifiApState == WifiManager.WIFI_AP_STATE_ENABLED) {
                    if (mStartedSoftAP == false) {
                        mStartedSoftAP = true;
                        log("StartActivity: WiFiScanReceiver::OnReceive: Soft AP uap0 created");
                        //softap - uap0 created
                        //Now we can launch the web-Server on upa0 interface and also start a WiFi Scan
                        if(mScanIssued == false) {
                            mScanIssued = scanWifi();
                        }
                        if(mNotStarted) {
                            startServer(TYPE_UAP, mPort);
                        }
                        mNextStep = NO_STEP;
                    }
                } else if (wifiApState == WifiManager.WIFI_AP_STATE_DISABLED) {
                    //SoftAP is disabled now, now lets disable/enable wifi
                    log("StartActivity: WiFiScanReceiver::OnReceive: SoftAP is OFF now");
                    if (mNextStep == DISABLE_WIFI) {
                        log("StartActivity: WiFiScanReceiver::OnReceive: Set WiFi to OFF");
                        mNextStep = ENABLE_WIFI;
                        disableWifi();
                    } else if (mNextStep == ENABLE_WIFI) {
                        log("StartActivity: WiFiScanReceiver::OnReceive: Set WiFi to ON");
                        mNextStep = ADD_NETWORK;
                        enableWifi();
                    }
                } else {
                    log("StartActivity: WiFiScanReceiver::OnReceive:2: Unknown Event: " + action + " wifiApState: " + wifiApState);
                }
            } else if (WifiManager.SCAN_RESULTS_AVAILABLE_ACTION.equals(action)) {
                if (mScanCompleted == false) {
                    //Scan result is available, so prepare the wifi ap list
                    log("StartActivity: WiFiScanReceiver::OnReceive: WiFiScan Completed");
                    mScanCompleted = true;
                    setWiFiAPList();
                }
            }
        }
    }
}
