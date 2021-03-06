#include "initgl.h"


/*
 * File:   Init3D.cpp
 * Author: paul
 *
 * Created on 17. Mai 2018, 21:11
 */

#include "initgl.h"
//#include "vecmath/vecmath.h"
#include <vector>
//#include "3DLoader/load3ds.h"

// Res for windowed Mode

#define SD_WIDTH    1280.0f//1024
#define SD_HEIGHT   768.0f//768
#define FULLSCREEN_WIDTH  3840//1680.0f//1920.0f
#define FULLSCREEN_HEIGHT 2160//1050.0f//1200.0f

using namespace irrklang;

InitGL::InitGL (const std::string titel){

    caption = titel;
    window = nullptr;
    maincontext = nullptr;

    _Frames = nullptr;
    _FramerateOut = 0;

    _Mouse.lastx = 0;
    _Mouse.lasty = 0;
    _Mouse.x     = 0;
    _Mouse.y     = 0;

    sphere1 = NULL;
    lightSource = NULL;
    skybox  = NULL;
    base2d  = NULL;
   // land = nullptr;

   // cockpit = NULL;
    textrender = NULL;
   // soundengine = NULL;
    projection = nullptr;

    MainMenu = nullptr;
    showMenu = false;

    LoadConfiguration();
    InitUtils();
    InitMatrices();
    InitFX();

    // Irrklan init
 //   soundengine = createIrrKlangDevice();
 //   if ( soundengine )
 //       loginfo(" SoundEngine created ","InitGL::InitGL");
}

InitGL::InitGL(const InitGL& orig) {
}

InitGL::~InitGL() {

    // Alten Videomode wiederherstellen
    SDL_SetWindowDisplayMode(window,&DesktopDisplayMode);

    /*
    if (soundengine)
        soundengine->drop();
*/
    safeDelete(sphere1);
    safeDelete(lightSource);
    //safeDelete(land);

    if ( skybox != NULL)
        delete skybox;
    if (base2d != NULL  )
        delete base2d;
//    if (cockpit != NULL)
//        delete cockpit;

    if (ambientLight != nullptr)
        delete  ambientLight;

    DeleteUtils();
    DeleteMatrices();
    DeleteShaders();
}

void InitGL::DeleteShaders() {
    glDeleteProgram(cubeshaderprog_color);
    glDeleteProgram(cubeshaderprog_tex);
    glDeleteProgram(sphereshader_color);
    glDeleteProgram(cubeshaderprog_color_normal);
    glDeleteProgram(currentShader);
}

void InitGL::safeDelete(BaseObject * bo) {
    if (bo != NULL) {
        delete bo;
        bo = NULL;
    }
 }

//------------------------------------------
//  Abstracts Render
// -----------------------------------------
void InitGL::Render(glm::mat4 cam) {

    if ( skybox != nullptr)
        skybox->Draw(cam);

    // Hier noch zus??tzliche virtuelle Renderer (objecte, und so weiter!
}

void InitGL::showSkyBox() {

}


bool InitGL::hasSkyBox() { return _HasSkyBox; }
void InitGL::setHasSkybox(bool enable) { _HasSkyBox = enable; }

void InitGL::LoadConfiguration(){


    _FullScreen = false;
    _ResX = SD_WIDTH;
    _ResY = SD_HEIGHT;

    fileUtil * fu = new fileUtil();
    std::vector<std::string> enginecfg;
    bool ok = fu->readLine(MAIN_CFG,enginecfg);

    if (ok) {
        for (uint i = 0; i< enginecfg.size(); i++) {

            std::vector<std::string> parts = split(enginecfg.at(i), SPACE);
            if ( ! parts.empty()) {

                if ( parts.at(0) == "FullScreen") {
                    if (parts.size() > 1) {  // wenigstens 2 eintr??ge:
                        _FullScreen = (StringToInt(parts.at(1)) == 1 ? true : false);
                        if (_FullScreen)
                            loginfo("Schalte auf Fullscreen.......Done");
                        else
                            loginfo("Schalte auf Window-Mode.......Done");
                    }
                }
                else if ( parts.at(0) == "Resolution") {
                    if (parts.size() > 2) {  // wenigstens 3 eintr??ge:
                        _ResX = StringToInt(parts.at(1));
                        _ResY = StringToInt(parts.at(2));
                        loginfo("Setting Resolution " +parts.at(1) + "x" + parts.at(2) + ".......Done");
                    }
                    else
                        logwarn("Warnung: konnte Resolution nich lesen -> verwende Default 1920x1200");
                }
            }
        }
    }
    else
        logwarn("Konnte keine .cfg Datei finden ","CEngine::LoadConfiguration");

}

std::vector<std::string> InitGL::split(std::string const& input, std::string const& separator = " ")
{
  std::vector<std::string> result;
  std::string::size_type position, start = 0;

  while (std::string::npos != (position = input.find(separator, start)))
  {
    result.push_back(input.substr(start, position-start));
    start = position + separator.size();
  }

  result.push_back(input.substr(start));
  return result;
}


//------------------------------------------
// effects
//------------------------------------------
void InitGL::InitFX(){

}


