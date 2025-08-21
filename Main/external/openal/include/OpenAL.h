#include <Windows.h>

#include "stdio.h"
#include "al.h"
#include "alc.h"

#define MYMUSIC 102

class OpenAL
{
public:
    ALuint audioBuffer;
    ALuint audioSource;

    void Play(void)
    {
        alSourcePlay(audioSource);
        if (alGetError() != AL_NO_ERROR)
        {
            printf("alSourcePlay Failed...\n");
        }
    }

    BOOL playFromSeek(ALfloat seek)
    {
        if (seek < 0 || seek > 1)
        {
            printf("playFromSeek() : Invalid Seek Value.\n");
            return FALSE;
        }

        alSourcei(audioSource, AL_BUFFER, audioBuffer);
        if (alGetError() != AL_NO_ERROR)
        {
            printf("playFromSeek() : alSourcei() Failed.\n");
            return FALSE;
        }

        ALint total = 0;
        alGetBufferi(audioBuffer, AL_SIZE, &total);
        if (alGetError() != AL_NO_ERROR)
        {
            printf("playFromSeek() : alGetBufferi() Failed.\n");
            return FALSE;
        }

        alSourcei(audioSource, AL_BYTE_OFFSET, seek * total);
        if (alGetError() != AL_NO_ERROR)
        {
            printf("playFromSeek() : alSourcei() Failed.\n");
            return FALSE;
        }

        alSourcePlay(audioSource);
        if (alGetError() != AL_NO_ERROR)
        {
            printf("alSourcePlay() Failed...\n");
        }

        return TRUE;
    }

    ALfloat getBufferLengthInSeconds(ALuint buffer)
    {
        ALint size, bits, channels, frequency;

        alGetBufferi(audioBuffer, AL_SIZE, &size);
        if (alGetError() != AL_NO_ERROR)
        {
            printf("getBufferLengthInSeconds() : alGetBufferi() Failed For AL_SIZE.\n");
            return -1.0f;
        }

        alGetBufferi(audioBuffer, AL_BITS, &bits);
        if (alGetError() != AL_NO_ERROR)
        {
            printf("getBufferLengthInSeconds() : alGetBufferi() Failed For AL_BITS.\n");
            return -1.0f;
        }

        alGetBufferi(audioBuffer, AL_CHANNELS, &channels);
        if (alGetError() != AL_NO_ERROR)
        {
            printf("getBufferLengthInSeconds() : alGetBufferi() Failed For AL_CHANNELS.\n");
            return -1.0f;
        }

        alGetBufferi(audioBuffer, AL_FREQUENCY, &frequency);
        if (alGetError() != AL_NO_ERROR)
        {
            printf("getBufferLengthInSeconds() : alGetBufferi() Failed For AL_FREQUENCY.\n");
            return -1.0f;
        }

        return (ALfloat)(((ALuint)size / channels / (bits / 8)) / (ALfloat)frequency);
    }

    void playFromSeconds(ALfloat seconds)
    {
        playFromSeek(seconds / getBufferLengthInSeconds(audioBuffer));
    }

