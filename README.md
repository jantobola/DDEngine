DDEngine
========

DDEngine is a lightweight graphics engine written in C++ and Direct3D 11. It uses 3rd party libraries such as DirectXTK, DirectxTEX, AntTweakBar, Assimp, CEGUI.

Main focus of the engine is to support easy development and testing of graphic algorithms and shaders.

FEATURES
--------
- Shader management
- Object management
- Real-time shader compiling
- Model loading
- Console
- External config
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
- Sample project



-----------------

Samples
-------
![Skybox, texturing](http://fii.cz/vbhhaepq)
![Sphere](http://puu.sh/cAxqS/1ffe115e88.jpg)
![Water simulation 2](https://scontent-a.xx.fbcdn.net/hphotos-prn2/t31.0-8/10286810_10202082145115757_252098159032020820_o.jpg)
![Water simulation 3](https://scontent-a.xx.fbcdn.net/hphotos-prn2/l/t31.0-8/10275344_10202088807242306_7099633422965902923_o.jpg)

Object management
---
![Object management](http://puu.sh/cplxG/0a3af47acf.jpg)

External config
---
![External config](http://puu.sh/cBQFU/2820c767ac.png)


---------------------
Jan Tobola, 2013-2014
