#include "engine.h"
#include "../utils/utils.h"
#include "../landscape/landscape.h"
//#include "../defaults.h"
/*

#include <stdio.h>
#include <usb.h>
main(){
    struct usb_bus *bus;
    struct usb_device *dev;
    usb_init();
    usb_find_busses();
    usb_find_devices();
    for (bus = usb_busses; bus; bus = bus->next)
        for (dev = bus->devices; dev; dev = dev->next){
            printf("Trying device %s/%s\n", bus->dirname, dev->filename);
            printf("\tID_VENDOR = 0x%04x\n", dev->descriptor.idVendor);
            printf("\tID_PRODUCT = 0x%04x\n", dev->descriptor.idProduct);
        }
}

*/



#define safeDelete(pt) \
    if (pt != nullptr) \
        delete pt;


//------------------------------------------------------------------
// static functions for Button Handler
//------------------------------------------------------------------




void EnableButton1() {
    if ( butn0 != nullptr)
        butn0->enable();
    if ( butn1 != nullptr)
        butn1->enable();
}
void DisableButton1() {
    if ( butn0 != nullptr)
        butn0->disable();
    if ( butn2 != nullptr)
        butn2->disable();
}
void Button3() {
    if ( butn1 != nullptr)
        butn1->disable();
}

void FxFog() {
    loginfo("TestFunc2","Fog");
}

void EngineTestFunc2() {
     loginfo("TestFunc2","Engine:: EngineTestFunc 2");
}
void EngineTestFunc3() {
    loginfo("TestFunc3","Engine:: EngineTestFunc 3");
}

void EngineTestFunc4() {
    loginfo("TestFunc4","Engine:: EngineTestFunc 4");
}


CEngine::CEngine(std::string titel) :
        InitGL(titel){
    con1 = nullptr;
    con2 = nullptr;

    fogBtn = nullptr;
}

CEngine::~CEngine(){
    safeDelete (con1);
    safeDelete (con2);

    //safeDelete(fogBtn);
}

void CEngine::Done() {
    InitGL::Done();
}

void CEngine::Run() {

    initMenu();
    Init2D();
    Init3D();
    InitButtons();
    //logimage("Erstelle Cockpt............");
    //cockpit= new Base2D(_ResX, _ResY,"images/blackWidowAlpha1920x1200.png");

    InitGL::Run();
}

// ---------------------------------------------------------------
// Place here Functons for button handler..
// ---------------------------------------------------------------

void CEngine::functoogleCheckBox() {


}
void CEngine::funcToogleSkybox() {
    if (renderSkybox) {
        renderSkybox = false;
        loginfo("skybox = false");
    }
    else{
        renderSkybox = true;
        loginfo("skybox = true");
    }
}

void CEngine::funcFog(){
    loginfo("Fog - function");
}

void CEngine::Render(glm::mat4 cam) {
    if (renderSkybox)
        InitGL::Render(cam);
}