// fog
void InitGL::fogParam(){
    float col[4] = {0.5f,0.5f,0.5f,1.0f};
    glFogi(GL_FOG_MODE,GL_LINEAR);
    glFogfv(GL_FOG_COLOR ,col);
    glFogf(GL_FOG_DENSITY, 0.75f);              // How Dense Will The Fog Be
    glHint(GL_FOG_HINT, GL_DONT_CARE);          // Fog Hint Value
    glFogf(GL_FOG_START, 1.0f);             // Fog Start Depth
    glFogf(GL_FOG_END, 5.0f);               // Fog End Depth
    glEnable(GL_FOG);

}
void InitGL::setFog(bool enable) {
        //Shader use forfogging
}




// ******************************************
// Utils
// ------------------------------------------
void InitGL::InitUtils() {
    filestream = new fileUtil();
    if (filestream == NULL) {
        logwarn ("Konnte Util FileStream nicht erstellen !!","InitGL::InitUtils");
    }
    else
        loginfo("Erstelle FileUtil...... DEone","InitGL::InitUtils");
}

void InitGL::InitShaders() {
    //================================================================
    //Test f??r CustomShader

    shader = new Shader();

    // Vertex Shader
    // ------------------------------------------------------------------------
    std::string v_source ="ShaderSources/cubevertexshader.vex";
    int vs = shader ->compileVertexShaderFromFile(v_source,filestream);
    //Fragment Shader Color
    v_source ="ShaderSources/colorshader.frg";
    int fs_Color = shader ->compileFragmentShaderFromFile(v_source,filestream);
    // Fragment Shader Texture
    v_source ="ShaderSources/cubefragmentshaderMulti.frg";
    int fs_Tex = shader ->compileFragmentShaderFromFile(v_source,filestream);
    // ColorCubeShader
    loginfo("Erstelle Cube Color Shader.................done");
    shader->CreateCustomShader(cubeshaderprog_color);
    shader->AttachCustomShader(cubeshaderprog_color,vs);
    shader->AttachCustomShader(cubeshaderprog_color,fs_Color);
    shader->CreateCustomProgram(cubeshaderprog_color);

    //Texture CubeShader
    loginfo("Erstelle Cube Texture Shader ..............done");
    shader->CreateCustomShader(cubeshaderprog_tex);
    shader->AttachCustomShader(cubeshaderprog_tex,vs);
    shader->AttachCustomShader(cubeshaderprog_tex,fs_Tex);
    shader->CreateCustomProgram(cubeshaderprog_tex);

    // Shader f??r lightning
    loginfo("Erstelle Cube Lightning Shader ..............done");
    v_source ="ShaderSources/cubevertexnormalshader.vex";
    int vsn = shader ->compileVertexShaderFromFile(v_source,filestream);
    //Fragment Shader Color
    v_source ="ShaderSources/cubefragmentshaderMultinormals.frg";
    int fsn = shader ->compileFragmentShaderFromFile(v_source,filestream);
    shader->CreateCustomShader(cubeshaderprog_normals);
    shader->AttachCustomShader(cubeshaderprog_normals,vsn);
    shader->AttachCustomShader(cubeshaderprog_normals,fsn);
    shader->CreateCustomProgram(cubeshaderprog_normals);

    // Shader f??r colorlightning
    loginfo("Erstelle Cube Color Light Shader ..............done");
    v_source ="ShaderSources/vertexnormalcolorshader.vex";
    int vscn = shader ->compileVertexShaderFromFile(v_source,filestream);
    //Fragment Shader Color
    v_source ="ShaderSources/fragmentnormalcolorshader.frg";
    int fscn = shader ->compileFragmentShaderFromFile(v_source,filestream);
    shader->CreateCustomShader(cubeshaderprog_color_normal);
    shader->AttachCustomShader(cubeshaderprog_color_normal,vscn);
    shader->AttachCustomShader(cubeshaderprog_color_normal,fscn);
    shader->CreateCustomProgram(cubeshaderprog_color_normal);

    glDetachShader(cubeshaderprog_color,vs);
    glDetachShader(cubeshaderprog_color,vscn);
    glDetachShader(cubeshaderprog_color,fscn);
    glDetachShader(cubeshaderprog_color,fs_Color);
    glDetachShader(cubeshaderprog_tex,fs_Tex);

    glDetachShader(cubeshaderprog_normals,fsn);
    glDetachShader(cubeshaderprog_normals,vsn);


    // =======================================================================
    //------------------------------------------------------------------------
    // Sphere Shader color:
    v_source = "ShaderSources/spherevertexshader.vex";
    vs = shader->compileVertexShaderFromFile(v_source,filestream);
    // Fragment sHader
    v_source ="ShaderSources/spherefragmentshader.frg";
    fs_Color = shader->compileVertexShaderFromFile(v_source,filestream);
    //Alles zusammenf??gen:
    loginfo("Erstelle Sphere Color Shader ..............done");
    shader -> CreateCustomShader(sphereshader_color);
    shader -> AttachCustomShader(sphereshader_color,vs);
    shader -> AttachCustomShader(sphereshader_color,fs_Color);
    shader ->CreateCustomProgram(sphereshader_color);
    // ========================================================================
    //delete binares
    shader->deleteShader(vs);
    shader->deleteShader(fs_Color);
    shader->deleteShader(fs_Tex);

    shader->deleteShader(vsn);
    shader->deleteShader(fsn);
    // ========================================================================
    currentShader = cubeshaderprog_normals;
}

