package com.rhg135.dashedirc;

import android.content.Context;
import android.util.TypedValue;

public final class Colors {
    public static String getColor(Context ctx, int attr) {
        TypedValue typedValue = new TypedValue();
        ctx.getTheme().resolveAttribute(attr, typedValue, true);
        return String.format("#%06X", 0xFFFFFF & typedValue.data);
    }
    public static String getAccent(Context ctx) {
        return getColor(ctx, android.R.attr.colorAccent);
    }
}
