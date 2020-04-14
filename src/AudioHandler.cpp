#include "redhand/AudioHandler.hpp"

using namespace redhand;

ALCenum error;

AudioHandler::AudioHandler() {
}

inline ALenum AudioHandler::to_al_format(short channels, short samples) {
    bool stereo = (channels > 1);

    switch (samples) {
    case 16:
        if (stereo)
            return AL_FORMAT_STEREO16;
        else
            return AL_FORMAT_MONO16;
    case 8:
        if (stereo)
            return AL_FORMAT_STEREO8;
        else
            return AL_FORMAT_MONO8;
    default:
        return -1;
    }
}

void AudioHandler::CheckErrors() {
    error = alGetError();
    if (error != AL_NO_ERROR){
        std::cout << "There was an error" << std::endl;
    }
}

void AudioHandler::PlaySound(const char *sound_dir) {
    ALint source_state;

    alGetError();

    ALCdevice *device;

    device = alcOpenDevice(NULL);

    ALCcontext *context;

    context = alcCreateContext(device, NULL);

    if (!alcMakeContextCurrent(context))
        std::cout << "Failed to make context" << std::endl;

    CheckErrors();

    ALuint source;

    alGenSources((ALuint)1, &source);
    CheckErrors();

    alSourcef(source, AL_PITCH, 1);
    CheckErrors();
    alSourcef(source, AL_GAIN, 1);
    CheckErrors();
    alSource3f(source, AL_POSITION, 0, 0, 0);
    CheckErrors();
    alSource3f(source, AL_VELOCITY, 0, 0, 0);
    CheckErrors();
    alSourcei(source, AL_LOOPING, AL_FALSE);
    CheckErrors();

    ALuint buffer;

    alGenBuffers((ALuint)1, &buffer);
    CheckErrors();

    WaveInfo *wave;
    char *bufferData;
    int ret;

    wave = WaveOpenFileForReading(sound_dir);
    //std::cout << wave << std::endl;
    if (!wave) {
        fprintf(stderr, "failed to read wave file\n");
        //std::cout << "failed to read wave file\n" << stderr << std::endl;
    }

    ret = WaveSeekFile(0, wave);
    if (ret) {
        fprintf(stderr, "failed to seek wave file\n");
    }

    bufferData = (char *)malloc(wave->dataSize);
    if (!bufferData) {
        perror("malloc");
    }

    ret = WaveReadFile(bufferData, wave->dataSize, wave);
    if (ret != (int)wave->dataSize) {
        fprintf(stderr, "short read: %d, want: %d\n", ret, wave->dataSize);
    }

    alBufferData(buffer, to_al_format(wave->channels, wave->bitsPerSample),
                 bufferData, wave->dataSize, wave->sampleRate);
    CheckErrors();

    alSourcei(source, AL_BUFFER, buffer);
    CheckErrors();

    alSourcePlay(source);
    CheckErrors();

    alGetSourcei(source, AL_SOURCE_STATE, &source_state);
    CheckErrors();
    while (source_state == AL_PLAYING) {
        alGetSourcei(source, AL_SOURCE_STATE, &source_state);
        CheckErrors();
    }

    alDeleteSources(1, &source);
    alDeleteBuffers(1, &buffer);
    device = alcGetContextsDevice(context);
    alcMakeContextCurrent(NULL);
    alcDestroyContext(context);
    alcCloseDevice(device);
}
