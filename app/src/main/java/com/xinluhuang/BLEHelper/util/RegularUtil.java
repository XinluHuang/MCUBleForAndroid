package com.xinluhuang.BLEHelper.util;

import java.util.regex.Pattern;

public class RegularUtil {
    private RegularUtil() {}
    public static boolean isInteger(String str) {
        return Pattern.compile("^[\\d]+$").matcher(str).matches();
    }
    public static boolean isDate(String str) {
        if( Pattern.compile("^[\\d]{5,6}$").matcher(str).matches()){
            int date=Integer.valueOf(str);
            if (date%100<60 && date%10000<6000 && date<240000){
                return true;
            }
        }
        return false;
    }
}
