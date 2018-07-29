package com.tapc.ndk;

import android.app.Activity;
import android.app.Instrumentation;
import android.os.Bundle;
import android.view.KeyEvent;
import android.view.View;
import android.widget.Button;

import com.tapc.platform.jni.CommonDriver;
import com.tapc.platform.jni.Driver;

public class MainActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
//        final Driver drive = new Driver();
//        drive.initCom(Driver.UART_DEVICE_NAME, 9600);
//        drive.openUinput(Driver.UINPUT_DEVICE_NAME);
//        drive.closeUinput();
        CommonDriver driver = new CommonDriver();
        driver.open(Driver.UINPUT_DEVICE_NAME);
        CommonDriver driver2 = new CommonDriver();
        driver2.open(Driver.UART_DEVICE_NAME);
        byte[] ss = new byte[2];
        int r = driver2.write(null, 0);
        ss = driver2.read(10);
        Button button = (Button) findViewById(R.id.button);
        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
//                drive.homeEvent();
//                Intent intent = new Intent(Intent.ACTION_MAIN);
//                intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_CLEAR_TOP);// 注意
//                intent.addCategory(Intent.CATEGORY_HOME);
//                startActivity(intent);
                new Thread() {
                    public void run() {
                        try {
                            Instrumentation inst = new Instrumentation();
                            inst.sendKeyDownUpSync(KeyEvent.KEYCODE_BACK);
                        } catch (Exception e) {
                            e.printStackTrace();
                        }
                    }
                }.start();
            }
        });
    }
}
