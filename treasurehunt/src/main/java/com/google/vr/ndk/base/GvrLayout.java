//
// Source code recreated from a .class file by IntelliJ IDEA
// (powered by Fernflower decompiler)
//

package com.google.vr.ndk.base;

import android.content.Context;
import android.os.Handler;
import android.os.RemoteException;
import android.util.AttributeSet;
import android.view.Surface;
import android.view.View;
import android.widget.FrameLayout;
import com.google.vr.cardboard.ContextUtils;
import com.google.vr.cardboard.VrContextWrapper;
import com.google.vr.cardboard.annotations.UsedByReflection;
import com.google.vr.ndk.base.GvrApi;
import com.google.vr.ndk.base.GvrLayoutImpl;
import com.google.vr.ndk.base.GvrLayoutImplWrapper;
import com.google.vr.ndk.base.GvrUiLayout;
import com.google.vr.ndk.base.TraceCompat;
import com.google.vr.vrcore.library.api.IGvrLayout;
import com.google.vr.vrcore.library.api.ObjectWrapper;

@UsedByReflection("Unity")
public class GvrLayout extends FrameLayout {
    private static final String TAG = "GvrLayout";
    private IGvrLayout impl;
    private GvrUiLayout uiLayout;
    private GvrApi gvrApi;

    public GvrLayout(Context var1) {
        super(var1);
        if(ContextUtils.getActivity(var1) == null) {
            throw new IllegalArgumentException("An Activity Context is required for VR functionality.");
        } else {
            this.init();
        }
    }

    public GvrLayout(Context var1, AttributeSet var2, int var3, int var4) {
        super(var1, var2, var3, var4);
        if(ContextUtils.getActivity(var1) == null) {
            throw new IllegalArgumentException("An Activity Context is required for VR functionality.");
        } else {
            this.init();
        }
    }

    public GvrLayout(VrContextWrapper var1) {
        super(var1);
        this.init();
    }

    GvrLayout(Context var1, GvrLayoutImpl var2) {
        super(var1);
        this.impl = new GvrLayoutImplWrapper(var2);
        this.init();
    }

    private void init() {
        TraceCompat.beginSection("GvrLayout.init");

        try {
            if(this.impl == null) {
                this.impl = new GvrLayoutImplWrapper(new GvrLayoutImpl(this.getContext()));
            }

            this.uiLayout = new GvrUiLayout(this.impl.getUiLayout());
            this.gvrApi = new GvrApi(this.getContext(), this.impl.getNativeGvrContext());
            this.addView((View)ObjectWrapper.unwrap(this.impl.getRootView(), View.class));
        } catch (RemoteException var5) {
            throw new RuntimeException(var5);
        } finally {
            TraceCompat.endSection();
        }

    }

    @UsedByReflection("Unity")
    public GvrUiLayout getUiLayout() {
        return this.uiLayout;
    }

    @UsedByReflection("Unity")
    public void onPause() {
        TraceCompat.beginSection("GvrLayout.onPause");

        try {
            this.impl.onPause();
        } catch (RemoteException var5) {
            throw new RuntimeException(var5);
        } finally {
            TraceCompat.endSection();
        }

    }

    @UsedByReflection("Unity")
    public void onResume() {
        TraceCompat.beginSection("GvrLayout.onResume");

        try {
            this.impl.onResume();
        } catch (RemoteException var5) {
            throw new RuntimeException(var5);
        } finally {
            TraceCompat.endSection();
        }

    }

    @UsedByReflection("Unity")
    public void shutdown() {
        TraceCompat.beginSection("GvrLayout.shutdown");

        try {
            this.impl.shutdown();
        } catch (RemoteException var5) {
            throw new RuntimeException(var5);
        } finally {
            TraceCompat.endSection();
        }

    }

    @UsedByReflection("Unity")
    public void setPresentationView(View var1) {
        try {
            this.impl.setPresentationView(ObjectWrapper.wrap(var1));
        } catch (RemoteException var3) {
            throw new RuntimeException(var3);
        }
    }

    public void setFixedPresentationSurfaceSize(int var1, int var2) {
        this.getGvrApi().setSurfaceSize(var1, var2);
    }

    public boolean enableAsyncReprojectionVideoSurface(GvrLayout.ExternalSurfaceListener var1, Handler var2, boolean var3) {
        return ((GvrLayoutImplWrapper)this.impl).getWrappedImpl().enableAsyncReprojectionVideoSurface(var1, var2, var3);
    }

    @UsedByReflection("Unity")
    public boolean setAsyncReprojectionEnabled(boolean var1) {
        try {
            return this.impl.setAsyncReprojectionEnabled(var1);
        } catch (RemoteException var3) {
            throw new RuntimeException(var3);
        }
    }

    public boolean enableCardboardTriggerEmulation(Runnable var1) {
        try {
            return this.impl.enableCardboardTriggerEmulation(ObjectWrapper.wrap(var1));
        } catch (RemoteException var3) {
            throw new RuntimeException(var3);
        }
    }

    public int getAsyncReprojectionVideoSurfaceId() {
        return ((GvrLayoutImplWrapper)this.impl).getWrappedImpl().getAsyncReprojectionVideoSurfaceId();
    }

    public Surface getAsyncReprojectionVideoSurface() {
        return ((GvrLayoutImplWrapper)this.impl).getWrappedImpl().getAsyncReprojectionVideoSurface();
    }

    @UsedByReflection("Unity")
    public GvrApi getGvrApi() {
        return this.gvrApi;
    }

    public void setStereoModeEnabled(boolean var1) {
        try {
            this.impl.setStereoModeEnabled(var1);
        } catch (RemoteException var3) {
            throw new RuntimeException(var3);
        }
    }

    public interface ExternalSurfaceListener {
        void onSurfaceAvailable(Surface var1);

        void onFrameAvailable();
    }
}
