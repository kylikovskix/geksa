package ru.kylikovskix.geksa

import android.content.DialogInterface
import android.content.Intent
import android.content.SharedPreferences
import android.os.AsyncTask
import android.os.Bundle
import android.preference.Preference.OnPreferenceClickListener
import android.preference.PreferenceManager
import android.util.DisplayMetrics
import android.view.Menu
import android.view.MenuItem
import android.view.View
import android.widget.*
import androidx.appcompat.app.AlertDialog
import androidx.appcompat.app.AppCompatActivity
import kotlinx.android.synthetic.main.activity_fullscreen.*
import kotlinx.android.synthetic.main.int_value_input_.*
import org.json.JSONObject
import ru.kylikovskix.geksa.SettingsActivity.Companion.DEF_LINEAR_SPEED_VALUE
import ru.kylikovskix.geksa.SettingsActivity.Companion.DEF_ROTATE_SPEED_VALUE
import java.util.*
import kotlin.collections.ArrayList


class FullscreenActivity : AppCompatActivity(), TCPClient.OnMessageListener {

    var is_power = false;
    var mode = 0; //  0 - static, 1 - dynamic
    var client: TCPClient? = null
    var timer: Timer? = null
    val buttons = ArrayList<Button>()

    val LEFT_FRONT_FOOT_IDX     = 0
    val LEFT_MIDLE_FOOT_IDX     = 1
    val LEFT_BACK_FOOT_IDX      = 2
    val RIGTH_FRONT_FOOT_IDX    = 3
    val RIGTH_MIDLE_FOOT_IDX    = 4
    val RIGTH_BACK_FOOT_IDX     = 5
    val FOOTS_COUNT = 6

    companion object {
        lateinit var preferences: SharedPreferences
            private set

        fun savePreference(key: String, value: Boolean) {
            val e = preferences.edit()
            e.putBoolean(key, value)
            e.apply()
        }

        fun savePreference(key: String, value: String) {
            val e = preferences.edit()
            e.putString(key, value)
            e.apply()
        }

        fun savePreference(key: String, value: Int) {
            val e = preferences.edit()
            e.putInt(key, value)
            e.apply()
        }

        fun savePreference(key: String, value: Long) {
            val e = preferences.edit()
            e.putLong(key, value)
            e.apply()
        }
    }

    fun connect() {
        disconnect()
        connectTask().execute()
    }

    fun disconnect() {
        client?.stop()
        client = null
        updateView()
    }

    fun sendRequestInfo() {
        if (isConnected())
            client?.sendMessage("pose;")
    }

    fun isConnected(): Boolean {
        return true
        return (client != null) && (client?.isConnected() ?: false)
    }

    fun setMargin(view: View, left: Int, top: Int, rigth: Int, bottom: Int) {
        val layoutParams = view.getLayoutParams() as FrameLayout.LayoutParams
        layoutParams.leftMargin = left
        layoutParams.topMargin = top
        layoutParams.rightMargin = rigth
        layoutParams.bottomMargin = bottom
        view.setLayoutParams(layoutParams)
    }

    fun updateView() {
        val display = windowManager.defaultDisplay
        val metricsB = DisplayMetrics()
        display.getMetrics(metricsB)
        val step = Math.min(metricsB.widthPixels, metricsB.heightPixels) / 8

        is_power = isConnected() && is_power
        val backgroundResource =
            if (is_power) R.drawable.button_background_on else R.drawable.button_background_off

        for(i in 0 .. buttons.size-1)
            buttons[i].setBackgroundResource(backgroundResource)

        setMargin(btnLeftFrontFootCoxa, 2 * step, 2 * step, 0, 0)
        setMargin(btnLeftFrontFootFemora, step, step, 0, 0)

        setMargin(btnLeftMidleFootCoxa, 2 * step, 0, 0, 0)
        setMargin(btnLeftMidleFootFemora, step, 0, 0, 0)

        setMargin(btnLeftBackFootCoxa, 2 * step, 0, 0, 2 * step)
        setMargin(btnLeftBackFootFemora, step, 0, 0, step)

        setMargin(btnRigthFrontFootCoxa, 0, 2 * step, 2 * step, 0)
        setMargin(btnRigthFrontFootFemora, 0, step, step, 0)

        setMargin(btnRigthMidleFootCoxa, 0, 0, 2 * step, 0)
        setMargin(btnRigthMidleFootFemora, 0, 0, step, 0)

        setMargin(btnRigthBackFootCoxa, 0, 0, 2 * step, 2 * step)
        setMargin(btnRigthBackFootFemora, 0, 0, step, step)

        btnPower.setImageResource(if (is_power) R.drawable.power_on else R.drawable.power_off)
        btnConnect.setImageResource(if (isConnected()) R.drawable.connect_on else R.drawable.connect_off)
        btnMode.setImageResource(if (mode == 0) R.drawable.static_mode else R.drawable.dinamic_mode)

        gvMove.visibility = if (mode == 0 || !is_power) View.GONE else View.VISIBLE
        btnPower.visibility = if (isConnected()) View.VISIBLE else View.GONE

        btnMode.visibility = if (is_power) View.VISIBLE else View.GONE
    }

    override fun onCreateOptionsMenu(menu: Menu): Boolean {
        menuInflater.inflate(R.menu.main, menu)
        return true
    }

