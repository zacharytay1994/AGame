//#include <iostream>
//#include <vector>
//#include <string>
//#include "music.h"
//
//static std::vector<char*> gPathList;
//
////void ERRCHECK_fn(FMOD_RESULT result, const char* file, int line);
////#define ERRCHECK(_result) ERRCHECK_fn(_result, __FILE__, __LINE__)
//
//
//FMOD::System* sound_system;
//FMOD::Sound* sound1;
//FMOD::Channel* channel = 0;
//FMOD_RESULT       result;
//unsigned int      version;
//void* extradriverdata = 0;
//bool playing = false;
//bool mute = false;
//
//
//
//void sound_init() {
//
//    //Create system object and initailize it
//    result = FMOD::System_Create(&sound_system);
//    //ERRCHECK(result);
//
//
//    result = sound_system->getVersion(&version);
//    //ERRCHECK(result);
//
//
//    if (version < FMOD_VERSION)
//    {
//        std::cout << "FMOD lib version" << version << "doesn't math hearder version" << FMOD_VERSION;
//    }
//
//    //init sound track
//    result = sound_system->init(32, FMOD_INIT_NORMAL, extradriverdata);
//
//    //load tracks
//    //char* filePath = new char;
//
//    //result = sound_system->createSound(Common_MediaPath("drumloop.wav"), FMOD_DEFAULT, 0, &sound1);
//    result = sound_system->createSound("../Assets/Sound/mario.wav", FMOD_DEFAULT, 0, &sound1);
//    result = sound1->setMode(FMOD_LOOP_OFF);  /* drumloop.wav has embedded loop points which automatically makes looping turn on, */
//
//
//
//    //result = sound_system->createSound(Common_SoundPath("drumloop.wav"), FMOD_DEFAULT, 0, &sound1);
//    std::cout << "Sound load";
//}
//
//void sound_update() {
//    //std::cout<< AEFrameRateControllerGetFrameTime()<<std::endl;
//    std::cout << "Sound update" << std::endl;
//
//    //mute
//    if (AEInputCheckTriggered(AEVK_M)) {
//        mute = !mute;
//        //channel->getMute(&mute);
//        //channel->setMute(&mute);
//    }
//    if (!mute) {
//        //Play sound
//        //Player jump
//        if (AEInputCheckTriggered(AEVK_SPACE) && !playing)
//        {
//            result = sound_system->playSound(sound1, 0, false, &channel);
//            std::cout << "sound pressed";
//            //ERRCHECK(result);
//        }
//
//        result = sound_system->update();
//
//        //system pause
//        if (AEInputCheckTriggered(AEVK_P)) {
//            bool paused;
//            result = channel->getPaused(&paused);
//            paused = !paused;
//            result = channel->setPaused(paused);
//        }
//
//        if (channel) {
//
//            result = channel->isPlaying(&playing);
//
//        }
//
//    }
//}
//
//void sound_free() {
//    result = sound1->release();
//}
//
//
////std::string Common_SoundPath(std::string filename) {
////    std::string commonpath = "../Assets/Sound/";
////    std::string filepath = commonpath + filename;
////    return filepath;
////}
//
////const char* Common_MediaPath(const char* filename) {
////    char* filepath= new char;
////    strcat(filepath, "../Assets/Sound/");
////    strcat(filepath, filename);
////    gPathList.push_back(filepath);
////    return filepath;
////}