package ru.kylikovskix.geksa

import android.content.Context
import android.preference.DialogPreference
import android.util.AttributeSet

class NumberPickerPreference(context: Context, attrs: AttributeSet) : DialogPreference(context, attrs) {
    init {
        //setDialogLayoutResource(R.layout.numberpicker_dialog);
        setPositiveButtonText(android.R.string.ok);
        setNegativeButtonText(android.R.string.cancel);
        setDialogIcon(null);
    }
}