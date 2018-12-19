package com.xinluhuang.BLEHelper.module.main;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothGatt;
import android.bluetooth.BluetoothGattCharacteristic;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.v7.app.AppCompatActivity;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import com.xinluhuang.BLEHelper.R;
import com.xinluhuang.BLEHelper.BlueToothHelper;
import com.xinluhuang.BLEHelper.module.SearchActivity;
import com.xinluhuang.BLEHelper.util.LogUtil;
import com.xinluhuang.BLEHelper.util.RegularUtil;

import butterknife.BindView;
import butterknife.ButterKnife;

public class MainActivity extends AppCompatActivity {
    private static final int RECEIVE = 0;
    private static final int CONNECTED = 1;
    private static final int DISCONNECTED = 2;
    private static final int REQUEST_ENABLE_BT = 3;
    private static final int ADDRESS_REQUEST = 4;
    public static final int ADDRESS_RESULT = 5;

    private BluetoothAdapter mBluetoothAdapter;
    BlueToothHelper blueToothHelper;


    @BindView(R.id.tv)
    TextView textView;
    @BindView(R.id.temperature)
    TextView temperature;
    @BindView(R.id.humidity)
    TextView humidity;


    private BroadcastReceiver broadcastReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            int state = intent.getIntExtra(BluetoothAdapter.EXTRA_STATE, BluetoothAdapter.STATE_OFF);
            if (state == BluetoothAdapter.STATE_ON) {
                init_ble();
            }
        }
    };

    private Handler handler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {
                case CONNECTED:
                    getSupportActionBar().setTitle("已连接");
                    break;
                case DISCONNECTED:
                    getSupportActionBar().setTitle("已断开");
                    break;
                case RECEIVE:
                    byte[] bs = (byte[]) msg.obj;
                    if (bs == null || bs[0]!=bs[bs.length-1] ||bs.length<3) return;
                    switch (bs[0]) {
                        case '\0':
                            int hum=bs[1];
                            humidity.setText(""+hum);
                            LogUtil.d("湿度"+hum);
                            break;
                        case '\1':
                            int temp=bs[1];
                            temperature.setText(""+temp);
                            LogUtil.d("温度"+temp);
                            break;
                        default:
                            textView.append(new String(bs));
                            break;
                    }


                    break;
                default:
                    break;

            }
        }
    };


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        ButterKnife.bind(this);

        getSupportActionBar().setTitle("未连接");

        IntentFilter intentFilter = new IntentFilter();
        intentFilter.addAction(BluetoothAdapter.ACTION_STATE_CHANGED);
        registerReceiver(broadcastReceiver, intentFilter);

        init_ble();

        blueToothHelper.setBlueToothListener(new BlueToothHelper.BlueToothListener() {
            @Override
            public void read(byte[] value) {
                handler.sendMessage(Message.obtain(handler, RECEIVE, value));

            }

            @Override
            public void connected() {
                handler.sendMessage(Message.obtain(handler, CONNECTED));
            }

            @Override
            public void disconnected() {
                handler.sendMessage(Message.obtain(handler, DISCONNECTED));
            }
        });


        getSupportFragmentManager().beginTransaction()
                .replace(R.id.container, GeneralFragment.newInstance())
                .commit();
    }


    private void init_ble() {
        if (!getPackageManager().hasSystemFeature(
                PackageManager.FEATURE_BLUETOOTH_LE)) {
            Toast.makeText(this, "不支持BLE", Toast.LENGTH_SHORT).show();
            finish();
        }
        //蓝牙权限是普通权限，不需要动态申请
        mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        if (mBluetoothAdapter == null || !mBluetoothAdapter.isEnabled()) {
            Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableBtIntent, REQUEST_ENABLE_BT);
        }

        blueToothHelper = BlueToothHelper.getInstance();
        blueToothHelper.connectDevice();

    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.main_menu, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
            case R.id.search_item:
                blueToothHelper.disconnect();
                startActivityForResult(new Intent(this, SearchActivity.class), ADDRESS_REQUEST);
                break;
            case R.id.dht_item:
                blueToothHelper.getMcuControl().dht();
                break;
        }
        return true;
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (ADDRESS_RESULT == resultCode) {
            BlueToothHelper.getInstance().connectDevice();
        }
        super.onActivityResult(requestCode, resultCode, data);
    }

    @Override
    protected void onDestroy() {
        unregisterReceiver(broadcastReceiver);
        blueToothHelper.disconnect();
        super.onDestroy();
    }
}
