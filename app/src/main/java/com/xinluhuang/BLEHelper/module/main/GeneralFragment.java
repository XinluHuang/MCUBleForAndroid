package com.xinluhuang.BLEHelper.module.main;


import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import com.xinluhuang.BLEHelper.R;
import com.xinluhuang.BLEHelper.BlueToothHelper;

/**
 * A simple {@link Fragment} subclass.
 */
public class GeneralFragment extends Fragment {
    public GeneralFragment() {
        // Required empty public constructor
    }

    public static GeneralFragment newInstance() {
        GeneralFragment generalFragment = new GeneralFragment();
        Bundle bundle = generalFragment.getArguments();
        if (bundle == null) {
            bundle = new Bundle();
        }
        generalFragment.setArguments(bundle);
        return generalFragment;
    }


    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        View view = inflater.inflate(R.layout.fragment_general, container, false);
        view.findViewById(R.id.light).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                BlueToothHelper.getInstance().getMcuControl().light();
            }
        });
        view.findViewById(R.id.motor).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                getFragmentManager().beginTransaction()
                        .replace(R.id.container, MotorFragment.newInstance())
                        .addToBackStack(null)//点击后退即可恢复
                        .commit();
            }
        });
        view.findViewById(R.id.time).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                getFragmentManager().beginTransaction()
                        .replace(R.id.container, TimeFragment.newInstance())
                        .addToBackStack(null)
                        .commit();
            }
        });
        view.findViewById(R.id.music).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                getFragmentManager().beginTransaction()
                        .replace(R.id.container, MusicFragment.newInstance())
                        .addToBackStack(null)
                        .commit();
            }
        });


        return view;
    }

}
