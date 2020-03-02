package ru.kylikovskix.geksa

import android.app.Activity
import android.app.Fragment
import android.content.Context
import android.os.Bundle
import android.preference.DialogPreference
import android.preference.Preference
import android.preference.Preference.OnPreferenceClickListener
import android.preference.PreferenceActivity
import android.preference.PreferenceFragment
import android.util.AttributeSet
import android.view.Gravity
import android.view.ViewGroup
import android.widget.FrameLayout
import android.widget.NumberPicker
import android.widget.SeekBar
import ru.kylikovskix.geksa.FullscreenActivity.Companion.preferences
import ru.kylikovskix.geksa.FullscreenActivity.Companion.savePreference


class SettingsActivity : PreferenceActivity() {

    companion object {

        val DEF_LINEAR_SPEED_VALUE  = 100
        val MAX_LINEAR_SPEED_VALUE  = 500
        val DEF_ROTATE_SPEED_VALUE  = 22
        val MAX_ROTATE_SPEED_VALUE  = 45

        class SettingsFr : PreferenceFragment(), OnPreferenceClickListener {

            var mFrontSpeedKey: String? = null
            var mBackSpeedKey: String? = null
            var mLeftRotateSpeedKey: String? = null
            var mRigthRotateSpeedKey: String? = null

            var mFrontSpeedTitle: String? = null
            var mBackSpeedTitle: String? = null
            var mLeftRotateSpeedTitle: String? = null
            var mRigthRotateSpeedTitle: String? = null

            var mActivity:Activity? = null

            override fun onCreate(savedInstanceState: Bundle?) {
                super.onCreate(savedInstanceState);
                addPreferencesFromResource(R.xml.pref_general);

                findPreference(mFrontSpeedKey).setOnPreferenceClickListener(this)
                findPreference(mBackSpeedKey).setOnPreferenceClickListener(this)
                findPreference(mLeftRotateSpeedKey).setOnPreferenceClickListener(this)
                findPreference(mRigthRotateSpeedKey).setOnPreferenceClickListener(this)
            }

            override fun onAttach(activity: Activity?) {
                super.onAttach(activity)
                mActivity = activity

                mFrontSpeedKey = getString(R.string.front_speed_key)
                mBackSpeedKey = getString(R.string.back_speed_key)
                mLeftRotateSpeedKey = getString(R.string.left_rotate_speed_key)
                mRigthRotateSpeedKey = getString(R.string.rigth_rotate_speed_key)

                mFrontSpeedTitle = getString(R.string.front_speed_title)
                mBackSpeedTitle = getString(R.string.back_speed_title)
                mLeftRotateSpeedTitle = getString(R.string.left_rotate_speed_title)
                mRigthRotateSpeedTitle = getString(R.string.rigth_rotate_speed_title)
            }

            override fun onDetach() {
                mActivity = null
                super.onDetach()
            }

            override fun onPreferenceClick(preference: Preference?): Boolean {
               when(preference?.key) {
                   mFrontSpeedKey -> {
                       if(mFrontSpeedKey != null) {
                           val frontSpeed = preferences.getInt(mFrontSpeedKey, DEF_LINEAR_SPEED_VALUE)
                           mActivity?.openIntValueDialog(
                               R.drawable.front, mFrontSpeedTitle ?: "", frontSpeed, 0, MAX_LINEAR_SPEED_VALUE, {
                                   savePreference(mFrontSpeedKey ?: "", it)
                               })
                           return true
                       }
                    }
                   mBackSpeedKey -> {
                       if(mBackSpeedKey != null) {
                           val backtSpeed = preferences.getInt(mBackSpeedKey, DEF_LINEAR_SPEED_VALUE)
                           mActivity?.openIntValueDialog(R.drawable.back, mBackSpeedTitle ?: "", backtSpeed, 0, MAX_LINEAR_SPEED_VALUE,
                               {
                                   savePreference(mBackSpeedKey ?: "", it)
                               })
                           return true
                       }
                   }
                   mLeftRotateSpeedKey -> {
                       if(mLeftRotateSpeedKey != null) {
                           val leftRotateSpeed = preferences.getInt(mLeftRotateSpeedKey, DEF_ROTATE_SPEED_VALUE)
                           mActivity?.openIntValueDialog(R.drawable.left, mLeftRotateSpeedTitle ?: "", leftRotateSpeed, 0, MAX_ROTATE_SPEED_VALUE,
                               {
                                   savePreference(mLeftRotateSpeedKey ?: "", it)
                               })
                           return true
                       }
                   }
                   mRigthRotateSpeedKey -> {
                       if(mRigthRotateSpeedKey != null) {
                           val leftRotateSpeed = preferences.getInt(mRigthRotateSpeedKey, DEF_ROTATE_SPEED_VALUE)
                           mActivity?.openIntValueDialog(R.drawable.rigth,mRigthRotateSpeedTitle ?: "",leftRotateSpeed,0,MAX_ROTATE_SPEED_VALUE,
                               {
                                   savePreference(mRigthRotateSpeedKey ?: "", it)
                               })
                           return true
                       }
                   }
                }
                return false
            }
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState);
        onCreatePreferenceFragment();
    }

    private fun onCreatePreferenceFragment() {
        getFragmentManager().beginTransaction()
            .replace(android.R.id.content, SettingsFr() as Fragment)
            .commit();
    }
}