// --------------------------------------------------------------
// Init Methods for Engines
// --------------------------------------------------------------
void CEngine::initMenu(){


    //                  |Resolution|  | Position           | width,height, colors             |
     MainMenu = new CMenu(_ResX, _ResY, 0, 0, MENU_WIDTH, MENU_HEIGHT, glm::vec4(0.1,0.1,0.1,0.8), glm::vec4(0.9,0.9,0.9,1.0));
    // loginfo("Erstelle Main Menu ...... done","InitGL::InitEngineObject");

    int curr_y;
    // -------------------------------------
    // Standard Menu ist in Initgl vorhanden
    // jetzt  bef??llen
    //--------------------------------------

    con1 = new CControllContainer(MainMenu->Pos().x,
                                  MainMenu->Pos().y,
                                  MainMenu->Width(), 0);

    butn0 = new CImageButton(_ResX, _ResY,"images/ButtonReleased.png", "images/NewCube.png", con1->NextControllPos() );
    butn0->setSize(BTN_WIDTH,BTN_HEIGHT);
    butn0->setScale(TEXT::SCALE_SMALL);
    butn0->setColor(BTN_ENABLE);
    butn0->setDisablecolor(BTN_DISABLE);
    butn0->AddHandler(FxFog);
    con1->addButton(butn0);

    butn1 = new CImageButton(_ResX, _ResY,"images/ButtonReleased.png", "images/ShowSkyBox.png", con1->NextControllPos());
    butn1->useShader(TEXTURE_SHADER);
    butn1->setColor(BTN_ENABLE);
    butn1->setDisablecolor(BTN_DISABLE);
    butn1->setSize(BTN_WIDTH,BTN_HEIGHT);
    butn1->AddHandler(DisableButton1);
  //  butn1->setbuttonColors(glm::vec3(0,0,1), glm::vec3(0,0,1));
    con1->addButton(butn1);

    butn2 = new CImageButton(_ResX, _ResY,"images/ButtonReleased.png",  "images/NewCube.png", con1->NextControllPos());
    butn2->setSize(BTN_WIDTH,BTN_HEIGHT);
    butn2->setScale(TEXT::SCALE_SMALL);
    butn2->setColor(BTN_ENABLE);
    butn2->setDisablecolor(BTN_DISABLE);
    butn2->AddHandler(Button3);
    con1->addButton(butn2);
    MainMenu->addConatiner(con1);

    curr_y = MainMenu->CurrentY() + MENU_SPACER;

    //---------------------------------------------------
    // 2. container
    //---------------------------------------------------
    con2 = new CControllContainer(MainMenu->Pos().x,
                                             curr_y,
                                  MainMenu->Width(),0);
    fogBtn = new CImageButton(_ResX, _ResY, "images/ButtonReleased.png", "images/ToogleSkybox.png",con2->NextControllPos() );
    fogBtn->setColor(BTN_ENABLE);
    fogBtn->setDisablecolor(BTN_DISABLE);
    fogBtn->setSize(BTN_WIDTH,BTN_HEIGHT);

    fogBtn->AddHandler(CEngine::funcToogleSkybox);
    con2->addButton(fogBtn);
    //-----------------------------------------------------
    // Textedit:
    //-----------------------------------------------------
    sSize s;
    sPoint p;
    //con2->addSpacer();
    curr_y = MainMenu->CurrentY();

    s.w = MainMenu->Width() - 2;
    s.h = CONTROLL::HEIGHT;

    p = con2->NextControllPos();

    txtEdit = new TextEdit(_ResX, _ResY, "images/ButtonReleased.png", p,s ,glm::vec4(0.79, 0.99, 1.0, 1.0) , glm::vec4(0.79, 0.99, 1.0, 1.0));
    //txtEdit->setSize(s.w,s.h);
    txtEdit->setPos(p.x, p.y);
    txtEdit->setColor(glm::vec4(0.79, 0.99, 1.0, 1.0));
    con2->addControll2D(txtEdit);
    curr_y = MainMenu->CurrentY();

    // add label for Frames to buildin textrender label
    txtEdit->setLabel("FPS");
    setFramelabel(txtEdit->getTextPtr());
    //MainMenu->addConatiner(con2);

    //----------------------------------------------------
    // checkbox
    //----------------------------------------------------
    p = con2->NextControllPos();

    checkBox = new CheckBox(_ResX, _ResY, "images/ButtonReleased.png", p,s ,glm::vec4(0.79, 0.99, 1.0, 1.0) );
    //txtEdit->setSize(s.w,s.h);
    checkBox->setPos(p.x, p.y);
    checkBox->setColor(glm::vec4(0.79, 0.99, 1.0, 1.0));
    con2->addControll2D(checkBox);
    curr_y = MainMenu->CurrentY();

    // add label for Frames to buildin textrender label
    checkBox->setLabel("CheckBox");
    MainMenu->addConatiner(con2);
}

