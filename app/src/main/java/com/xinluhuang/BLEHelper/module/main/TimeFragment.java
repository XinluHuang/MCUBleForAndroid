package com.xinluhuang.BLEHelper.module.main;


import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.EditText;
import android.widget.Toast;

import com.xinluhuang.BLEHelper.BlueToothHelper;
import com.xinluhuang.BLEHelper.R;
import com.xinluhuang.BLEHelper.util.DateUtil;
import com.xinluhuang.BLEHelper.util.RegularUtil;

/**
 * A simple {@link Fragment} subclass.
 */
public class TimeFragment extends Fragment {
    private View syn;
    private View setting;
    private EditText timeInput;

    public TimeFragment() {
        // Required empty public constructor
    }

    public static TimeFragment newInstance() {
        TimeFragment timeFragment = new TimeFragment();
        Bundle bundle = timeFragment.getArguments();
        if (bundle == null) {
            bundle = new Bundle();
        }
        timeFragment.setArguments(bundle);
        return timeFragment;
    }


    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        View view = inflater.inflate(R.layout.fragment_time, container, false);
        syn = view.findViewById(R.id.syn);
        timeInput=view.findViewById(R.id.time_input);
        syn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                BlueToothHelper.getInstance().getMcuControl().setTime(DateUtil.getNow());
            }
        });

        setting = view.findViewById(R.id.time_setting);
        setting.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String str=timeInput.getText().toString();
                if(RegularUtil.isDate(str)) {
                    BlueToothHelper.getInstance().getMcuControl().setTime(Integer.valueOf(str));
                }else {
                    Toast.makeText(getActivity(),"输入的格式不对",Toast.LENGTH_SHORT).show();
                }
            }
        });
        return view;
    }

}
