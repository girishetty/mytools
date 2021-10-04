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
 * @version $Id: ServerHandler.java 727 2011-01-02 13:04:32Z markus $
 */
package com.android.webserver;

import java.io.*;
import java.net.*;

class ServerHandler extends Thread {
    private BufferedReader mClientReader;
    private Socket mClient;
    private StartActivity mActivity;
	
    private static final String CONFIG_WIFI = "wifi";
    private static final String CONFIG_RESOLUTION = "resolution";
  
    ServerHandler(Socket s, StartActivity activity) {
        mClient = s;
        mActivity = activity;
    }

    //Main Control body of this class
    public void run() {
        String document = "";

        try {
            mClientReader = new BufferedReader(new InputStreamReader(mClient.getInputStream()));

            // Receive data
            while (true) {
                String s = mClientReader.readLine().trim();

                if (s.equals("")) {
                    break;
                }
 
                if (s.substring(0, 3).equals("GET")) {
                    int leerstelle = s.indexOf(" HTTP/");
       	            document = s.substring(5, leerstelle);
                    document = document.replaceAll("[/]+","/");
                }
            }
        }
        catch (Exception e) {
            Server.remove(mClient);
            try {
                mClient.close();
            }
            catch (Exception ex){}
        }
    
        //Log the document name
        Server.send("ServerHandler: " + document);

        // Standard-Doc
        if (document.equals("")) {
            document = "index.html";
        }
	
        // Don't allow directory traversal
        if (document.indexOf("..") != -1) {
            document = "403.html";
        }
	
        //Other document that we may hev received/read from the webservre client
        //parse to see if we have "config" in the URI
        int index = document.indexOf("config=");
        if (index != -1) {
            //we have an URL that includes "?config=CONFIG_TYPE"
            //Extract the CONFIG_TYPE by indexng through "config="
            index += 7;
            String configType = document.substring(index);

            //Log CONFIG_TYPE
            Server.send("ServerHandler: CONFIG_TYPE: " + configType);

            //Prepare the response URI based on CONFIG_TYPE
            if(configType.equalsIgnoreCase(CONFIG_WIFI)) {
                document = createSelectWiFiWebPage();
            }
            else if(configType.equalsIgnoreCase(CONFIG_RESOLUTION)) {
                document = "change-res.html";
           }
        }
        else {
            //parse to see if we have wif_ap and the password
            index = document.indexOf("wifi_ap");
            if (index != -1) {
                //we have an URL that includes "?wifi_ap=AP_NAME&password=PASS"
                //Extract the AP_NAME by indexng through "wifi_ap="
                index += 8;
                String wifiAP = document.substring(index, document.indexOf('&', index));

                //index through "password=" to extract the PASS
                index = document.indexOf("password");
                index += 9;
                String wifiPASS = document.substring(index);

                //Log WiFiAP and PASSWORD
                Server.send("ServerHandler: WiFiAp: " + wifiAP + " PASSWORD: " + wifiPASS);

                //Set the device to connect to the AP
                if(true == connectToWiFiAP(wifiAP, wifiPASS)) {
                    //Prepare the response URI with success
                    document = "status.html";
                }
                else {
                    document = "404.html";
                }
            }
        }

        // Construct the absolute path for the document
        document = "/sdcard/device_config/" + document;
        document = document.replaceAll("[/]+","/");
        if(document.charAt(document.length()-1) == '/') {
            document = "/sdcard/device_config/404.html";
        }
	
        String headerBase = "HTTP/1.1 %code%\n"+
            "Server: MarverllBG2CD/1\n"+
            "Content-Length: %length%\n"+
            "Connection: close\n"+
            "Content-Type: text/html; charset=iso-8859-1\n\n";

        String header = headerBase;
        header = header.replace("%code%", "403 Forbidden");

        Server.send("ServerHandler: " + document);

        try {
            File f = new File(document);
            if (!f.exists()) {
                header = headerBase;
                header = header.replace("%code%", "404 File not found");
                document = "404.html";
            }
        }
        catch (Exception e) {}
    
        if (!document.equals("/sdcard/device_config/403.html")) {
            header = headerBase.replace("%code%", "200 OK");
        }
	
        try {
            File f = new File(document);
            if (f.exists()) {
                BufferedInputStream in = new BufferedInputStream(new FileInputStream(document));
                BufferedOutputStream toClient = new BufferedOutputStream(mClient.getOutputStream());
                ByteArrayOutputStream tempOut = new ByteArrayOutputStream();
    	  
                byte[] buf = new byte[4096];
                int count = 0;
                while ((count = in.read(buf)) != -1){
                    tempOut.write(buf, 0, count);
                }

                tempOut.flush();
                header = header.replace("%length%", ""+tempOut.size());

                toClient.write(header.getBytes());
                toClient.write(tempOut.toByteArray());
                toClient.flush();
            }
            else {
                Server.send(document);
                // Send HTML-File (Ascii, not as a stream)
                header = headerBase;
                header = header.replace("%code%", "404 File not found");	    	  
                header = header.replace("%length%", ""+"404 - File not Found".length());	    	  
                PrintWriter out = new PrintWriter(mClient.getOutputStream(), true);
                out.print(header);
                out.print("404 - File not Found");
                out.flush();
            }

            Server.remove(mClient);
            mClient.close();
        }
        catch (Exception e) { }
    }

    private String createSelectWiFiWebPage() {
        String webPageName = "select-wifi.html";
        ServerWebPage webPage = new ServerWebPageSelectWifi (mActivity);
        webPage.createWebPage();
        return webPageName;
    }

    boolean connectToWiFiAP(String wifiAP, String wifiPASS) {

        boolean success = true; 
        success = mActivity.connectToWiFiAP(wifiAP, wifiPASS);
        Server.send("ServerHandler: connectToWiFiAP returned: " + success);
        return success;
    }
}
