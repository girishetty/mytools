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
import java.util.List;

public class ServerWebPageSelectWifi extends ServerWebPage {
    protected StartActivity mActivity;

    ServerWebPageSelectWifi(StartActivity activity) {
        mActivity = activity;
        mPageName = StartActivity.HTML_PATH + "select-wifi.html";
    }

    public void createWebPage() {
        try {
            BufferedWriter webPage = new BufferedWriter(new FileWriter(mPageName));

            webPage.write("<html>");
            webPage.newLine();
            webPage.write("<head>");
            webPage.newLine();
            webPage.write("<title>BG2CD Device Configuration Web Portal</title>");
            webPage.newLine();
            webPage.write("</head>");
            webPage.newLine();
            webPage.write("<body style=\"background-color:blue\">");
            webPage.newLine();
            webPage.write("<font face=\"cursive,serif\" size=\"4\" color=\"#ff9900\">");
            webPage.newLine();
            webPage.write("BG2CD Device Configuration Web Portal");
            webPage.newLine();
            webPage.write("<br>");
            webPage.newLine();
            webPage.write("<br>");
            webPage.newLine();
            webPage.write("<font face=\"cursive,serif\" size=\"4\">");
            webPage.newLine();
            webPage.write("<form action=\"/html/tags/html_form_tag_action.cfm\" method=\"get\">");
            webPage.newLine();
            webPage.write("Available WiFi Access Points:");
            webPage.newLine();
            webPage.write("<select name=\"wifi_ap\">");
            webPage.newLine();

            //Insert all WiFi AP Here
            List<String> wifiAPList = mActivity.getWiFiAPList();
            int count = wifiAPList.size();
            String wifiAP = "";
            for (int index = 0; index < count; index++) {
                wifiAP = wifiAPList.get(index);
                if (!wifiAP.isEmpty()) {
                    webPage.write("  <option name=\"wifi-ap\" value =\"" + wifiAP  + "\">" + wifiAP + "</option>");
                    webPage.newLine();
                }
            }

            webPage.write("</select>");
            webPage.newLine();
            webPage.write("<br />");
            webPage.newLine();
            webPage.write("Password:");
            webPage.newLine();
            webPage.write("<input type=\"password\" name=\"password\" value=\"\" maxlength=\"100\" />  ");
            webPage.newLine();
            webPage.write("<br />");
            webPage.newLine();
            webPage.write("<br />");
            webPage.newLine();
            webPage.write("<input type=\"submit\" value=\"Connect\" />");
            webPage.newLine();
            webPage.write("</form>");
            webPage.newLine();
            webPage.write("</body>");
            webPage.newLine();
            webPage.write("</html>");

            webPage.flush();
            webPage.close();
        } catch (Exception e) {
            Server.send("ServerWebPage: Exception: " + e.getMessage() + "for " + mPageName);
        }
    }
}