void InitGL::DeleteUtils() {
    if ( filestream != NULL) {
        delete filestream;
        loginfo("Deleted filestream","InitGL::DeleteUtils");
    }
}

void InitGL::DeleteMatrices() {
    if (projection != NULL) {
        delete projection;
        loginfo("Delete Prokjection Matrix Class","InitGL::DeleteMatrtices");
    }
}

// ******************************************
// MAtrices
// ------------------------------------------
void InitGL::InitMatrices() {
    projection = new Projection(0,_ResX,0,_ResY,0.01f,1000.0f);
    if (projection != NULL) {
        loginfo("Created Procjection Class","InitGL::InitMatrices");
    }
}

// ******************************************
// Heier der graphische Teil
// ------------------------------------------
bool InitGL::InitSDL2()  {


    if (  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO ) != 0 )
    {
            printf("Initialisierung fehlgeschlagen  FEHLERMELDUNG:  %s" , SDL_GetError());
            return(false);

    }
    atexit(SDL_Quit);

    printf("Init success \n");
    SDL_GL_LoadLibrary(NULL);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL,1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,5);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,24);

    int numDisplaymodes = SDL_GetNumDisplayModes(0);
    loginfo("Num Display modes: " + IntToString(numDisplaymodes), "InitGL::Init");

    // Alle Display modes auflisten:


     for(int j = 0; j < SDL_GetNumVideoDisplays(); j++)
     {
        for (int i = 0; i < numDisplaymodes; i++ ) {
            SDL_DisplayMode dpm;
            SDL_GetDisplayMode(j,i,&dpm);
            std::string w = IntToString(dpm.w);
            std::string h = IntToString(dpm.h);
            std::string index =IntToString(i);
            try {

                // todo: Pixelformat auswerten
                loginfo("Display : " + IntToString(j) + "  Resolution Mode[" + index+ "] : " + w + "x" + h ,"InitGL::Init");
            }
            catch ( ...) {
                logwarn("Konnte mode[" + index + "] nicht ermitteln");
            }

        }
     }
    SDL_GetDesktopDisplayMode(0, &DesktopDisplayMode );

//    _ResX = FULLSCREEN_WIDTH;
//    _ResY = FULLSCREEN_HEIGHT;

    if ( _FullScreen) {


        window = SDL_CreateWindow(
                caption.c_str(),
                0, //SDL_WINDOWPOS_UNDEFINED,
                0, //SDL_WINDOWPOS_UNDEFINED,
                _ResX,_ResY,
                //SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_OPENGL
                SDL_WINDOW_OPENGL
            );
        SDL_SetWindowFullscreen(window,SDL_WINDOW_FULLSCREEN);

//        int w,h;
//        SDL_GL_GetDrawableSize(window,&w,&h);
//        logwarn("Drawable size " +IntToString(w) + "  " + IntToString(h) );

    }
    else {

            _ResX = SD_WIDTH;
            _ResY = SD_HEIGHT;
            window = SDL_CreateWindow(
                caption.c_str(),
                SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED,
                _ResX,_ResY,
                SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
            );

    glViewport(0,0,_ResX, _ResY);
}



    if ( window == nullptr)  {
        sdl_die("Konnte Fenster nicht erzeugen");
        return false;
    }

    maincontext = SDL_GL_CreateContext(window);
    if ( ! maincontext) {
        sdl_die (" Konnte Context nicht erzeugen ");
        return false;
    }

    printf(" Geladene OpenGL version :");
    //gladLoadGLLoader(SDL_GL_GetProcAddress);
    // Alles Ok ,checken was initialisiert ist :
    printf("Vendor :  %s\n",glGetString(GL_VENDOR));
    printf("Renderer  %s\n",glGetString(GL_RENDERER));
    printf("Version   %s\n",glGetString(GL_VERSION));// glClearColor(0.5f,0.5f,0.0f,0.0f);


       GLenum err = glGetError();
       switch (err){
           case GL_NO_ERROR         :   loginfo (" Kein Fehler ","Base2D Render");  break;
           case GL_INVALID_ENUM     :   loginfo ("Invalid Enum","Base2D Render");   break;
           case GL_INVALID_VALUE    :   loginfo ("Invalid Value","Base2D REnder");  break;
           case GL_INVALID_OPERATION:   loginfo ("Invalid operation","Base2D Render"); break;
           case GL_INVALID_FRAMEBUFFER_OPERATION: loginfo ("Invalid Framebuffer Operation","Base2D Render"); break;
           case GL_OUT_OF_MEMORY    :   loginfo("Out of Memory","Base2D Render"); break;
           case GL_STACK_UNDERFLOW  :   loginfo ("Stack UNDERFLOW","Base2D Render"); break;
           case GL_STACK_OVERFLOW   :   loginfo ("Stack Overflow","Base2D Render"); break;
           default: loginfo("Undefined");
       }

    //loginfo("Erstelle Viewport " + IntToString(_ResX) + " x " + IntToString(_ResY),"InitGL::InitEngineObject");
    //glViewport(0,0, _ResX , _ResY );

    glewExperimental = GL_TRUE;
    glewInit();
    // Use v-sync
    SDL_GL_SetSwapInterval(1);

    newDisplayMode.w = _ResX;
    newDisplayMode.h = _ResY;
    newDisplayMode.format = SDL_PIXELFORMAT_RGBX8888;


    //if (_FullScreen)
    SDL_SetWindowDisplayMode(window,&newDisplayMode);  // Only in fullscreen available

     // Testweise Displaymode ermitteln
    SDL_DisplayMode current;
    int error = SDL_GetCurrentDisplayMode(1, &current);
    if ( error == 0) {
       printf ("InitGL -- WindowSize:  %i x %i h \n",current.w,current.h);
    }
    else
        printf ("InitGL -- RES_X,RES_Y :  %i x %i h \n",_ResX,_ResY);

    MouseResX = (float)_ResX / (float)current.w;
    MouseResY = (float)_ResY / (float)current.h;

    logwarn("MouseRes X " + FloatToString(MouseResX));
    logwarn("MouseRes Y " + FloatToString(MouseResY));

    InitEngineObject();


    // --------------------------------
    // Test Loader:
    //---------------------------------

    //me = new CMeshObject();
    //me->Load3DSMesh("Meshes/spaceship.3ds");

    //objLoader = new COBJLoader("Meshes/Quad.obj");

    //int errorid = objLoader->LoadOBJ();

    //logwarn(" OBJLoader return ID: " + IntToString(errorid));


    return true;
}

