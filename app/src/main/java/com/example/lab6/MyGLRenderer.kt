package com.example.lab6

import android.content.Context
import android.graphics.BitmapFactory
import android.opengl.GLSurfaceView
import android.opengl.Matrix
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class MyGLRenderer(private val context: Context): GLSurfaceView.Renderer {
    private val projectionMatrix = FloatArray(16)
    private lateinit var pixels: IntArray

    override fun onSurfaceCreated(p0: GL10?, p1: EGLConfig?) {
        CRenderer.onSurfaceCreated()
        val options = BitmapFactory.Options()
        options.inScaled = false
        val bitmap = BitmapFactory.decodeResource(context.resources, R.drawable.box, options)
        pixels = IntArray(bitmap.width * bitmap.height)
        bitmap.getPixels(pixels, 0, bitmap.width, 0, 0, bitmap.width, bitmap.height)
        CRenderer.setTexture(pixels, bitmap.width, bitmap.height)
        bitmap.recycle()
    }

    override fun onSurfaceChanged(p0: GL10?, width: Int, height: Int) {
        val ratio = width.toFloat() / height.toFloat()
        Matrix.perspectiveM(projectionMatrix, 0, 45f, ratio, 0.1f, 100f)
        CRenderer.onSurfaceChanged(width, height, projectionMatrix)
    }

    override fun onDrawFrame(p0: GL10?) {
        CRenderer.render()
    }
}