void CEngine::Init2D() {

    //================================
    // Init 2D Objects
    // ==========y=====================

    // Testweise mal einf??gen
    loginfo("============================");
    loginfo("Erstelle 2D Objekte.........");
    loginfo("============================");
    logimage("Erstelle TestIcon..........");
    base2d = new Base2D(_ResX, _ResY,"icons/ibus-setup-hangul.png");
    base2d->setPos(500,400);
    add2Dobject(base2d);
  }

void CEngine::Init3D(){


   logEmptyLine();
   loginfo("============================");
   loginfo("Erstelle 3D Objekte.........");
   loginfo("============================");


   fileUtil * fileutil = new fileUtil();
   bool ok;
   //--------------------------------------
   // loading Textured cubes
   //--------------------------------------
   ok = fileutil->readLine(OBJECT3D_CFG + "TexturedCubes/TexCubes.cfg", object3DTexturedList);
   if (ok) {
        if ( ! loadTexturedCubes() )
            logwarn("Fehler: Keine Textured Cubes gefunden oder Fehler im Pfad!");
   }
   else
       logwarn("Fehler: Datei  < config/TexCubes.cfg  > nicht gefunden !");


   // --------------------------------------
   // colored cubes loading
   //---------------------------------------
   ok = fileutil->readLine(OBJECT3D_CFG + "ColoredCubes/ColorCubes.cfg", object3DColoredList);
   if (ok) {
        if ( ! loadColorCubes() )
            logwarn("Fehler: Keine Colored Cubes gefunden oder Fehler im Pfad!");
   }
   else
       logwarn("Fehler: Datei  < config/ColorCubes.cfg  > nicht gefunden !");


   // --------------------------------------
   // Landscape loading
   //---------------------------------------
   ok = fileutil->readLine(OBJECT3D_CFG + "Landscapes/Landscapes.cfg", object3DLandscapeList);
   if (ok) {
        if ( ! loadLandscape() )
            logwarn("Fehler: Keine Landscape gefunden oder Fehler im Pfad!");
   }
   else
       logwarn("Fehler: Datei  < config/Landscape.cfg  > nicht gefunden !");


   logEmptyLine() ;
   loginfo("----------------------------");
   loginfo(" Init 3D Objects ");
   loginfo("     READY");
   loginfo("----------------------------");
}


bool CEngine::loadLandscape() {
    loginfo("Lade Datei |Landscapes.cfg|","CEngine::loadLandscapes");

     // Liste mit Objekten abarbeiten :

    if (object3DLandscapeList.empty() )
        return false;

    logwarn("Anzahl der Landscapes: " + IntToString(object3DLandscapeList.size()));




    for (unsigned int i = 0; i < object3DLandscapeList.size(); i++) {

         std::string path = OBJECT3D_CFG + "Landscapes/" + object3DLandscapeList[i];

         loginfo("Erstelle Object: .......< " + path+ " >","Engine::loadLandscapes");

         fileUtil * objreader = new fileUtil;
         std::vector<std::string> objconfig;
         objreader->readLine(path, objconfig);

         if( ! objconfig.empty() ) {

             loginfo("objconfig size : " + IntToString(objconfig.size()),"Engine::loadLandScape");

             for (int i =0; i< objconfig.size(); i++) {
                 logimage("objconfig[" + IntToString(i)+ "] : " + objconfig[i] ,"Engine::loadLandscape");
             }


             sLandscape landscape;
             if (initLandscape(landscape,objconfig)) {

                 LandScape * obj = new LandScape();
                 //obj->SetColor(glm::vec4(s3D.color.x, s3D.color.y, s3D.color.z, s3D.color.w));
                 if ( landscape.textures == "NONE" )
                     obj->SetHasTextures( false);
                 else
                     obj->SetHasTextures( true);

                 //obj->SetColor(glm::vec4(landscape.color.x, landscape.color.y, landscape.color.z, landscape.color.w));
                 //obj->Rotate(glm::vec3(landscape.trans.rotate.x, landscape.trans.rotate.y, landscape.trans.rotate.z) );
                 //obj->Translate(glm::vec3(landscape.trans.translate.x, landscape.trans.translate.y, landscape.trans.translate.z));
                 //obj->Scale(glm::vec3(landscape.trans.scale.x, landscape.trans.scale.y, landscape.trans.scale.z));
                 obj->SetColor(glm::vec4(0.0,1.0,0.0,1.0));
                 obj->Rotate(BASE::ZERO_VEC3);
                 obj->Translate( BASE::ZERO_VEC3);
                 obj->Scale(glm::vec3(1.0,1.0,1.0));

                 //obj->setPatchX(landscape.patchX);
                 //obj->setPatchZ(landscape.patchZ);
                 //obj->setRasterX(landscape.rasterX);
                 //obj->setRasterZ(landscape.rasterZ);

                 //----------------------------------------
                 // Add textures , if we have some
                 // ---------------------------------------
                 bool texturesok;
                 std::vector<std::string> images;

                 std::string path = landscape.textures;
                 if ( landscape.textures != "NONE" ) {
                     fileUtil fu;

                     texturesok =  fu.readLine(path, images);

                     if (texturesok)
                         obj->addTexture(images,"Engine::loadLandscape");
                     else
                         logwarn("Engine::loadScape :  konnte Textures nicht laden ! ","Engine::loadLandscape");
                 }

                 obj->init();

                 loginfo("Landscape initialisisert ","CEngine::loadLandscape");
                 add2List(obj,LIGHT_SHADER);
             }
             else
                 logwarn("konnte Landscape nicht initialisieren !!", "CEngine::loadLandscpae" );
                // Hier die neuen stringpart functions einbauen

             logEmptyLine();
             loginfo("Prepare for next Object: ","CEngine::init3D");

        }
    }
    return true;
}


