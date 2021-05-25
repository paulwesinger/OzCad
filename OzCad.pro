QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        3DLoader/load3ds.cpp \
        base2d/base2d.cpp \
        baseobject/baseobject.cpp \
        buttons/button.cpp \
        camera/camera.cpp \
        checkbox/checkbox.cpp \
        colors/colorscheme.cpp \
        controllcontainer/controllcontainer.cpp \
        cube/colorcube.cpp \
        cube/cube.cpp \
        engine/engine.cpp \
        fileutils/fileutil.cpp \
        imageloader/loadimage.cpp \
        initgl.cpp \
        landscape/landscape.cpp \
        lights/light.cpp \
        logs/logs.cpp \
        main.cpp \
        menu/menu.cpp \
        meshobject/meshobject.cpp \
        objectclasses/object.cpp \
        objloader/objloader.cpp \
        plane3D/plane3D.cpp \
        projection/projection.cpp \
        shaders/shader.cpp \
        skybox/skybox.cpp \
        sphere/sphere.cpp \
        textedit/textedit.cpp \
        textrenderer/textrender.cpp \
        utils/utils.cpp \
        vecmath/vecmath.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    BtnInstances/Button0.txt \
    BtnInstances/Button2.txt \
    Description.txt \
    Makefile \
    Meshes/85-cottage_obj.zip \
    Meshes/85-cottage_obj/cottage_obj.mtl \
    Meshes/Kiste.3ds \
    Meshes/Quad.mtl \
    Meshes/spaceship.3ds \
    ShaderSources/FogShaderExample.txt \
    ShaderSources/colorshader.frg \
    ShaderSources/cubefragmentshaderMulti.frg \
    ShaderSources/cubefragmentshaderMultinormals.frg \
    ShaderSources/cubevertexnormalshader.vex \
    ShaderSources/cubevertexshader.vex \
    ShaderSources/fogfragmentshader.frg \
    ShaderSources/fogvertexshader.vex \
    ShaderSources/fragmentnormalcolorshader.frg \
    ShaderSources/vertexnormalcolorshader.vex \
    config/2DObjects/2DObjects.txt \
    config/3DObjects/ColoredCubes/ColorCubes.cfg \
    config/3DObjects/ColoredCubes/Cube1.txt \
    config/3DObjects/ColoredCubes/Cube2.txt \
    config/3DObjects/ColoredCubes/Cube3.txt \
    config/3DObjects/Cube1.txt \
    config/3DObjects/Cube2.txt \
    config/3DObjects/Cube3.txt \
    config/3DObjects/Cube4.txt \
    config/3DObjects/Objects3DList.cfg \
    config/3DObjects/TexturedCubes/Cube1.txt \
    config/3DObjects/TexturedCubes/Cube2.txt \
    config/3DObjects/TexturedCubes/Cube3.txt \
    config/3DObjects/TexturedCubes/Cube4.txt \
    config/3DObjects/TexturedCubes/Cube5.txt \
    config/3DObjects/TexturedCubes/TexCubes.cfg \
    config/Buttons/Button1.txt \
    config/Buttons/Button2.txt \
    config/Buttons/Button3.txt \
    config/Buttons/ButtonList.cfg \
    config/Engine/engine.cfg \
    config/SphereWorldTextures.cfg \
    config/cube2textures.cfg \
    config/cubetextures.cfg \
    fonts/DroidSans.ttf \
    icons/256x256/vlc.png \
    icons/QtProject-qtcreator.png \
    icons/chromium-browser.png \
    icons/ekiga.png \
    icons/gnome-color-manager.png \
    icons/google-chrome.png \
    icons/hb-icon.png \
    icons/ibus-hangul.png \
    icons/ibus-setup-hangul.png \
    icons/linguist.png \
    icons/preferences-color.png \
    icons/smplayer.png \
    images/Add.png \
    images/Add.xcf \
    images/Andromeda200x150.png \
    images/Bottom.png \
    images/Bottom.xcf \
    images/ButtonBG.png \
    images/ButtonBG.xcf \
    images/ButtonBGPressed.xcf \
    images/ButtonPressed.png \
    images/ButtonReleased.png \
    images/ControllBg.png \
    images/Headline.png \
    images/Headline.xcf \
    images/HochtzeitFred3.JPG \
    images/HochzeitFred1.JPG \
    images/HochzeitFred2.JPG \
    images/HochzeitFred4.JPG \
    images/HochzeitFred4.png \
    images/HochzeitFredTorte.JPG \
    images/HochzeitFredTorte.png \
    images/NewCube.png \
    images/NewCube.xcf \
    images/Rahmen.xcf \
    images/ShowSkybox.png \
    images/ShowSkybox.xcf \
    images/Textfeld.png \
    images/Textfeld.xcf \
    images/Textfeld_neu.png \
    images/Textfeld_neu.xcf \
    images/button_green.png \
    images/button_green.xcf \
    images/button_white.png \
    images/darkgray.png \
    images/error.png \
    images/errorAlpha.png \
    images/free-vector-web-buttons-30.jpg \
    images/leaves.jpg \
    images/leaves.png \
    images/leaves.xcf \
    images/redButton.png \
    images/set-of-colorful-web-buttons-vector-1539551.jpg \
    images/testszeneAlpha.png \
    images/text_add_Yellow.png \
    images/text_add_Yellow.xcf \
    images/textfeldrahmen.png \
    images/the_delicate_web_buttons_sinks_01_vector_180084.jpg \
    images/wall.png \
    images/wallAlpha.png \
    images/world-map-1748403__340.webp \
    images/world.png \
    libs/ikpFlac.so \
    libs/ikpMP3.so \
    libs/libIrrKlang.so \
    maser/README.md \
    master/README.md \
    skybox/desert/desert_bk.tga \
    skybox/desert/desert_dn.tga \
    skybox/desert/desert_ft.tga \
    skybox/desert/desert_lf.tga \
    skybox/desert/desert_rt.tga \
    skybox/desert/desert_up.tga \
    skybox/orbital/orbital-element_bk.tga \
    skybox/orbital/orbital-element_dn.tga \
    skybox/orbital/orbital-element_ft.tga \
    skybox/orbital/orbital-element_lf.tga \
    skybox/orbital/orbital-element_rt.tga \
    skybox/orbital/orbital-element_up.tga \
    sounds/media/MF-3DAYS.S3M \
    sounds/media/MF-PANTS.MOD \
    sounds/media/MF-W-90.XM \
    sounds/media/bell.wav \
    sounds/media/explosion.wav \
    sounds/media/getout.ogg \
    sounds/media/license.txt \
    sounds/media/ophelia.mp3

