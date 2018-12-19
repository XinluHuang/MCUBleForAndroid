package com.xinluhuang.BLEHelper.util;

import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;

public class DateUtil {
    public static final String DATE = "HHmmss";
    public static int getNow() {
        return Integer.valueOf(new SimpleDateFormat(DATE).format(new Date()));
    }
    private DateUtil() {
    }
}