bool CEngine::loadTexturedCubes(){

    loginfo("Lade Datei |TexCubes.cfg|","CEngine::loadTextureCubes");

     // Liste mit Objekten abarbeiten :

    if (object3DTexturedList.empty() )
        return false;

    for (unsigned int i = 0; i < object3DTexturedList.size(); i++) {

         std::string path = OBJECT3D_CFG + "TexturedCubes/" + object3DTexturedList[i];

         loginfo("Erstelle Object: .......< " + path+ " >","Engine::loadTexturedCubes");

         fileUtil * objreader = new fileUtil;
         std::vector<std::string> objconfig;
         objreader->readLine(path, objconfig);


         if( ! objconfig.empty() ) {

             s3DStruct s3D;

             if (init3DStruct(s3D,objconfig)) {

                 CCube * obj = new CCube();
                 //obj->SetColor(glm::vec4(s3D.color.x, s3D.color.y, s3D.color.z, s3D.color.w));
                 if ( s3D.textures == "" )
                     obj->SetHasTextures( false);
                 else
                     obj->SetHasTextures( true);

                 obj->SetColor(glm::vec4(s3D.color.x, s3D.color.y, s3D.color.z, s3D.color.w));
                 obj->SetFirstTranslate( ( s3D.firstTranslate == 1) ? true: false);
                 obj->Rotate(glm::vec3(s3D.trans.rotate.x, s3D.trans.rotate.y, s3D.trans.rotate.z) );
                 obj->Translate(glm::vec3(s3D.trans.translate.x, s3D.trans.translate.y, s3D.trans.translate.z));
                 obj->Scale(glm::vec3(s3D.trans.scale.x, s3D.trans.scale.y, s3D.trans.scale.z));

                 //----------------------------------------
                 // Add textures , if we have some
                 // ---------------------------------------
                 bool texturesok;
                 std::vector<std::string> images;

                 std::string path = s3D.textures;
                 if ( s3D.textures != "NONE" ) {
                     fileUtil fu;

                     texturesok =  fu.readLine(path, images);
                     if (texturesok)
                         obj->addTexture(images,"InitGL::add3DObject");
                     else
                         logwarn("Init::Cube2 :  konnte Textures nicht laden ! ","InitGL::Init::cube2::addTexture");
                 }
                 loginfo("s3D initialisisert ","CEngine::init3D");
                 add2List(obj,LIGHT_SHADER);

             }
             else
                 logwarn("konnte s3D nicht initialisieren !!", "CEngine::init3D" );
                // Hier die neuen stringpart functions einbauen

             logEmptyLine();
             loginfo("Prepare for next Object: ","CEngine::init3D");

        }
    }
    return true;
}

