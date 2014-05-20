DDEngine
========

DDEngine is a lightweight graphics engine written in C++ and Direct3D 11. It uses 3rd party libraries such as DirectXTK, CEGUI, AntTweakBar.

This engine is not a game engine. The main purpose of this engine is to test CG algorithms quickly and easily.

FEATURES
--------
- Shader management
- Real-time shader compiling
- Console
- Easy to use

MODULES
-------

DDEngine
- Core module, engine logic
- Result from build is a static library (ddengine.lib)

DDEngineRes
- Resource Only DLL. Resources such as textures, fonts, xml schemes are packed into one dll (dderes.dll)
- DDEngine module depends on this module.

WaterRendering
- It is a demo project. It demonstrates how DDEngine can be used.

![Wireframe](http://puu.sh/82gEN/3625c118e7.jpg)
![Skybox, texturing](http://fii.cz/vbhhaepq)
![Water simulation](http://puu.sh/7J1XQ/53a0da53e6.jpg)
![Water simulation 2](https://scontent-a.xx.fbcdn.net/hphotos-prn2/t31.0-8/10286810_10202082145115757_252098159032020820_o.jpg)
![Water simulation 3](https://scontent-a.xx.fbcdn.net/hphotos-prn2/l/t31.0-8/10275344_10202088807242306_7099633422965902923_o.jpg)


---------------------
Jan Tobola, 2013-2014
