package com.example.android2unity;

import android.app.Activity;
import android.app.Service;
import android.os.Build;
import android.os.VibrationEffect;
import android.os.Vibrator;
import android.widget.Toast;

import java.lang.reflect.Field;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

/**
 * Unity Android 震动插件
 * 支持：1. 设备震动支持检测 2. 自定义时长 3. 强度调节（Android 8.0+）4. 预设挡位 5. 停止震动
 */
public class Android2U {
    private Activity mUnityActivity;
    private Vibrator mVibrator;

    /**
     * 反射获取 Unity 当前 Activity
     */
    private Activity getUnityActivity() {
        if (mUnityActivity == null) {
            try {
                // Unity 固定的 Activity 
                Class<?> unityPlayerClass = Class.forName("com.unity3d.player.UnityPlayer");
                // 通过静态字段 currentActivity 获取实例
                Field currentActivityField = unityPlayerClass.getDeclaredField("currentActivity");
                currentActivityField.setAccessible(true); // 允许访问私有字段
                mUnityActivity = (Activity) currentActivityField.get(unityPlayerClass);
            } catch (ClassNotFoundException | NoSuchFieldException | IllegalAccessException e) {
                showToast("获取 Unity 上下文失败：" + e.getMessage());
            }
        }
        return mUnityActivity;
    }

    /**
     * 初始化震动器（延迟加载，避免启动时占用资源）
     */
    private void initVibrator() {
        if (mVibrator == null && getUnityActivity() != null) {
            // 获取 Android 震动服务（Service.VIBRATOR_SERVICE 是系统固定常量）
            mVibrator = (Vibrator) getUnityActivity().getSystemService(Service.VIBRATOR_SERVICE);
        }
    }

    // -------------------------- 对外暴露的震动接口（供 Unity 调用）--------------------------

    /**
     * 【接口1】检查设备是否支持震动（避免无效调用）
     * @return true=支持，false=不支持
     */
    public boolean isVibrationSupported() {
        initVibrator(); // 确保震动器已初始化
        return mVibrator != null && mVibrator.hasVibrator();
    }

    /**
     * 【接口2】基础震动（仅控制时长，兼容所有 Android 版本）
     * @param milliseconds 震动时长（单位：毫秒），范围限制：10~2000ms（避免过短/过长）
     */
    public void OnceVibrate(long milliseconds) {
        if (!isVibrationSupported()) {
            showToast("设备不支持震动");
            return;
        }
        // 限制时长范围，防止异常值（如负数、超长时间）
        long safeTime = Math.max(10, Math.min(milliseconds, 2000));

        // 适配 Android 8.0+（API 26）：旧 API 已废弃，用 VibrationEffect 替代
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            // 创建单次震动效果：时长 + 默认强度（255，最强）
            VibrationEffect effect = VibrationEffect.createOneShot(safeTime, VibrationEffect.DEFAULT_AMPLITUDE);
            mVibrator.vibrate(effect);
        } else {
            // 低版本兼容：直接调用旧 API（虽废弃，但仍能正常工作）
            mVibrator.vibrate(safeTime);
        }
    }

    /**
     * 【接口3】带强度的震动（Android 8.0+ 支持强度调节，低版本忽略强度）
     * @param milliseconds 震动时长（10~2000ms）
     * @param intensity 震动强度（0.0~1.0，0=最弱，1=最强）
     */
    public void VibrateWithIntensity(long milliseconds, float intensity) {
        if (!isVibrationSupported()) {
            showToast("设备不支持震动");
            return;
        }
        // 参数安全校验：时长范围 10~2000ms，强度范围 0.0~1.0
        long safeTime = Math.max(10, Math.min(milliseconds, 2000));
        float safeIntensity = Math.max(0.0f, Math.min(intensity, 1.0f));

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            // 强度转换：Android 原生强度范围是 0~255，需将 0.0~1.0 映射为整数
            int amplitude = Math.round(safeIntensity * 255);
            VibrationEffect effect = VibrationEffect.createOneShot(safeTime, amplitude);
            mVibrator.vibrate(effect);
        } else {
            // 低版本无强度支持：用“时长缩放”模拟强度（强度越高，震动越长）
            long simulatedTime = (long) (safeTime * (0.5f + safeIntensity * 0.5f)); // 50%~100% 时长
            mVibrator.vibrate(simulatedTime);
        }
    }

    /**
     * 【接口4】预设挡位震动（简化调用，适合快速实现多档反馈）
     * @param level 挡位（1=弱震，2=中震，3=强震），其他值提示无效
     */
    public void SettingVibrateByLevel(int level) {
        switch (level) {
            case 1: // 弱震：短时长 + 低强度
                VibrateWithIntensity(150, 0.3f);
                break;
            case 2: // 中震：中等时长 + 中等强度
                VibrateWithIntensity(150, 0.6f);
                break;
            case 3: // 强震：长时长 + 高强度
                VibrateWithIntensity(200, 1.0f);
                break;
            default:
                showToast("无效挡位！请使用 1~3 挡");
                break;
        }
    }

    /**
     * 【接口5】停止当前震动（如需要中断正在进行的长震动）
     */
    public void stopVibration() {
        if (mVibrator != null && mVibrator.hasVibrator()) {
            mVibrator.cancel();
        }
    }

    // -------------------------- 辅助功能（Toast 提示 + Unity 回调）--------------------------

    /**
     * 显示 Toast 提示（需在 UI 线程调用，避免 Android 崩溃）
     * @param content 提示内容
     */
    public void showToast(String content) {
        Activity activity = getUnityActivity();
        if (activity != null) {
            // 用 runOnUiThread 确保在主线程执行 UI 操作
            activity.runOnUiThread(() -> Toast.makeText(activity, content, Toast.LENGTH_SHORT).show());
        }
    }

    /**
     * 调用 Unity 中的方法（向 Unity 回传数据，如震动状态）
     * @param gameObjectName Unity 中接收回调的 GameObject 名称
     * @param methodName GameObject 上挂载脚本的方法名
     * @param param 传递的参数（仅支持字符串，复杂参数需序列化）
     */
    public void callUnityMethod(String gameObjectName, String methodName, String param) {
        try {
            Class<?> unityPlayerClass = Class.forName("com.unity3d.player.UnityPlayer");
            // Unity 固定的方法：UnitySendMessage(GameObject名, 方法名, 参数)
            Method sendMsgMethod = unityPlayerClass.getMethod("UnitySendMessage",
                    String.class, String.class, String.class);
            sendMsgMethod.invoke(unityPlayerClass, gameObjectName, methodName, param);
        } catch (ClassNotFoundException | NoSuchMethodException | IllegalAccessException | InvocationTargetException e) {
            showToast("调用 Unity 方法失败：" + e.getMessage());
        }
    }
}