void InitGL::setClearColor(float r, float g, float b) {
    _ClearColor.x = r;
    _ClearColor.y = g;
    _ClearColor.z = b;
    _ClearColor.w = 1.0f;
}


void InitGL::SetClearColor(float r, float g, float b, float a) {
    _ClearColor.x = r;
    _ClearColor.y = g;
    _ClearColor.z = b;
    _ClearColor.w = a;
}

void InitGL::InitEngineObject() {
// ===============================================================
// Hier werden alle Objeckte initialisiert
// ===============================================================


    //                   |Resolution|  | Position  | | width,height, colors             |
//    MainMenu = new CMenu(_ResX, _ResY, _ResX- 200, 0, 200, 900, glm::vec4(0.1,0.1,0.6,0.5), glm::vec4(0.9,0.9,0.9,1.0));
//    loginfo("Erstelle Main Menu ...... done","InitGL::InitEngineObject");



    loginfo("Setze ClearColor auf Schwarz ...... done","InitGL::InitEngineObject");
    _ClearColor.x = 0.0f; _ClearColor.y = 0.0f; _ClearColor.z = 0.0f; _ClearColor.w = 1.0f;
    glClearColor(_ClearColor.x, _ClearColor.y, _ClearColor.z, _ClearColor.w);

    // ---------------------------------------
    // Camera View
    // ---------------------------------------
    loginfo("Erstelle Camera Objekt","InitGL::InitEngineObject");
    camera = new (Camera);
    loginfo("Erstelle Camera Objekt ...... done","InitGL::InitEngineObject");

    // ---------------------------------------
    // Light init
    //----------------------------------------
    ambientLight = new light;
    ambientLight->setColor(glm::vec3(1.0,0.0,1.0));
    ambientLight->setPos(glm::vec3(0.0,1.0,40.0));


    loginfo("Erstelle Standard Ambientes Licht ","InitGL::InitEngineObjects");

    // ---------------------------------------
    // Skybox ,objects init.
    // ---------------------------------------
    loginfo("Erstelle Standard SkyBox \"Dessert\" ........","InitGL::InitEngineObject");
    skybox = new SkyBox(projection->GetPerspective());
    std::vector<std::string> faces;

    faces.push_back("skybox/desert/desert_lf.tga");
    faces.push_back("skybox/desert/desert_rt.tga");
    faces.push_back("skybox/desert/desert_up.tga");
    faces.push_back("skybox/desert/desert_dn.tga");
    faces.push_back("skybox/desert/desert_ft.tga");
    faces.push_back("skybox/desert/desert_bk.tga");
    skybox -> Load(faces);
    loginfo("Erstelle Skybox ........Done","InitGL::InitEngineObject");



    //================================
    // Init 2D Objects
    // ===============================

    logimage("Erstelle Text Renderer.....");
    sPoint p;
    p.x =   100;
    p.y =   400;

    textrender = new TextRender(_ResX, _ResY, p);
    textrender->AddString("Das ist die 1. Zeile");
    textrender->AddString("Das ist die 2. Zeile");
    textrender->AddString("Das ist die 3. Zeile");

    textrender->SetHasBottom(true);
    textrender->SetHasHeader(true);
    textrender->SetHasBackground(true);
    textrender->SetHasTexture(true);
    textrender->SetAlignRight(false);



    loginfo("Erstelle Shaders........");
    InitShaders();
    loginfo("..... done all");
    loginfo("============================");


    //========================================
    // Init 3D Objects
    //========================================

    loginfo("Erstelle 3D Objects .........");
    loginfo("Erstelle cube 1 - 3......done");

    // Liste mit Texture pfaden erstellen

    std::vector<std::string> cubeimages;
    fileUtil fu;
    // Texture loading
    bool texturesok;

    // Sphere
    loginfo("Erstelle Sphere .........done");
    sphere1  = new CSphere(glm::vec3(0.0,0.0,0.0),glm::vec4(1.0,1.0,1.0,1.0), projection->GetPerspective(),15,(GLfloat)4.0,shader);
    sphere1->SetColor(glm::vec4(1.0,1.0,0.5,1.0));
    sphere1->SetHasAlpha(true);

    texturesok =  fu.readLine("config/SphereWorldTextures.cfg",cubeimages);
    if (texturesok)
        sphere1->addTexture(cubeimages,"InitGL::Sphere");
    else
        logwarn("Init::Sphere1 konnte Textures nicht laden ! ","InitGL::Init::cube2::addTexture");
    cubeimages.clear();

    //-----------------------------------------
    // Lightsource as a spere
    //-----------------------------------------
    loginfo("Serstell LichtQuelle als wiesse spere....","InitGL::InitEngineObjects");
    lightSource = new CSphere(ambientLight->getPos(),glm::vec4(1.0,1.0,1.0,1.0),projection->GetPerspective(),18,(GLfloat)2.0,shader );
    lightSource->SetColor(glm::vec4(1.0,1.0,1.0,0.4));
    //Texture loading
    cubeimages.clear();
    texturesok =  fu.readLine("config/cube2textures.cfg",cubeimages);
    if (texturesok)
        lightSource->addTexture(cubeimages,"InitGL::Sphere");
    else
        logwarn("Init::Sphere1 konnte Textures nicht laden ! ","InitGL::Init::cube2::addTexture");
    cubeimages.clear();

    loginfo("Done 3D Objects .............");

    //-----------------------------------------
    // Landscape
    //-----------------------------------------
/*    loginfo("Erstelle Landscape","InitGL::InitEngineObjects");
    land = new LandScape();


    cubeimages.clear();
    texturesok =  fu.readLine("config/cube2textures.cfg",cubeimages);
    if (texturesok)
        land->addTexture(cubeimages,"InitGL::Landscape");
    else
        logwarn("Init::Sphere1 konnte Textures nicht laden ! ","InitGL::InitEngineObjects::Landscape::addTexture");

*/

}