bool CEngine::loadColorCubes() {

    loginfo("Lade Datei |ColorCubes.cfg|","CEngine::loadColorCubes");

     // Liste mit Objekten abarbeiten :

    if (object3DColoredList.empty() )
        return false;

    for (unsigned int i = 0; i < object3DColoredList.size(); i++) {

         std::string path = OBJECT3D_CFG + "ColoredCubes/" + object3DColoredList[i];

         loginfo("Erstelle Object: .......< " + path+ " >","Engine::Init3D");

         fileUtil * objreader = new fileUtil;
         std::vector<std::string> objconfig;
         objreader->readLine(path, objconfig);


         if( ! objconfig.empty() ) {

             s3DStruct s3D;

             if (init3DStruct(s3D,objconfig)) {

                 CColorCube * obj = new CColorCube();
                 //obj->SetColor(glm::vec4(s3D.color.x, s3D.color.y, s3D.color.z, s3D.color.w));
                 obj->SetHasTextures( false);
                 obj->SetColor(glm::vec4(s3D.color.x, s3D.color.y, s3D.color.z, s3D.color.w));
                 obj->SetFirstTranslate( ( s3D.firstTranslate == 1) ? true: false);
                 obj->Rotate(glm::vec3(s3D.trans.rotate.x, s3D.trans.rotate.y, s3D.trans.rotate.z) );
                 obj->Translate(glm::vec3(s3D.trans.translate.x, s3D.trans.translate.y, s3D.trans.translate.z));
                 obj->Scale(glm::vec3(s3D.trans.scale.x, s3D.trans.scale.y, s3D.trans.scale.z));

                 loginfo("s3D initialisisert ","CEngine::init3D");

                 add2List(obj,LIGHT_COLOR_SHADER);
             }
             else
                 logwarn("konnte s3D nicht initialisieren !!", "CEngine::init3D" );
                // Hier die neuen stringpart functions einbauen

             logEmptyLine();
             loginfo("Prepare for next Object: ","CEngine::init3D");

        }
    }
    return true;
}


void CEngine::InitButtons() {

    logEmptyLine();
    loginfo("============================");
    loginfo("Erstelle Buttons ...........");
    loginfo("============================");

    loadButtons();

}

void CEngine::loadButtons() {
    // Open file
    bool ok;
    fu = new fileUtil();

    ok = fu->readLine(BUTTONS_CFG + "ButtonList.cfg", btnList);


    if (ok) {
        loginfo("Lade Datei |ButtonList.cfg|","CEngine::loadbuttons");
        logEmptyLine();

        for (unsigned int i = 0; i < btnList.size(); i++  ){

            std::string path = BUTTONS_CFG + btnList[i];

            fileUtil * btnreader = new fileUtil;
            std::vector<std::string> btnconfig;
            btnreader->readLine(path, btnconfig);



            if ( ! btnconfig.empty() ) {

                loginfo("Erstelle Button: .......< " + path+ " >", "Engine::loadButtons");

                sButtonStruct btnStruct;
                if  (initButtonStruct(btnStruct,btnconfig) ) {
                    sPoint pos;
                    CButton * btn = new CTextButton(_ResX,_ResY ,btnStruct.path,"",pos);//   btnStruct.path);
                    btn->setPos(btnStruct.PosX,btnStruct.PosY);
                    btn->setSize(btnStruct.SizeX, btnStruct.SizeY);
                    btn->setbuttonColors(glm::vec3(btnStruct.ImageRed,btnStruct.ImageGreen,btnStruct.ImageBlue),
                                         glm::vec3(btnStruct.TextRed,btnStruct.TextGreen,btnStruct.TextBlue));

                    switch (i) {
                        case 0: btn ->AddHandler(EngineTestFunc4); break;
                        case 1: btn ->AddHandler(EngineTestFunc2); break;
                        case 2: btn ->AddHandler(EngineTestFunc3); break;
                    default:
                        btn ->AddHandler(EngineTestFunc4);
                    }
                    if (btnStruct.Enable > 0 )
                        btn->enable();
                    else
                        btn->disable();
                    btn->setText("Edit");
                    addButton(btn);

                    loginfo(".........Done", "Engine::loadButtons");
                    logEmptyLine();
                }

            }

        }
    }
    else
        logwarn("Fehler: Datei | config/ButtonList nicht gefunden ! |");


    logEmptyLine() ;
    loginfo("----------------------------");
    loginfo(" Load Button Objects ");
    loginfo("        READY");
    loginfo("----------------------------");

}

