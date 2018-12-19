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
public class MusicFragment extends Fragment {


    public MusicFragment() {
        // Required empty public constructor
    }

    public static MusicFragment newInstance() {
        MusicFragment musicFragment = new MusicFragment();
        Bundle bundle = musicFragment.getArguments();
        if (bundle == null) {
            bundle = new Bundle();
        }
        musicFragment.setArguments(bundle);
        return musicFragment;
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        View view = inflater.inflate(R.layout.fragment_music, container, false);
        view.findViewById(R.id.music_off).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                BlueToothHelper.getInstance().getMcuControl().music(0);
            }
        });
       view.findViewById(R.id.music_pause).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                BlueToothHelper.getInstance().getMcuControl().music(4);
            }
        });

        view.findViewById(R.id.music_1).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                BlueToothHelper.getInstance().getMcuControl().music(1);
            }
        });
        view.findViewById(R.id.music_2).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                BlueToothHelper.getInstance().getMcuControl().music(2);
            }
        });
        view.findViewById(R.id.music_3).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                BlueToothHelper.getInstance().getMcuControl().music(3);
            }
        });
        return view;
    }

}
