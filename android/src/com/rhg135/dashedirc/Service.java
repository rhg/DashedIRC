package com.rhg135.dashedirc;

import android.content.Context;
import android.content.Intent;
import android.app.Notification;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import org.qtproject.qt5.android.bindings.QtService;

public class Service extends QtService
{
    @Override
        public int onStartCommand(Intent intent, int flags, int startId) {
            final NotificationChannel notificationChannel = new NotificationChannel("service",
                    "Manager", NotificationManager.IMPORTANCE_DEFAULT);
            getSystemService(NotificationManager.class).createNotificationChannel(notificationChannel);
            final Notification notification = new Notification.Builder(this, "service")
                    .setContentTitle("Running Dashed IRC Service")
                    .build();
            startForeground(1, notification);
            return START_REDELIVER_INTENT;
        }

    public static void startService(Context ctx) {
        ctx.startService(new Intent(ctx, Service.class));
    }
}