// --------------------------------------------
// Adding's
// --------------------------------------------
void InitGL::add2List(BaseObject *obj, ShaderType s) {

    obj->initShader(COLOR_SHADER,cubeshaderprog_color);
    obj->initShader(TEXTURE_SHADER,cubeshaderprog_tex);
    obj->initShader(LIGHT_SHADER, cubeshaderprog_normals);
    obj->initShader(LIGHT_COLOR_SHADER, cubeshaderprog_color_normal);
    obj->setActiveShader(s);

    obj->addLight(ambientLight);
    list3D.push_back(obj);
}

void InitGL::add2Dobject(Base2D *obj) {
    objects2D.push_back(obj);
}

void InitGL::addButton(CButton* obj) {
    buttons.push_back(obj);
}

void InitGL::toogleFullScreen (){
    Uint32 FullscreenFlag = SDL_WINDOW_FULLSCREEN;
    bool IsFullscreen = SDL_GetWindowFlags(window) & FullscreenFlag;
    SDL_SetWindowFullscreen(window,IsFullscreen ? 0 : FullscreenFlag);

    if (FullscreenFlag ) {

        _ResX = FULLSCREEN_WIDTH;
        _ResY = FULLSCREEN_HEIGHT;

        glViewport(0,0,_ResX, _ResY);//FULLSCREEN_WIDTH, FULLSCREEN_HEIGHT);


    }
    else {
        _ResX = SD_WIDTH;
        _ResY = SD_HEIGHT;
        glViewport(0,0,_ResX, _ResY);
    }
}

void InitGL::setFramelabel(TextRender *pText){
    if (pText != nullptr)
        _Frames = pText;
}

void InitGL::stopAnimation() {
 if (_Animate)
     _Animate = false;
 else
     _Animate = true;
}

