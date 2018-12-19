package com.xinluhuang.BLEHelper;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothGatt;
import android.bluetooth.BluetoothGattCallback;
import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothGattDescriptor;
import android.bluetooth.BluetoothGattService;
import android.bluetooth.BluetoothProfile;

import com.xinluhuang.BLEHelper.application.BTApplication;
import com.xinluhuang.BLEHelper.util.Constants;
import com.xinluhuang.BLEHelper.util.LogUtil;
import com.xinluhuang.BLEHelper.util.SPUtil;

import java.util.List;
import java.util.UUID;

public class BlueToothHelper {
    private final MyBluetoothGattCallback callback;
    private BluetoothAdapter mBluetoothAdapter;
    private BluetoothDevice device;
    private boolean isConnect;
    private BluetoothGattCharacteristic characteristic;
    private BluetoothGatt gatt;
    private BlueToothListener listener;
    private MCUControl mcuControl;

    private BlueToothHelper() {
        mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        callback = new MyBluetoothGattCallback();
        mcuControl = new MCUControl();
    }

    public void connectDevice() {
        //蓝牙地址保存在sharedPreferences中
        device = mBluetoothAdapter.getRemoteDevice((String) SPUtil.get(SPUtil.DEVICE_ADD, Constants.HC08_ADDRESS));
        if (device != null) {
            gatt = device.connectGatt(BTApplication.getContext(), true, callback);
        }
    }

    public void disconnect() {
        if (gatt != null) {
            gatt.disconnect();
            gatt.close();
        }
    }

    public void write(String content) {
        if (characteristic != null) {
            characteristic.setValue(content);
            gatt.writeCharacteristic(characteristic);
        }
    }

    public void write(byte[] value) {
        if (characteristic != null) {
            characteristic.setValue(value);
            gatt.writeCharacteristic(characteristic);
        }
    }

    public void setBlueToothListener(BlueToothListener blueToothListener) {
        listener = blueToothListener;
    }


    public MCUControl getMcuControl() {
        return mcuControl;
    }

    class MyBluetoothGattCallback extends BluetoothGattCallback {
        @Override
        public void onConnectionStateChange(BluetoothGatt gatt, int status, int newState) {
            if (newState == BluetoothProfile.STATE_CONNECTED) {
                gatt.discoverServices();//打开服务，完毕后会调用onServicesDiscovered，有延迟
                super.onConnectionStateChange(gatt, status, newState);
            } else if (newState == BluetoothProfile.STATE_DISCONNECTED) {
                if (listener != null) {
                    listener.disconnected();
                }
            }
        }

        @Override
        public void onCharacteristicChanged(BluetoothGatt gatt, BluetoothGattCharacteristic characteristic) {
            super.onCharacteristicChanged(gatt, characteristic);
            if(listener!=null){
                listener.read(characteristic.getValue());
            }

        }

        @Override
        public void onServicesDiscovered(BluetoothGatt gatt, int status) {
            if (status == BluetoothGatt.GATT_SUCCESS) {
                isConnect = true;
                List<BluetoothGattService> services = gatt.getServices();
                for (BluetoothGattService service : services) {
                    for (BluetoothGattCharacteristic bluetoothGattCharacteristic : service.getCharacteristics()) {
                        String uuid = bluetoothGattCharacteristic.getUuid().toString();
                        if (Constants.HEART_RATE_MEASUREMENT.equals(uuid)) {
                            if (listener != null) {
                                listener.connected();
                            }
                            characteristic = bluetoothGattCharacteristic;
                            setCharacteristicNotification(gatt, bluetoothGattCharacteristic, true);
                        }
                    }
                }
            }
            super.onServicesDiscovered(gatt, status);
        }

        //接收消息设置
        public void setCharacteristicNotification(BluetoothGatt gatt, BluetoothGattCharacteristic characteristic, boolean enabled) {
            if (gatt == null) {
                return;
            }
            gatt.setCharacteristicNotification(characteristic, enabled);
            BluetoothGattDescriptor clientConfig = characteristic.getDescriptor(UUID.fromString(Constants.DESCRIPTOR_UUID));
            if (enabled) {
                clientConfig.setValue(BluetoothGattDescriptor.ENABLE_NOTIFICATION_VALUE);
            } else {
                clientConfig.setValue(BluetoothGattDescriptor.DISABLE_NOTIFICATION_VALUE);
            }
            gatt.writeDescriptor(clientConfig);
        }
    }



    public class MCUControl {
        private MCUControl() {
        }

        public void light() {
            write("" + '\1' + '\1');
        }

        public void setTime(int num) {
            write("" + '\0' + num + '\0');
        }

        public void motor(int speed) {
            write("" + '\2' + speed + '\2');
        }

        public void music(int index) {
            write("" + '\3' + index + '\3');
        }

        public void dht() {
            write("" + '\4' + '\4');
        }

    }

    public interface BlueToothListener {
        void read(byte[] value);

        void connected();

        void disconnected();
    }

    public static BlueToothHelper getInstance() {
        return Nested.blueToothHelper;
    }

    private static class Nested {
        private static BlueToothHelper blueToothHelper = new BlueToothHelper();
    }
}