std::string &CEngine::getValueItem(std::string &s, std::string erasestring) {
    return s.erase(0,erasestring.length() ) ;
}

bool CEngine::initLandscape(sLandscape &ls, std::vector<std::string> &cfg){
    if (cfg.size() >= CFG_3D_SIZE ) {

        //+---------------------------------------------------------------------+
        //+     VORGEHEN :                                                      |
        //+     Liste abarbeiten, Teilstring bis " " ermitteln,                 |
        //+     Variablen name = Teilstring --> 2. Teilstring in Wert wandeln   |
        //+     und in der s3DStruct zuweisen                                   |
        //+---------------------------------------------------------------------+

        for (uint i = 0; i < cfg.size(); i++) {
            std::vector<std::string> parts = split(cfg.at(i), SPACE);

            if (parts.at(0) == "colorRed")
                ls.color.x = StringToFloat(parts.at(1));

            if (parts.at(0) == "colorGreen")
                ls.color.y = StringToFloat(parts.at(1));

            if (parts.at(0) == "colorBlue")
                ls.color.z = StringToFloat(parts.at(1));

            if (parts.at(0) == "colorAlpha")
                ls.color.w = StringToFloat(parts.at(1));

            if (parts.at(0) == "hasLight")
                ls.hasLight = StringToInt(parts.at(1));

            if (parts.at(0) == "textures")
                ls.textures = parts.at(1);

            if (parts.at(0) == "translateX")
                ls.trans.translate.x = StringToFloat(parts.at(1));

            if (parts.at(0) == "translateY")
                ls.trans.translate.y = StringToFloat(parts.at(1));

            if (parts.at(0) == "translateZ")
                ls.trans.translate.z = StringToFloat(parts.at(1));

            if (parts.at(0) == "rotateX")
                ls.trans.rotate.x = StringToFloat(parts.at(1));

            if (parts.at(0) == "rotateY")
                ls.trans.rotate.y = StringToFloat(parts.at(1));

            if (parts.at(0) == "rotateZ")
                ls.trans.rotate.z = StringToFloat(parts.at(1));

            if (parts.at(0) == "scaleX")
                ls.trans.scale.x = StringToFloat(parts.at(1));

            if (parts.at(0) == "scaleY")
                ls.trans.scale.y = StringToFloat(parts.at(1));

            if (parts.at(0) == "scaleZ")
                ls.trans.scale.z = StringToFloat(parts.at(1));

            if (parts.at(0) == "patchX")
                ls.trans.scale.y = StringToInt(parts.at(1));

            if (parts.at(0) == "patchZ")
                ls.trans.scale.z = StringToInt(parts.at(1));

            if (parts.at(0) == "rasterX")
                ls.rasterX = StringToFloat(parts.at(1));

            if (parts.at(0) == "rasterZ")
                ls.rasterZ = StringToFloat(parts.at(1));


        }
        return true;

    }
    return false;
}