void InitGL::Run() {

    bool quit = false;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Diese transformations vectoren enthalten die "steps" f??r die Animation
    vec3 steptrans;
    vec3 steprotate;
    vec3 stepscale;


    steptrans  = vec3(0.0,-0.5,0.0);
    steprotate = vec3(0.5,0.8,0.0);
    stepscale  = vec3(0.0,0.0,0.0);

    sphere1->Translate(vec3(0.0,-4.0,0.0));

    // timetest
    Uint32 tickstart = SDL_GetTicks();
    Uint32 tickend   = tickstart;
    Uint32 elapsed = 0;
    Uint32 second  = 0; // Z??hler f??r elapsed bis 1000 ms
    int event = 0;

    /*

    soundengine = irrklang::createIrrKlangDevice();
    //soundengine1 = irrklang::createIrrKlangDevice();
    //if (soundengine) {



    irrklang::vec3df position(23,70,200);

    // start the sound paused:
    irrklang::ISound* snd = soundengine->play3D("sounds/media/bell.wav", position, false, true);

    if (snd)

    {
       irrklang::vec3df velPerSecond(0,0,0);    // only relevant for doppler effects
       snd ->setVolume( 100);
       snd->setMinDistance(30.0f); // a loud sound
       snd->setIsPaused(false); // unpause the sound
    }

*/
    //--------------------------------------------------
    // framerate berechene
    //--------------------------------------------------
    int frames = 0;
    Uint32 ms = 0;
    showMenu = true;

    while ( ! quit) {
         //     snd ->setVolume(40);
         // snd->drop();
        elapsed = tickend - tickstart;

        ms += elapsed;
        frames++;
        if (ms > 1000) {
            _FramerateOut = frames;
            frames = 0;
            ms = 0;
            if (_Frames != nullptr)
                _Frames->setText(0, IntToString(_FramerateOut));
         }
        textrender->setText(0,"Mouse X " + IntToString(_Mouse.x) );
        textrender->setText(1,"Mouse Y " + IntToString(_Mouse.y) );
        tickstart = tickend;
        // -------------------------------
        // Test f??r dynamische winkel
        // abh??ngig von verbrauchter zeit
        // -------------------------------

       double ups = 5.0f;  // umdrehung per sec...... min

       /*
        * TODO:
        * Klasse f??r SDL_GetTicks() erstellen ,
        * Funktionen f??r U/min bzw. U/sec. erstellen
        * verstrichene Zeit ist eben dann max. 60000 f??r Minute
        * oder 1000 f??r Secunde !!!
        */
       /* Dieser Bereich f??r DauerRotation der KAmeara
       double frames = 6000000.0 / second ;

       double mmin = 10.0f;
       double speed  = 0.2f; // geschwindigkeit

       second += elapsed;

       angleX = 360.0f * ups / frames   ;   // upm jetzt
       float ax = static_cast<float> (angleX);

       //checkdegree(ax);
       if (ax > 360.0f )
           ax -= 360;

       if ( second > 60000 )
          second = 0;

       glm::vec3 pos = camera -> GetOrgPos();
       pos = glm::rotateY(pos,ax);

       camera -> SetPos(pos);
       camera -> UpdateCamera();
       */
       uint motionx =0;
       uint motiony =0;




       e.type = 0;
       SDL_PollEvent( &e );

       event = HandleInput(e,motionx,motiony );

       SDL_Event testEvent = e;

       HandleEvent(testEvent);

       switch ( event) {
           case KEY_Esc  : quit = true;    break;

           case KEY_M :{
               if (showMenu == true)
                    showMenu = false;
                else showMenu = true;
               break;
           }


           case KEY_A: camera->MoveLeft(elapsed); break;
           case KEY_Left : camera->YawCameraLeft(elapsed); break;

           case KEY_D: camera->MoveRight(elapsed);break;
           case KEY_Right: camera->YawCameraRight(elapsed); break;


           case KEY_E: camera->MoveForward(elapsed); break;
           case KEY_Up: camera->PitchCameraUp(elapsed); break; break;

           case KEY_S:camera->MoveBackward(elapsed);break;
           case KEY_Down: camera ->PitchCameraDown(elapsed); break;

           case KEY_Q: stopAnimation(); break;

           // Key "C" und "T" sind f??r Color und Texture Shader
           case KEY_C: {
               currentShader = cubeshaderprog_color;
               break;
           }

           case KEY_T: {
               currentShader = cubeshaderprog_tex;
               break;
           }

           case KEY_F11: {
                toogleFullScreen(); break;
           }

           // ORtho oder perspective mode:
           case KEY_O : {
               // Hier sollte eine Objectliste durchlaufen werden ,
               // bei allen objecten jetzt projection auf orho setzen
               // Hier setzte ich mal alle 3 per "Hand"
           ////    cube->SetProjection(projection->GetOrtho(), true);
           ////    cube2->SetProjection(projection->GetOrtho(), true);
           ////    cube3->SetProjection(projection->GetOrtho(), true);
               //skybox->SetProjection(projection->GetOrtho());
               break;
           }
           case KEY_P: {  // Wired
            ////   cube->SetProjection(projection->GetPerspective(), false);
            ////   cube2->SetProjection(projection->GetPerspective(), false);
            ////   cube3->SetProjection(projection->GetPerspective(), false);
               //skybox->SetProjection(projection->GetOrtho());
               break;
           }

           // Roll Camera left
           case NUM_1: {
               camera->YawEyeLeft(elapsed);
               break;
           }
           // Roll Camera right
           case NUM_3: {
               camera-> YawEyeRight(elapsed);
               break;
           }
           case NUM_2: {
               // Pitch Camera Doun
               camera -> PitchEyeDown(elapsed);
               break;
           }

           case NUM_5: {
               // Pitch camera Up
               camera -> PitchEyeUp(elapsed);//
               break;
           }
       }

       glEnable(GL_DEPTH_TEST);
       glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



       vec3 dummy;
       dummy = vec3(0.0,0.2,0.0);
       if (_Animate) {
            sphere1->SetFirstTranslate(true);
            sphere1 ->StepRotate(dummy);
       }


       //meshObject
        dummy = vec3(1.0,0.0,0.0);
      //  me->SetFirstTranslate(true);
      //  me ->StepRotate(dummy);
      //  me->StepTranslate(glm::vec3(2.0,0.5,0.0));
      //  me->Translate(glm::vec3(dummy));
      //  me->Draw(camera, currentShader);


        // lightsource

        lightSource->SetColor(glm::vec4(1.0,1.0,1.0,1.0));

        //dummy = vec3(0.0,0.2,0.0);

        lightSource->SetFirstTranslate(true);
        if (_Animate)
            lightSource ->StepRotate( glm::vec3(0.0,0.2,0.2));    //dummy);

        sphere1->Draw(camera);
        lightSource->Draw(camera);

        // Landscape

        //land->Draw(camera);


        // ===================================
        // Das beste zum Schluss : Skybox
        // ===================================

        Render(camera->GetView());
        //skybox->Draw(camera->GetView());


        // ===================================
        // Engine Objekte
        // ===================================

        if (! list3D.empty() ) {
            for (unsigned int i=0;i < list3D.size(); i++ ) {
                dummy = vec3(1.0 * (float) i ,2.0,3.0);
                list3D[i]->SetProjection(projection->GetPerspective());
                ////list3D[i]->setActiveShader(LIGHT_SHADER);


                float hlp = 0.1; //(float) (i+1);
                glm::vec3 rv(hlp * 0.5);

                glm::vec3 vt(0.001,0.002,0.003);

                //list3D[0]->Translate(camera->GetPos() + camera->GetDir());

                if (_Animate) {
                  //  if (i > 0) {
                        list3D[i]->StepTranslate(vt);
                        list3D[i]->StepRotate(rv);
                   // }
                }


                vt.x =0.01;
                vt.y =0.0;
                vt.z =0.0;

               // list3D[i]->setActiveShader(ShaderType::);
                list3D[i]->Draw(camera);

            }
        }


        // ===================================
        // Alles f??r 2D Projektion vorbereiten
        //====================================

        Prepare2D();

        //cockpit->Render(0.0f,0.0f,camera);

        //------------------------------------
        // MainMenu rendern
        // -----------------------------------

        if ( MainMenu != nullptr  && showMenu) {
            MainMenu ->Render();

        }


        textrender -> Render();


        if ( !  objects2D.empty() ) {
            for (uint i =0; i < objects2D.size(); i++) {
                (objects2D[i])->Render();
            }
        }

        if ( ! buttons.empty() ) {
            for ( uint i = 0; i < buttons.size(); i++) {
                buttons[i]->Render();

            }
        }

        Restore3D();

        SDL_GL_SwapWindow(window);
        tickend = SDL_GetTicks();

     }
}

