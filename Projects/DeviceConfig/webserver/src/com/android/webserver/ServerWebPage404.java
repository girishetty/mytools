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

public class ServerWebPage404 extends ServerWebPage {

    ServerWebPage404() {
        mPageName = StartActivity.HTML_PATH + "404.html";
    }

    public void createWebPage() {
        try {
            File file = new File(mPageName); 
            if (!file.exists()) {
                //404.html is not there, so create one
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
                webPage.write("404 - Page not found");
                webPage.newLine();
                webPage.write("<br>");
                webPage.newLine();
                webPage.write("</body>");
                webPage.newLine();
                webPage.write("</html>");

                webPage.flush();
                webPage.close();
            }
        } catch (Exception e) {
            Server.send("ServerWebPage: Exception: " + e.getMessage() + "for " + mPageName);
        }
    }
}