bool CEngine::init3DStruct(s3DStruct &d3s, std::vector<std::string> &cfg){
    if (cfg.size() >= CFG_3D_SIZE ) {

        //+---------------------------------------------------------------------+
        //+     VORGEHEN :                                                      |
        //+     Liste abarbeiten, Teilstring bis " " ermitteln,                 |
        //+     Variablen name = Teilstring --> 2. Teilstring in Wert wandeln   |
        //+     und in der s3DStruct zuweisen                                   |
        //+---------------------------------------------------------------------+

        for (uint i = 0; i < cfg.size(); i++) {
            std::vector<std::string> parts = split(cfg.at(i), SPACE);

            if (parts.at(0) == "originX")
                d3s.origin.x = StringToFloat(parts.at(1));

            if (parts.at(0) == "originY")
                d3s.origin.y = StringToFloat(parts.at(1));

            if (parts.at(0) == "originZ")
                d3s.origin.z = StringToFloat(parts.at(1));

            if (parts.at(0) == "colorRed")
                d3s.color.x = StringToFloat(parts.at(1));

            if (parts.at(0) == "colorGreen")
                d3s.color.y = StringToFloat(parts.at(1));

            if (parts.at(0) == "colorBlue")
                d3s.color.z = StringToFloat(parts.at(1));

            if (parts.at(0) == "colorAlpha")
                d3s.color.w = StringToFloat(parts.at(1));

            if (parts.at(0) == "hasLight")
                d3s.hasLight = StringToInt(parts.at(1));

            if (parts.at(0) == "textures")
                d3s.textures = parts.at(1);

            if (parts.at(0) == "translateX")
                d3s.trans.translate.x = StringToFloat(parts.at(1));

            if (parts.at(0) == "translateY")
                d3s.trans.translate.y = StringToFloat(parts.at(1));

            if (parts.at(0) == "translateZ")
                d3s.trans.translate.z = StringToFloat(parts.at(1));

            if (parts.at(0) == "rotateX")
                d3s.trans.rotate.x = StringToFloat(parts.at(1));

            if (parts.at(0) == "rotateY")
                d3s.trans.rotate.y = StringToFloat(parts.at(1));

            if (parts.at(0) == "rotateZ")
                d3s.trans.rotate.z = StringToFloat(parts.at(1));

            if (parts.at(0) == "scaleX")
                d3s.trans.scale.x = StringToFloat(parts.at(1));

            if (parts.at(0) == "scaleY")
                d3s.trans.scale.y = StringToFloat(parts.at(1));

            if (parts.at(0) == "scaleZ")
                d3s.trans.scale.z = StringToFloat(parts.at(1));

            if (parts.at(0) == "firstTranslate")
                d3s.firstTranslate = StringToInt(parts.at(1));


        }
        return true;

    }
    return false;
}

bool CEngine::initButtonStruct(sButtonStruct &bs, std::vector<std::string> cfg) {

    if (cfg.size() >= CFG_BUTTON_SIZE ) {
        bs.path = getValueItem(cfg[0],"Image: ");
        loginfo("Image Path: "+ bs.path);

        bs.PosX = StringToFloat(getValueItem(cfg[1],"PositionX: "));

        bs.PosY = StringToFloat(getValueItem(cfg[2],"PositionY: "));

        bs.SizeX =  StringToFloat(getValueItem(cfg[3],"SizeX: "));

        bs.SizeY =  StringToFloat(getValueItem(cfg[4],"SizeY: "));

        bs.Enable = StringToInt(getValueItem(cfg[5],"Enable: "));

        bs.ImageRed = StringToFloat(getValueItem(cfg[6],"BGRed: "));

        bs.ImageGreen = StringToFloat(getValueItem(cfg[7],"BGGreen: "));

        bs.ImageBlue = StringToFloat(getValueItem(cfg[8],"BGBlue: "));

        bs.TextRed = StringToFloat(getValueItem(cfg[9],"TextColorRed: "));

        bs.TextGreen = StringToFloat(getValueItem(cfg[10],"TextColorGreen: "));

        bs.TextBlue = StringToFloat(getValueItem(cfg[11],"TextColorBlue: "));

        return true;
    }
    else
        return false;
}