void InitGL::Done() {
}

void InitGL::Prepare2D() {
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glDepthMask(0);
}

void InitGL::Restore3D() {
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    glFrontFace(GL_CCW);
    glEnable(GL_BLEND);
    glDepthMask(1);
}



uint InitGL::HandleEvent(SDL_Event e) {
    switch (e.type) {
        case SDL_MOUSEMOTION : OnMouseMove(e.motion.x, e.motion.y, e.button.state); break;

        case SDL_MOUSEBUTTONDOWN: {
            if ( e.button.button == SDL_BUTTON_LEFT ) {
                OnLeftMouseButtonUp(e.motion.x, e.motion.y);
            }
            break;
         }
        case SDL_MOUSEBUTTONUP: {
            if ( e.button.button == SDL_BUTTON_LEFT ) {
                OnLeftMouseButtonClick(e.motion.x, e.motion.y);
            }
            break;
        }
    }
    return  e.type; // Falls wo gebraucht wird
}

//-------------------------------------------------
// Mouse events
//-------------------------------------------------

MOUSE InitGL::convertMouse(int x, int y) {

    MOUSE m;
//    float fx = (float)x * MouseResX;
//  float fy = (float)y * MouseResY;

//    m.x = (int) fx;
//    m.y = (int) fy;
    m.x =  x;
    m.y =  y;

    return  m;
}
void InitGL::OnMouseMove(int &x, int &y, uint buttonstate) {
    _Mouse = convertMouse(x,y);
}

