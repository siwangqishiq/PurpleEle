package panyi.xyz.textrender;

import android.content.res.AssetManager;

public final class NativeBridge {
    // Used to load the 'textrender' library on application startup.
    static {
        System.loadLibrary("textrender");
    }

    public static native void init();

    public static native void trick();

    public static native void free();

    public static native void resize(int width , int height);

    public static native void setAndroidAssetManager(AssetManager mgr);

    public static native void handleOnAction(int action , float x , float y);
}