    override fun onOptionsItemSelected(item: MenuItem): Boolean {
        return when (item.itemId) {
            R.id.settings_action -> {
                val intent = Intent(this, SettingsActivity::class.java)
                startActivity(intent);
                true;
            }
            else -> super.onOptionsItemSelected(item)
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        setContentView(R.layout.activity_fullscreen)
        supportActionBar?.setDisplayHomeAsUpEnabled(true)

        buttons.clear()
        buttons.add(btnLeftFrontFootCoxa)
        buttons.add(btnLeftMidleFootCoxa)
        buttons.add(btnLeftBackFootCoxa)
        buttons.add(btnRigthFrontFootCoxa)
        buttons.add(btnRigthMidleFootCoxa)
        buttons.add(btnRigthBackFootCoxa)
        buttons.add(btnLeftFrontFootFemora)
        buttons.add(btnLeftMidleFootFemora)
        buttons.add(btnLeftBackFootFemora)
        buttons.add(btnRigthFrontFootFemora)
        buttons.add(btnRigthMidleFootFemora)
        buttons.add(btnRigthBackFootFemora)
        buttons.add(btnLeftFrontFootTibia)
        buttons.add(btnLeftMidleFootTibia)
        buttons.add(btnLeftBackFootTibia)
        buttons.add(btnRigthFrontFootTibia)
        buttons.add(btnRigthMidleFootTibia)
        buttons.add(btnRigthBackFootTibia)

        // настройи приложения
        preferences = PreferenceManager.getDefaultSharedPreferences(this)

        btnMode.setOnClickListener({
            mode = if(mode == 0) 1 else 0
            updateView()
        })

        btnConnect.setOnClickListener({
            if (isConnected()) {
                disconnect()
            } else if (client == null) {
                connect()
            }
        })

        btnPower.setOnClickListener({
            is_power = !is_power
            updateView()
        })

        btnMoveFront.setOnClickListener({
            val speed = preferences.getInt(getString(R.string.front_speed_key), DEF_LINEAR_SPEED_VALUE)
            client?.sendMessage("move %d;".format(speed))
        })

        btnMoveBack.setOnClickListener({
            val speed = preferences.getInt(getString(R.string.back_speed_key), DEF_LINEAR_SPEED_VALUE)
            client?.sendMessage("move -%d;".format(speed))
        })

        btnMoveLeft.setOnClickListener({
            val speed = preferences.getInt(getString(R.string.left_rotate_speed_key), DEF_ROTATE_SPEED_VALUE)
            client?.sendMessage("rotate -%d;".format(speed))
        })

        btnMoveRigth.setOnClickListener({
            val speed = preferences.getInt(getString(R.string.rigth_rotate_speed_key), DEF_ROTATE_SPEED_VALUE)
            client?.sendMessage("rotate %d;".format(speed))
        })

        btnStop.setOnClickListener({
            client?.sendMessage("stop;")
        })

        val MIN_ANGLE_VALUE = -90
        val MAX_ANGLE_VALUE = 90

        for(i in 0..buttons.size-1)
            buttons[i].setOnClickListener({
                val b = (it as Button)
                val f = i % 6
                val a = i / 6
                if(mode == 0)
                    openIntValueDialog(0, getString(R.string.input_angle_value_text), b.text.toString().toInt(), MIN_ANGLE_VALUE, MAX_ANGLE_VALUE,  {
                        when(a) {
                            0 ->    client?.sendMessage("coxa %d %d;".format(f, it))
                            1 ->    client?.sendMessage("femora %d %d;".format(f, it))
                            else -> client?.sendMessage("tibia %d %d;".format(f, it))
                        }
                })
        })

    }

    override fun onResume() {
        super.onResume()
        timer = startTimer({
            sendRequestInfo()
        }, 300)
        updateView()
    }

    override fun onDestroy() {
        client?.stop()
        super.onDestroy()
    }

    override fun msgError(message: String) {
        Toast.makeText(this, "connect erro: " + message, Toast.LENGTH_SHORT).show()
    }

    override fun msgReceived(message: String) {
        var msg = message.trim();
        if(msg.indexOf("json:{", ignoreCase = true) == 0) {
            val jsonObject = JSONObject(msg.substring(5,msg.length))
            if(jsonObject.has("POSE")) {
                val jsonArray = jsonObject.getJSONArray("POSE")
                for(i in 0..jsonArray.length()-1) {
                    val angles = jsonArray.getJSONArray(i)
                    for (j in 0..2)
                        buttons[j * FOOTS_COUNT + i].setText(angles[j].toString())
                }
            }
        }

    }

    inner class connectTask : AsyncTask<Void, String, Void?>() {

        override fun doInBackground(vararg arg: Void): Void? {
            client?.run()
            return null
        }

        override fun onProgressUpdate(vararg values: String) {
            super.onProgressUpdate(*values)
            if(values[0][0] == 'r')
                this@FullscreenActivity.msgReceived(values[0].substring(2))
            else if(values[0][0] == 'e')
                this@FullscreenActivity.msgError(values[0].substring(2))

            updateView()
        }

        override fun onPostExecute(result: Void?) {
            super.onPostExecute(result)
            disconnect()
        }

        override fun onPreExecute() {
            super.onPreExecute()
            val host_addr = preferences.getString(getString(R.string.host_address_key), "192.168.1.29:2000")
            this@FullscreenActivity.toast("connect to " + host_addr)
            val host_addr2 = host_addr?.split(":")
            client = TCPClient(host_addr2?.get(0) ?: "", host_addr2?.get(1)?.toInt() ?: 2000, object : TCPClient.OnMessageListener {

                override fun msgReceived(message: String) {
                    publishProgress("r: " + message)
                }

                override fun msgError(message: String) {
                    publishProgress("e: " + message)
                }
            })
            updateView()
        }
    }

}
