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
- Simple water simulation based od spring field.

![Skybox, texturing](http://fii.cz/vbhhaepq)
![Reflection, Refraction, Fresnel](http://puu.sh/82fTs/85ce79c53f.jpg)
![Heightmap terrain](http://puu.sh/7J26M/01804c5434.jpg)
![Water simulation](http://puu.sh/7J1XQ/53a0da53e6.jpg)

---------------------
Jan Tobola, 2013-2014
