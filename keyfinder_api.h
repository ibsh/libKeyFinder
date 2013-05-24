#ifndef KEYFINDER_API_H_
#define KEYFINDER_API_H_

#ifdef WIN32
    #ifdef DLLEXPORT
        #undef DLLEXPORT
    #endif
    #define DLLEXPORT __declspec (dllexport)
#else
    #define DLLEXPORT
#endif

#ifdef __cplusplus
extern "C" {
#endif

DLLEXPORT const char* kfinder_get_key(short signed int   *samples,
                                      unsigned int        nb_samples,
                                      short unsigned int  frame_rate,
                                      short unsigned int  nb_channels);

#ifdef __cplusplus
}
#endif

#endif /* KEYFINDER_API_H_ */