void InitGL::OnLeftMouseButtonUp(int &x, int &y) {

    MOUSE m = convertMouse(x,y);
    if ( ! MainMenu->containerList.empty() && MainMenu != nullptr) {

        for ( uint i = 0; i< MainMenu->containerList.size(); i++) {
            if ( ! MainMenu->containerList.at(i)->buttons.empty()) {

                for (uint j=0; j< MainMenu->containerList.at(i)->buttons.size(); j ++) {
                    if (MainMenu->containerList.at(i)->buttons.at(j)->intersect(m.x, m.y) ) {
                        MainMenu->containerList.at(i)->buttons.at(j)->OnRelease();

                    }
                }
            }
        }
    }
}
void InitGL::OnLeftMouseButtonClick(int &x, int &y) {

    MOUSE m = convertMouse(x,y);

    if ( ! buttons.empty()  ) {
        for (uint i = 0; i < buttons.size(); i++) {
            if (buttons[i]->intersect(m.x, m.y) )
                buttons[i]->OnClick();
        }
    }

    if ( ! MainMenu->containerList.empty() && MainMenu != nullptr) {

        for ( uint i = 0; i< MainMenu->containerList.size(); i++) {
            if ( ! MainMenu->containerList.at(i)->buttons.empty()) {

                for (uint j=0; j< MainMenu->containerList.at(i)->buttons.size(); j ++) {
                    if (MainMenu->containerList.at(i)->buttons.at(j)->intersect(m.x, m.y) ) {
                        MainMenu->containerList.at(i)->buttons.at(j)->OnClick();

                    }
                }
            }
            if ( ! MainMenu->containerList.at(i)->controlls2D.empty() ) {
                for (uint j=0; j< MainMenu->containerList.at(i)->controlls2D.size(); j ++) {
                    if (MainMenu->containerList.at(i)->controlls2D.at(j)->intersect(m.x, m.y) ) {
                        MainMenu->containerList.at(i)->controlls2D.at(j)->OnClick();
                    }
                }
            }
        }
    }
}


int InitGL::HandleInput(SDL_Event e, uint &mox, uint &moy) {

    switch (e.type) {
   //
        case SDL_MOUSEWHEEL :   return  MOUSE_Wheel;

        // Keyboard
        case SDL_KEYDOWN : {
            switch(e.key.keysym.sym ) {
                case SDLK_RIGHT     : return KEY_Right;
                case SDLK_LEFT      : return KEY_Left;
                case SDLK_UP        : return KEY_Up;
                case SDLK_DOWN      : return KEY_Down;
                case SDLK_ESCAPE    : return KEY_Esc;
                // Ziffernblock:
                case SDLK_KP_0      : return NUM_0;
                case SDLK_KP_1      : return NUM_1;
                case SDLK_KP_2      : return NUM_2;
                case SDLK_KP_3      : return NUM_3;
                case SDLK_KP_4      : return NUM_4;
                case SDLK_KP_5      : return NUM_5;
                case SDLK_KP_6      : return NUM_6;
                case SDLK_KP_7      : return NUM_7;
                case SDLK_KP_8      : return NUM_8;
                case SDLK_KP_9      : return NUM_9;

                case SDLK_a         : return KEY_A;
                case SDLK_s         : return KEY_S;
                case SDLK_d         : return KEY_D;
                case SDLK_e         : return KEY_E;
                case SDLK_p         : return KEY_P;  // PerspectiveMode
                case SDLK_o         : return KEY_O;  // Orthomode
                case SDLK_c         : return KEY_C;  // Colored Draw
                case SDLK_t         : return KEY_T;  // Textured Draw
                case SDLK_q         : return KEY_Q;
                case SDLK_m         : return KEY_M;

            case SDLK_F11           : return KEY_F11;

                default : return NO_INPUT;
            }
       }
        // Mouse Buttons
        case SDL_MOUSEBUTTONUP : {
            switch (e.button.button) {
                case SDL_BUTTON_LEFT    :   return MOUSE_BTN_Left_Clk;
                case SDL_BUTTON_MIDDLE  :   return MOUSE_BTN_MiddLe_Clk;
                case SDL_BUTTON_RIGHT   :   return MOUSE_BTN_Right_Clk;
            }
        }

        case SDL_MOUSEMOTION: {

            // Camera Yaw

            int stepx = e.motion.x - ( _ResX / 2);//_Mouse.lastx;
            int stepy = e.motion.y - ( _ResY / 2);//_Mouse.lasty;

             /*
            if ( stepx > 0 ) {

                mox = MOUSE_Move_Right;
                logwarn("Motion X " + IntToString(mox),"InitGL::HandleInput");
            }
            else if ( stepx < 0) {
                mox = MOUSE_Move_Left;
                logwarn("Motion X " + IntToString(mox),"InitGL::HandleInput");
            }
            // Das ganse f??r pitch

            if (stepy > 0 ) {
                moy =  MOUSE_Move_Down;
                logwarn("Motion Y " + IntToString(moy),"InitGL::HandleInput");
            }
            else if (stepy < 0 ) {
                moy =  MOUSE_Move_Up;
                logwarn("Motion Y " + IntToString(moy),"InitGL::HandleInput");

            }
            else
                return NO_INPUT;
            */
            mox = e.motion.x;
            moy = e.motion.y;

            _Mouse.lastx = e.motion.x;
            _Mouse.lasty = e.motion.y;
            return MOUSE_Move;
        }
        default : return NO_INPUT;
    }

    return NO_INPUT;
}

void InitGL::sdl_die( std::string msg)
{
    fprintf(stderr,"%s: %s \n", msg.c_str(),SDL_GetError());

}