    BOOL InitializeAudio()
    {
        // variable declartions
        ALCdevice *audioDevice;
        ALCcontext *alContext;

        HRSRC hResource = NULL;
        HANDLE hAudioBuffer = NULL;

        LPVOID audioBufferData = NULL;
        DWORD sizeOfAudioDatainBytes = 0;

        // code
        audioDevice = alcOpenDevice(NULL); // Open deafult Audio device
        if (alcGetError(audioDevice) != ALC_NO_ERROR)
        {
            printf("InitalizeAudio() : alCOpenDevice() Failed.\n");
            return FALSE;
            // alcGetError() checks Errors for 'alc' functions call just above
        }

        // Step - 2 # CreateOpenAL context.
        alContext = alcCreateContext(audioDevice, NULL); // NULL for default
        if (alcGetError(audioDevice) != ALC_NO_ERROR)
        {
            printf("InitalizeAudio() : alcCreateContext() Failed.\n");
            return FALSE;
        }

        // Step - 3 # similar to wglMakeCurrentcontext(). Make above context as current context
        alcMakeContextCurrent(alContext);
        if (alcGetError(audioDevice) != ALC_NO_ERROR)
        {
            printf("InitalizeAudio() : alcMakeContextCurrent() Failed.\n");
            return FALSE;
        }

        // STEP - # createAudioBuffer
        alGenBuffers(1, &audioBuffer);
        if (alcGetError(audioDevice) != ALC_NO_ERROR)
        {
            printf("InitalizeAudio() : alGenBuffer() Failed.\n");
            return FALSE;
            // alGetError() for error check funcion for above 'al' function.
        }

        // STEP # 5 LOAD WAV RESOURCE DATA
        // [A]: Find WAV Resource, and get its handle
        // Load WAV Resource

        hResource = FindResource(NULL, MAKEINTRESOURCE(MYMUSIC), TEXT("WAVE")); // kontya exe madhe, resource, type of respurce
        if (hResource == NULL)
        {
            printf("InitalizeAudio() : FindResource() Failed.\n");
            return FALSE;
        }

        // 5 [B] Use the Resourec data in memory and returns the handle.
        hAudioBuffer = LoadResource(NULL, hResource);
        if (hAudioBuffer == NULL)
        {
            printf("InitalizeAudio() : LoadResource() Failed.\n");
            return FALSE;
        }

        // C : Get starting byte-offset of this data in memory"Starting address bhetato"
        audioBufferData = LockResource(hAudioBuffer);
        if (audioBufferData == NULL)
        {
            printf("InitalizeAudio() : LockResource() Failed.\n");
            return FALSE;
        }

        // D: Get the size of  our Audio data in Bytes.
        sizeOfAudioDatainBytes = SizeofResource(NULL, hResource);
        if (sizeOfAudioDatainBytes == 0)
        {
            printf("InitalizeAudio() : SizeofResource() Failed.\n");
            return FALSE;
        }

        // # STEP # 6: load the wave data into audio buffer
        // Loadf Buffer Data Into Audio Buffer
        alBufferData(audioBuffer, AL_FORMAT_STEREO16, audioBufferData, sizeOfAudioDatainBytes, 44100); // frequency hrtz
        if (alGetError() != AL_NO_ERROR)
        {
            printf("InitalizeAudio() : alBufferData() Failed.\n");
            return FALSE;
        }

        // set 7 create audio source
        alGenSources(1, &audioSource);
        if (alGetError() != AL_NO_ERROR)
        {
            printf("InitalizeAudio() : alGenSource() Failed.\n");
            return FALSE;
        }

        // step 8 - give audio buffer to audio source
        alSourcei(audioSource, AL_BUFFER, audioBuffer);
        if (alGetError() != AL_NO_ERROR)
        {
            printf("InitalizeAudio() : alSourcei() Failed.\n");
            return FALSE;
        }
        alSourcei(audioSource, AL_LOOPING, AL_TRUE);
        if (alGetError() != AL_NO_ERROR)
        {
            printf("InitalizeAudio() : alSourcei() Failed.\n");
            return FALSE;
        }

        // UNLOAD WAV RESOURCE
        // UnlockResource(hResource);
        // FreeResource(hAudioBuffer);

        return (TRUE);
    }

    void UninitializeAudio(void)
    {
        // variable declartions
        ALCdevice *audioDevice;
        ALCcontext *alContext;

        ALint state;

        // code
        // step - get current status of audio playing or not
        alGetSourcei(audioSource, AL_SOURCE_STATE, &state);
        if (state == AL_PLAYING)
        {
            alSourceStop(audioSource);
            if (alGetError() != AL_NO_ERROR)
                printf("UninitializeAudio() : alSourceStop() Failed.\n");
        }

        alSourcei(audioSource, AL_BUFFER, 0);
        if (alGetError() != AL_NO_ERROR)
            printf("UninitializeAudio() : alSourcei() Failed.\n");

        alDeleteSources(1, &audioSource);
        if (alGetError() != AL_NO_ERROR)
            printf("UninitializeAudio() : alDeleteSpurces() Failed.\n");
        else
            audioSource = 0;

        alDeleteBuffers(1, &audioBuffer);
        if (alGetError() != AL_NO_ERROR)
            printf("UninitializeAudio() : alDeleteBuffers() Failed.\n");
        else
            audioBuffer = 0;

        alContext = alcGetCurrentContext();
        if (alContext == NULL)
            printf("UninitializeAudio() : alcGetCurrentContext() Failed.\n");
        else
        {
            audioDevice = alcGetContextsDevice(alContext);
            if (audioDevice == NULL)
                printf("UninitializeAudio() : alcGetContextDevice() Failed.\n");
        }

        if (alContext)
        {
            alcMakeContextCurrent(NULL);
            alcDestroyContext(alContext);
            alContext = NULL;
        }

        if (audioDevice)
        {
            alcCloseDevice(audioDevice);
            audioDevice = NULL;
        }
    }
};