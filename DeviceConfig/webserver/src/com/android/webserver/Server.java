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
 * @version $Id: Server.java 727 2011-01-02 13:04:32Z markus $
 */

package com.android.webserver;

import java.io.IOException;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.LinkedList;

import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.content.Context;

public class Server extends Thread {
    private ServerSocket mListener = null;
    private static Handler mHandler;
    private StartActivity mActivity;
    private boolean mIsRunning = true;
	
    public static LinkedList<Socket> clientList = new LinkedList<Socket>();
	
    public Server(String ip, int port, Handler handler, StartActivity activity) throws IOException {
        super();
        mHandler = handler;
        mActivity = activity;
        InetAddress ipadr = InetAddress.getByName(ip);
        mListener = new ServerSocket(port, 0, ipadr);
        //mListened.setReuseAddress(true);
    }
    
    public synchronized static void send(String s) {
        Message msg = new Message();
        Bundle b = new Bundle();
        b.putString("msg", s);
        msg.setData(b);
        mHandler.sendMessage(msg);
    }
    
    @Override
    public void run() {
        while( mIsRunning ) {
            try {
                send("Server: Waiting for connections.");
                Socket client = mListener.accept();
                send("Server: New connection from " + client.getInetAddress().toString());
                new ServerHandler(client, mActivity).start();
                clientList.add(client);
            } catch (IOException e) {
                send(e.getMessage());
            }
        }
    }

    public void stopServer() {
        mIsRunning = false;
        try {
            mListener.close();
        } catch (IOException e) {
            send(e.getMessage());
        }
    }

    public synchronized static void remove(Socket s) {
        send("Server: Closing connection: " + s.getInetAddress().toString());
        clientList.remove(s);      
    }
}
