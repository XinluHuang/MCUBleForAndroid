package com.xinluhuang.BLEHelper.module.main;


import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import com.xinluhuang.BLEHelper.BlueToothHelper;
import com.xinluhuang.BLEHelper.R;

/**
 * A simple {@link Fragment} subclass.
 */
public class MotorFragment extends Fragment {
    private View speed1;
    private View speed2;
    private View speed3;
    private View stop;

    public MotorFragment() {
        // Required empty public constructor
    }

    public static MotorFragment newInstance() {
        MotorFragment motorFragment = new MotorFragment();
        Bundle bundle = motorFragment.getArguments();
        if (bundle == null) {
            bundle = new Bundle();
        }
        motorFragment.setArguments(bundle);
        return motorFragment;
    }


    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        View view = inflater.inflate(R.layout.fragment_motor, container, false);
        speed1 = view.findViewById(R.id.speed_1);
        speed2 = view.findViewById(R.id.speed_2);
        stop = view.findViewById(R.id.speed_stop);
        speed1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                BlueToothHelper.getInstance().getMcuControl().motor(1);
            }
        });
        speed2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                BlueToothHelper.getInstance().getMcuControl().motor(2);
            }
        });
        stop.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                BlueToothHelper.getInstance().getMcuControl().motor(0);
            }
        });
        return view;
    }

}