HEADERS += \
    3DLoader/load3ds.h \
    base2d/base2d.h \
    baseobject/baseobject.h \
    buttons/button.h \
    camera/camera.h \
    checkbox/checkbox.h \
    colors/colorscheme.h \
    controllcontainer/controllcontainer.h \
    cube/colorcube.h \
    cube/cube.h \
    defaults.h \
    defines.h \
    engine/engine.h \
    fileutils/fileutil.h \
    imageloader/loadimage.h \
    include/ik_ESoundEngineOptions.h \
    include/ik_ESoundOutputDrivers.h \
    include/ik_EStreamModes.h \
    include/ik_IAudioRecorder.h \
    include/ik_IAudioStream.h \
    include/ik_IAudioStreamLoader.h \
    include/ik_IFileFactory.h \
    include/ik_IFileReader.h \
    include/ik_IRefCounted.h \
    include/ik_ISound.h \
    include/ik_ISoundDeviceList.h \
    include/ik_ISoundEffectControl.h \
    include/ik_ISoundEngine.h \
    include/ik_ISoundMixedOutputReceiver.h \
    include/ik_ISoundSource.h \
    include/ik_ISoundStopEventReceiver.h \
    include/ik_IVirtualRefCounted.h \
    include/ik_SAudioStreamFormat.h \
    include/ik_irrKlangTypes.h \
    include/ik_vec3d.h \
    include/irrKlang.h \
    initgl.h \
    irrklang/ik_ESoundEngineOptions.h \
    irrklang/ik_ESoundOutputDrivers.h \
    irrklang/ik_EStreamModes.h \
    irrklang/ik_IAudioRecorder.h \
    irrklang/ik_IAudioStream.h \
    irrklang/ik_IAudioStreamLoader.h \
    irrklang/ik_IFileFactory.h \
    irrklang/ik_IFileReader.h \
    irrklang/ik_IRefCounted.h \
    irrklang/ik_ISound.h \
    irrklang/ik_ISoundDeviceList.h \
    irrklang/ik_ISoundEffectControl.h \
    irrklang/ik_ISoundEngine.h \
    irrklang/ik_ISoundMixedOutputReceiver.h \
    irrklang/ik_ISoundSource.h \
    irrklang/ik_ISoundStopEventReceiver.h \
    irrklang/ik_IVirtualRefCounted.h \
    irrklang/ik_SAudioStreamFormat.h \
    irrklang/ik_irrKlangTypes.h \
    irrklang/ik_vec3d.h \
    irrklang/irrKlang.h \
    landscape/landscape.h \
    lights/light.h \
    logs/logs.h \
    menu/menu.h \
    meshobject/meshobject.h \
    objectclasses/object.h \
    objloader/objloader.h \
    plane3D/plane3D.h \
    projection/projection.h \
    shaders/shader.h \
    skybox/skybox.h \
    sphere/sphere.h \
    textedit/textedit.h \
    textrenderer/textrender.h \
    utils/utils.h \
    vecmath/vecmath.h
