package ru.kylikovskix.geksa

import android.util.Log
import java.io.*
import java.net.Socket
import java.net.InetAddress

class TCPClient(val addr:String, val port:Int, val listener: OnMessageListener) {

    interface OnMessageListener {
        fun msgError(message: String)
        fun msgReceived(message: String)
    }

    companion object {
        val TAG = "TCPClient"
    }
    // message to send to the server
    private var mServerMessage: String? = null
    // sends message received notifications
    private var mRun = false
    // used to send messages
    private var mBufferOut: PrintWriter? = null
    // used to read messages from the server
    private var mBufferIn: BufferedReader? = null
    private var socket:Socket? = null

    fun isConnected():Boolean {
        return socket?.isConnected ?: false
    }

    fun isConnectWait():Boolean {
        return socket != null
    }
    /**
     * Sends the message entered by client to the server
     *
     * @param message text entered by client
     */
    fun sendMessage(message: String) {
        val runnable = Runnable {
            if (mBufferOut != null) {
                Log.d(TAG, "Sending: " + message);
                //mMessageListener?.msgInformation(message)
                mBufferOut!!.println(message)
                mBufferOut!!.flush()
            }
        }
        val thread = Thread(runnable)
        thread.start()
    }

    /**
     * Close the connection and release the members
     */
    fun stop() {

        mRun = false

        if (mBufferOut != null) {
            mBufferOut!!.flush()
            mBufferOut!!.close()
        }

        mBufferIn = null
        mBufferOut = null
        mServerMessage = null
    }

    fun run() {

        try {
            //here you must put your computer's IP address.
            val serverAddr = InetAddress.getByName(addr)

            Log.d("TCP Client", "C: Connecting...");

            //create a socket to make the connection with the server
            socket = Socket(serverAddr, port)
            mRun = true

            try {

                //sends the message to the server
                mBufferOut = PrintWriter(BufferedWriter(OutputStreamWriter(socket?.getOutputStream())), true)

                //receives the message which the server sends back
                mBufferIn = BufferedReader(InputStreamReader(socket?.getInputStream()))


                //in this while the client listens for the messages sent by the server
                while (mRun) {

                    mServerMessage = mBufferIn!!.readLine()

                    if (mServerMessage != null) {
                        //call the method messageReceived from MyActivity class
                        listener.msgReceived(mServerMessage!!)
                    }

                }
                Log.d("RESPONSE FROM SERVER", "S: Received Message: '" + mServerMessage + "'");

            } catch (e: Exception) {
                Log.e("TCP", "S: Error", e);
                listener.msgError(e.localizedMessage)
            } finally {
                //the socket must be closed. It is not possible to reconnect to this socket
                // after it is closed, which means a new socket instance has to be created.
                socket?.close()
                socket = null
            }

        } catch (e: Exception) {
            Log.e("TCP", "C: Error", e);
            listener.msgError(e.localizedMessage)
        }
        mRun = false
    }
}