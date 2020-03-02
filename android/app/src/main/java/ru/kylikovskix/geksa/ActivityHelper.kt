package ru.kylikovskix.geksa

import android.app.Activity
import android.content.DialogInterface
import android.content.pm.PackageManager
import android.view.LayoutInflater
import android.widget.ImageView
import android.widget.SeekBar
import android.widget.TextView
import android.widget.Toast
import androidx.appcompat.app.AlertDialog
import androidx.appcompat.app.AppCompatActivity
import androidx.core.app.ActivityCompat
import androidx.core.content.ContextCompat
import java.util.*

fun AppCompatActivity.toast(msg:String) {
    Toast.makeText(this, msg, Toast.LENGTH_SHORT).show()
}

fun AppCompatActivity.toast(resId:Int) {
    Toast.makeText(this, resId, Toast.LENGTH_SHORT).show()
}

fun AppCompatActivity.openDialog(title:String, positiveText:String, negativeText:String, onPositiveButtonClick:()->Any, onNegativeButtonClick:()->Any) {
    val quitDialog: AlertDialog.Builder = AlertDialog.Builder(
        this
    )
    quitDialog.setTitle(title)
    quitDialog.setPositiveButton(positiveText, object : DialogInterface.OnClickListener {
        override  fun onClick(
            dialog: DialogInterface?,
            which: Int
        ) { onPositiveButtonClick() }
    })
    quitDialog.setNegativeButton(negativeText, object : DialogInterface.OnClickListener {
        override fun onClick(
            dialog: DialogInterface?,
            which: Int
        ) { onNegativeButtonClick() }
    })
    quitDialog.show()
}

// запустить автообновленеи формы
fun AppCompatActivity.startTimer(functer:(counter:Long)->Any, period:Long = 1000L, _timer: Timer? = null): Timer {
    var _counter = 0L
    val timer = if(_timer == null) Timer() else _timer

    timer.schedule(object : TimerTask() {
        override fun run() {
            runOnUiThread(object : Runnable {
                override fun run() {
                    _counter++
                    functer(_counter)
                }
            });
        }
    }, 0, period)
    return timer
}

// запросить требуемы разрешения
fun AppCompatActivity.checkPermissons(permissons:Array<String>, code:Int):Boolean {

    for(permisson in permissons) {
        if (ContextCompat.checkSelfPermission(
                this,
                permisson
            ) != PackageManager.PERMISSION_GRANTED
        ) {
            ActivityCompat.requestPermissions(this, permissons, code)
            return false;
        }
    }
    return true;
}

fun Activity.openIntValueDialog(resImage:Int, title: String, value: Int, minValue: Int, maxValue: Int, positiveFunc: (value:Int) -> Any?)    {
    val quitDialog: AlertDialog.Builder = AlertDialog.Builder(this)
    quitDialog.setTitle(title)

    val view = LayoutInflater.from(this).inflate(R.layout.int_value_input_, null)
    quitDialog.setView(view)

    val textView = view.findViewById<TextView>(R.id.textView)
    textView.text = value.toString()

    val imageView  = view.findViewById<ImageView>(R.id.imageView)
    imageView.setImageResource(resImage)

    val seekBar = view.findViewById<SeekBar>(R.id.seekBar)
    seekBar.max = maxValue - minValue
    seekBar.progress = value - minValue
    seekBar.setOnSeekBarChangeListener(object: SeekBar.OnSeekBarChangeListener {
        override fun onProgressChanged(seekBar: SeekBar?, progress: Int, fromUser: Boolean) {
            textView.text = (minValue + progress).toString()
        }

        override fun onStartTrackingTouch(seekBar: SeekBar?) {
        }

        override fun onStopTrackingTouch(seekBar: SeekBar?) {
        }

    })

    quitDialog.setPositiveButton(R.string.ok_text, object : DialogInterface.OnClickListener {
        override  fun onClick(
            dialog: DialogInterface?,
            which: Int
        ) { positiveFunc(textView.text.toString().toInt()) }
    })

    quitDialog.setNegativeButton(R.string.cancel_text, object : DialogInterface.OnClickListener {
        override fun onClick(
            dialog: DialogInterface?,
            which: Int
        ) { }
    })
    quitDialog.show()
}
