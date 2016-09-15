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
[![Water rendering](./WaterRendering/doc/vid1.jpg)](https://www.youtube.com/watch?v=SAgEGvJKh8w)


---------------------
Jan Tobola, 2013-2014
