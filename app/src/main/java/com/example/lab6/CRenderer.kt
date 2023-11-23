package com.example.lab6

class CRenderer {

    companion object {

        external fun onSurfaceCreated()

        external fun onSurfaceChanged(width: Int, height: Int, projMatrix: FloatArray)

        external fun render()

        external fun setTexture(pixels: IntArray, width: Int, height: Int)

        init {
            System.loadLibrary("lab6")
        }
    }
}