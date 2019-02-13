package com.rhg135.dashedirc;

import android.content.Context;
import android.content.Intent;
import org.qtproject.qt5.android.bindings.QtService;

public class Service extends QtService
{
    public static void startService(Context ctx) {
        ctx.startService(new Intent(ctx, Service.class));
    }
}
